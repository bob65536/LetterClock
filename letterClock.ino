/* This project will drive a Qclock (a clock with letters)
*  First point: drive 8 LED (WS2802). Then, 100. Then, add a RTC.
 *  Then, a LDR. Then, some buttons. Then, other things.

Credits to : NeoPixel Ring simple sketch (c) 2013 Shae Erisson
Released under the GPLv3 license to match the rest of the
Adafruit NeoPixel library

Progress: 
    Rev 0.0.1: Nothing is ready yet!
    Rev 0.0.2: Implementation finished but nothing works (bugs)
    Rev 0.1: just looping with rendom time (test)
    Rev 0.2: adding support of RTC
>   Rev 1: Show the right time
    Rev 1.1: Support LDR to adapt brightness
    Rev 1.2: Add buttons to set time
    Rev 1.3: Fancy colors
    Rev 2: Add demo switches to show nice things (heart, etc)
    Rev 3: Add alarm and buzzer
    Rev 4: ???

 */


#define RTC         1 // Enable RTC support


#include <Adafruit_NeoPixel.h>
#ifdef RTC
    #include <Wire.h>
    #include "Sodaq_DS3231.h"
    #define VIN_RTC     10 // Add Vin output to feed RTC module
#endif


#define PIN         7 // If all in one wire

// #define PIN1  4
// #define PIN2  5
// #define PIN3  6
// #define PIN4  7
// #define PIN5  8

#define NUMPIXELS   100 // How many pixels in there (per pin)
#define BRIGHT      40 // Brightness, between 0 and 255

#define DELAYVAL    5000 // Time (in milliseconds) to pause between pixels

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/* 
    Define line of letters for writing words
    Note: if count from 0, remove 1 from these values!
    Also: First value is the length of array. Do not turn the LED ID on!!!
*/
// Always on ("it is...")
int itis[] = {4, 1,2,4,5};

// Hours
int h1[] = {3, 76,77,78};
int h2[] = {3, 78,79,80};
int h3[] = {5, 41,42,43,44,45};
int h4[] = {4, 31,32,33,34};
int h5[] = {4, 67,68,69,70};
int h6[] = {3, 81,82,83};
int h7[] = {5, 71,72,73,74,75};
int h8[] = {5, 53,54,55,56,57};
int h9[] = {4, 57,58,59,60};
int h10[] = {3, 51,52,53};
int h11[] = {6, 45,46,47,48,49,50};
int h12[] = {6, 61,62,63,64,65,66};

int am[] = {2, 99,100};
int pm[] = {2, 97,98};

// Past/To/OClock
int past[] = {4, 37,38,39,40};
int to[] = {2, 36,37};
int oclock[] = {6, 85,86,87,88,89,90};

// Minutes
int m5[]  = {4, 11,12,13,14};
int m10[] = {3, 28,29,30};
int m15[] = {8, 8,21,22,23,24,25,26,27};
int m20[] = {6, 15,16,17,18,19,20};
int m25[] = {10, 11,12,13,14,15,16,17,18,19,20};
int m30[] = {4, 7,8,9,10};

int m0[] = {0};
int m1[] = {1, 94};
int m2[] = {1, 93};
int m3[] = {1, 92};
int m4[] = {1, 91};

/*
    This function returns a pointer with the right LEDs to turn on
    In this case, it will be the right table for hours
*/
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

/*
    This function returns a pointer with the right LEDs to turn on
    In this case, it will be the right table for AM or PM (US style)
*/
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

/*
    This function returns a pointer with the right LEDs to turn on
    In this case, it will be the right table for minutes (precision: 5 min)
*/
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

/*
    This function returns a pointer with the right LEDs to turn on
    In this case, it will be the right table for minutes (add 0 to 4 minutes)
*/
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

/*
    This function returns a pointer with the right LEDs to turn on
    In this case, it will be the right table for the right words to use (past, to, o'clock)
*/
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

void turnLedOnArray(int* pTable, int r, int g, int b)
{
    int sizeArray = getSizeArray(pTable);

    Serial.print(sizeArray);
    Serial.println(" LED to light.");
    for (int i = 1; i <= sizeArray; i++) 
    { 
        // Serial.print("Turning LED #");
        // Serial.println(pTable[i]);
        pixels.setPixelColor(pTable[i]-1, pixels.Color(r, g, b));
    }
    pixels.show();   // Send the updated pixel colors to the hardware.
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
    delay(2000);
#endif
}

uint32_t old_ts;

void loop() 
{
    Serial.println("*** Start loop ***");

#ifdef RTC
    DateTime now = rtc.now(); //get the current date-time
    uint32_t ts = now.getEpoch();
    int h = now.hour();
    int m = now.minute();
    int s = now.second();
    // int dy = now.day();
    // int mo = now.month();
    // int yr = now.year();

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
    delay(15000);
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
