/* This project will drive a Qclock (a clock with letters)
*  First point: drive 8 LED (WS2802). Then, 100. Then, add a RTC.
 *  Then, a LDR. Then, some buttons. Then, other things.

Credits to : NeoPixel Ring simple sketch (c) 2013 Shae Erisson
Released under the GPLv3 license to match the rest of the
Adafruit NeoPixel library

 */

/* Defines *******************************************************************/

#define RTC                         1 // Enable RTC support
#define LIGHT_ALL_PRECISE_MINUTES   1 // Display only current minute or show previous ones (cf issue #10)
#define ENABLE_LDR_SUPPORT          1

/* Libs to include ***********************************************************/
#include "letterClock.h"
#include <Adafruit_NeoPixel.h>
#ifdef RTC
    #include <Wire.h>
    #include "Sodaq_DS3231.h"
    #define VIN_RTC     10 // Add Vin output to feed RTC module
#endif


/* Pinouts *******************************************************************/
#define PIN         7 // If all in one wire

#ifdef ENABLE_LDR_SUPPORT
    #define LDR_PIN     A2
    #define LDR_VCC     6 /* Pin for sending 5V to divider bridge */
    #define LDR_GND     5 /* Pin for GND for divider bridge */   
#endif


/* Constants *****************************************************************/
#define NUMPIXELS   100 // How many pixels in there (per pin)
#define BRIGHT      24 // Brightness, between 0 and 255
#define SATURATION  240 // For colors: we want bright colors so set it to max!

#define DELAYVAL    600 // Time (in milliseconds) to pause between pixels

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/* Arrays to turn on (struct of pointers to array ) */
ArraysToTurnOn_t arraysToTurnOn; // Led to light NOW (in this loop)
ArraysToTurnOn_t arraysAlreadyOn; // LED already turned on
Time_t currTime;



int* getHour(int h, int m)
{
    int hMod = h;
    if (m > 34)
    {
        hMod = h + 1; /* eg: 1:40 -> Twenty to TWO */
    }
    int hMod12 = hMod % 12; /* To have something between 0 and 11 (here, 0 is like 12) */

    int pRes = m0;
    switch (hMod12)
    {
        case 0:
            pRes = h12;
            break;
        case 1:
            pRes = h1;
            break;
        case 2:
            pRes = h2;
            break;
        case 3:
            pRes = h3;
            break;
        case 4:
            pRes = h4;
            break;
        case 5:
            pRes = h5;
            break;
        case 6:
            pRes = h6;
            break;
        case 7:
            pRes = h7;
            break;
        case 8:
            pRes = h8;
            break;
        case 9:
            pRes = h9;
            break;
        case 10:
            pRes = h10;
            break;
        case 11:
            pRes = h11;
            break;
        default:
            break;
    }
    return pRes;
}


int* getHourAmPm(int h, int m)
{
    int hMod = h;
    if (m > 34)
    {
        hMod = h + 1; /* eg: 1:40 -> Twenty to TWO */
    }
    hMod = hMod % 24; /* If 23:55, say five to 12 AM, even if h > 11 */
    if (hMod > 11)
    {
        return pm;
    }
    else
    {
        return am;
    }
    
}

int* getMinutes(int m)
{
    int pRes = m0;
    if (((m >= 5) && (m < 10)) || ((m >= 55) && (m < 60)))
    {
        pRes = m5;
    }
    else if (((m >= 10) && (m < 15)) || ((m >= 50) && (m < 55)))
    {
        pRes = m10;
    }
    else if (((m >= 15) && (m < 20)) || ((m >= 45) && (m < 50)))
    {
        pRes = m15;
    }
    else if (((m >= 20) && (m < 25)) || ((m >= 40) && (m < 45)))
    {
        pRes = m20;
    }
    else if (((m >= 25) && (m < 30)) || ((m >= 35) && (m < 40)))
    {
        pRes = m25;
    }
    else if ((m >= 30) && (m < 35))
    {
        pRes = m30;
    }
    else
    {
        pRes = m0;
    }
    
    return pRes;
}

int* getPreciseMinutes(int m)
{
    int mMod5 = m % 5;
    int pRes = m0;
    switch (mMod5)
    {
    case 0:
        pRes = m0;
        break;
    case 1:
        pRes = m1;
        break;
    case 2:
        pRes = m2;
        break;
    case 3:
        pRes = m3;
        break;
    case 4:
        pRes = m4;
        break;
    default:
        break;
    }
    return pRes;
}

int* getPasTo(int m)
{
    int pRes = NULL;
    if (m >= 35 && m < 60)
    {
        pRes = to;
    }
    else if (m >= 5 && m < 35)
    {
        pRes = past;
    }
    else
    {
        pRes = oclock;
    }
    return pRes;
}

int getSizeArray(int* pTable)
{
    return pTable[0];
}

void updateArraysToTurnOn(ArraysToTurnOn_t* pArray, Time_t currTime)
{
    pArray->minutes = getMinutes(currTime.m);
    pArray->preciseMinutes = getPreciseMinutes(currTime.m);
    pArray->pasTo = getPasTo(currTime.m);
    pArray->hours = getHour(currTime.h, currTime.m);
    pArray->amPm = getHourAmPm(currTime.h, currTime.m);
}

void turnLedOnArray(int* pTable, uint32_t rgbColors)
{
    int sizeArray = getSizeArray(pTable);

    // Serial.print(sizeArray);
    // Serial.println(" LED to light.");
    for (int i = 1; i <= sizeArray; i++) 
    { 
        // Serial.print("Turning LED #");
        // Serial.println(pTable[i]);
        pixels.setPixelColor(pTable[i]-1, rgbColors);
    }
    pixels.show();   // Send the updated pixel colors to the hardware.
}

void turnLedOffArray(int* pTable)
{
    int sizeArray = getSizeArray(pTable);

    // Serial.print(sizeArray);
    // Serial.println(" LED to light.");
    for (int i = 1; i <= sizeArray; i++) 
    { 
        // Serial.print("Turning LED #");
        // Serial.println(pTable[i]);
        pixels.setPixelColor(pTable[i]-1, pixels.Color(0, 0, 0));
    }
    pixels.show();   // Send the updated pixel colors to the hardware.
}

void refreshLedArray(int* pOldTable, int* pNewTable, uint32_t rgbColors)
{
    if (pOldTable != pNewTable)
    {
        turnLedOffArray(pOldTable);
    }
    turnLedOnArray(pNewTable, rgbColors);
}

uint8_t getBrightnessLdr(void)
{
    int res = 0;
    int adc = analogRead(LDR_PIN); /* Get ADC reading (from 0 to 1023) */
    if (adc < 40)
    {
        // y = 1.75x + 20
        int ax = (7 * adc) >> 2; // divide by 4
        res = ax + 20;
    }
    else if (adc > 920)
    {
        // y = 0.5x - 260
        int ax = adc >> 1; // divide by 2
        res = ax - 260;
    }
    else
    {
        // y = 0.125x + 85
        int ax = adc >> 3; // divide by 8
        res = ax + 85;
    }
    Serial.print("¤¤¤ Brightness: ");
    Serial.print(res);
    Serial.print(" ADC: ");
    Serial.println(adc);
    
    if (res < 0)
    {
        return 0;
    }
    else if (res > 255)
    {
        return 255;
    }
    else
    {
        return (uint8_t)res;
    }
}

void updateTime(Time_t* pTime)
{
    DateTime now = rtc.now(); //get the current date-time
    pTime->h = now.hour();
    pTime->m = now.minute();
    pTime->s = now.second();
    // int dy = now.day();
    // int mo = now.month();
    // int yr = now.year();
    Serial.print("/// RTC says: time is ");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());
}

uint32_t setColor(int brightness, int seconds)
{
    uint32_t hue = (((uint32_t)65530u * (uint32_t)seconds) / 60u); 
    // In the end, hue should be between 0 and 65535!

    uint32_t rgb = Adafruit_NeoPixel::ColorHSV(hue, SATURATION, brightness); 
    // Serial.print("&&& My color: ");
    // Serial.println(hue);
    
    // return Adafruit_NeoPixel::gamma32(rgb); // To get vivid colors
    return (rgb);
}   

void setup() 
{
    Serial.begin(9600);
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
#ifdef RTC
    pinMode(VIN_RTC, OUTPUT);
    digitalWrite(VIN_RTC, HIGH);
    Wire.begin();
    rtc.begin();
    delay(200); // To make sure everything is loaded (?)
    updateTime(&currTime);
    updateArraysToTurnOn(&arraysAlreadyOn, currTime); // Init variable
#endif
#ifdef ENABLE_LDR_SUPPORT
    pinMode(LDR_PIN, INPUT);
    pinMode(LDR_VCC, OUTPUT);
    digitalWrite(LDR_VCC, HIGH);
    pinMode(LDR_GND, OUTPUT);
    digitalWrite(LDR_GND, LOW);
#endif
}

void loop() 
{
#ifdef RTC

    updateTime(&currTime);
    updateArraysToTurnOn(&arraysToTurnOn, currTime);
#ifdef ENABLE_LDR_SUPPORT
    uint8_t brightness = getBrightnessLdr();
    uint32_t colorToUse = setColor(brightness, currTime.s);
#else
    uint32_t colorToUse = setColor(BRIGHT, currTime.s);
#endif
    // Serial.print("*** It is ");
    // Serial.print(currTime.h);
    // Serial.print(":");
    // Serial.println(currTime.m);    
 
    turnLedOnArray(itis, colorToUse);
    refreshLedArray(arraysAlreadyOn.minutes ,arraysToTurnOn.minutes, colorToUse);
    refreshLedArray(arraysAlreadyOn.preciseMinutes, arraysToTurnOn.preciseMinutes, colorToUse);
    refreshLedArray(arraysAlreadyOn.pasTo ,arraysToTurnOn.pasTo, colorToUse);
    refreshLedArray(arraysAlreadyOn.hours ,arraysToTurnOn.hours, colorToUse);
    refreshLedArray(arraysAlreadyOn.amPm ,arraysToTurnOn.amPm, colorToUse);
  

    updateArraysToTurnOn(&arraysAlreadyOn, currTime); // Copy arraysToTurnOn on arraysAlreadyOn 
    delay(DELAYVAL);
#else
    pixels.clear(); // Set all pixel colors to 'off'
    for (int h = 0; h < 24; h++)
    {
        for (int m = 0; m < 60; m++)
        {
            pixels.clear(); // Set all pixel colors to 'off'

            Serial.print("*** It is ");
            Serial.print(h);
            Serial.print(":");
            Serial.println(m);
            
            turnLedOnArray(itis, BRIGHT, 0, 0);
            turnLedOnArray(getMinutes(m), BRIGHT, 0, 0);
            turnLedOnArray(getPreciseMinutes(m), BRIGHT, 0, 0);
            turnLedOnArray(getPasTo(m), BRIGHT, 0, 0);
            turnLedOnArray(getHour(h, m), BRIGHT, 0, 0);
            turnLedOnArray(getHourAmPm(h, m), BRIGHT, 0, 0);
            delay(DELAYVAL);
        }
    }
#endif
}
