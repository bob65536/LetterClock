/* This project will drive a Qclock (a clock with letters)
*  First point: drive 8 LED (WS2802). Then, 100. Then, add a RTC.
 *  Then, a LDR. Then, some buttons. Then, other things.

Credits to : NeoPixel Ring simple sketch (c) 2013 Shae Erisson
Released under the GPLv3 license to match the rest of the
Adafruit NeoPixel library

 */

/* Libs to include ***********************************************************/
#include "letterClock.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"

#define NUMPIXELS       100 // How many pixels in there (per pin)
#define BRIGHT          60 // Brightness, between 0 and 255
#define SATURATION      240 // For colors: we want bright colors so set it to max!
#define PERIOD_COLORS   300 // Cycle through all colors every x seconds

#define DELAYVAL        600 // Time (in milliseconds) to pause between pixels

/* Defines *******************************************************************/

#define RTC                         1 // Enable RTC support
#define ENABLE_LDR_SUPPORT          1
// enum    Board                   NANO; // Arduino Nano is used

/* Pinouts *******************************************************************/
/* 
    Define pinouts. Moreover, I2C pins differ depending on boards. Be careful
    For Pro Mini: (SDA, SCL) = (A4, A5)
    For Nano: (SDA, SCL) = (4, 5) (Digital pins)
*/
#define PIN             6 // If all in one wire

#ifdef ENABLE_LDR_SUPPORT
    #define LDR_PIN     A3
#endif

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/* Arrays to turn on (struct of pointers to array ) */
ArraysToTurnOn_t arraysToTurnOn; // Led to light NOW (in this loop)
ArraysToTurnOn_t arraysAlreadyOn; // LED already turned on
Time_t currTime;

void setTime(void)
{
    char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    // year, month, date, hour, min, sec and week-day (starts from 0 for Sunday and goes to 6)
    DateTime dt(2020, 05, 16, 14, 59, 30, 6);
    rtc.setDateTime(dt);
}

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
    int adc = 1023 - analogRead(LDR_PIN); /* Get ADC reading (from 0 to 1023). I swapped VCC and GND, BTW */
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

uint32_t setColor(int brightness, int minutes, int seconds)
{
    uint32_t secondsSinceHueIsZero = (uint32_t)seconds + ((uint32_t)minutes % (PERIOD_COLORS/60) * 60); // Between 0 and 299

    uint32_t hue = (((uint32_t)65534u * (uint32_t)secondsSinceHueIsZero) / PERIOD_COLORS); 
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
    #ifdef SET_TIME
        setTime();
    #endif
    Wire.begin();
    rtc.begin();
    delay(200); // To make sure everything is loaded (?)
    updateTime(&currTime);
    updateArraysToTurnOn(&arraysAlreadyOn, currTime); // Init variable
#endif
#ifdef ENABLE_LDR_SUPPORT
    pinMode(LDR_PIN, INPUT);
#endif
}

void loop() 
{
#ifdef RTC

    updateTime(&currTime);
    updateArraysToTurnOn(&arraysToTurnOn, currTime);

#ifdef ENABLE_LDR_SUPPORT
    uint8_t brightness = getBrightnessLdr();
    uint32_t colorToUse = setColor(brightness, currTime.m, currTime.s);
#else
    uint32_t colorToUse = setColor(BRIGHT, currTime.m, currTime.s);
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
