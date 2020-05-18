/* 
    An interesting description
*/

// Includes & Defines
#define LIGHT_ALL_PRECISE_MINUTES   1 // Display only current minute or show previous ones (cf issue #10)
#define FR // Define locale (EN for English, FR for French)


// Struct

typedef struct
{
    int h;
    int m;
    int s;
} Time_t;

typedef struct
{
    int* minutes;
    int* preciseMinutes;
    int* pasTo;
    int* hours;
    int* amPm;
} ArraysToTurnOn_t;

// typedef enum
// {
//     NANO;
//     PRO_MINI;
// } Boards;


/* 
    Define line of letters for writing words
    Note: if count from 0, remove 1 from these values!
    Also: First value is the length of array. Do not turn the LED ID on!!!
*/
#ifdef EN
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

#endif // ifdef EN

#ifdef FR 
// Always on ("it is...")
int itis[] = {5, 1,2,4,5,6};

// Hours
int h1[] = {3, 8,9,10};
int h2[] = {4, 41,43,44,45};
int h3[] = {5, 16,17,18,19,20};
int h4[] = {6, 32,33,34,35,36,37};
int h5[] = {4, 37,38,39,40};
int h6[] = {3, 14,15,16};
int h7[] = {4, 27,28,29,30};
int h8[] = {4, 47,48,49,50};
int h9[] = {4, 42,43,44,46};
int h10[] = {3, 11,12,13};
int h11[] = {4, 22,24,25,26};
int h12[] = {5, 21,22,23,25,26};

int am[] = {2, 99,100};
int pm[] = {2, 97,98};

// Past/To/OClock
int past[] = {0};
int to[] = {5, 61,62,63,64,65};
int oclock[] = {0};

// Minutes
int m5[]  = {4, 71,72,73,74};
int m10[] = {3, 83,84,85};
int m15[] = {7, 51,52,86,87,88,89,90}; // et quart
int m45[] = {7, 81,82,86,87,88,89,90}; // [moins] le quart
int m20[] = {5, 76,77,78,79,80};
int m25[] = {10, 71,72,73,74,75,76,77,78,79,80};
int m30[] = {7, 51,52,66,67,68,69,70};
#endif // ifdef FR

// Precise minutes
#ifdef LIGHT_ALL_PRECISE_MINUTES // Show all precise minutes (0-4 LED)
int m0[] = {0};
int m1[] = {1, 94};
int m2[] = {2, 93,94};
int m3[] = {3, 92,93,94};
int m4[] = {4, 91,92,93,94};
#else // Show only current precise minutes (0-1 LED)
int m0[] = {0};
int m1[] = {1, 94};
int m2[] = {1, 93};
int m3[] = {1, 92};
int m4[] = {1, 91};
#endif

// Prototypes

/*
    Returns a pointer with the right LEDs to turn on (for hours)
*/
int* getHour(int h, int m);

/*
    Returns a pointer with the right LEDs to turn on (for AM/PM)
*/
int* getHourAmPm(int h, int m);

/*
    Returns a pointer with the right LEDs to turn on (for minutes (+/- 5'))
*/
int* getMinutes(int m);

/*
    Returns a pointer with the right LEDs to turn on (for minutes (+/- 1'))
*/
int* getPreciseMinutes(int m);

/*
    Returns a pointer with the right LEDs to turn on (for the right words to use (past, to, o'clock))
*/
int* getPasTo(int m);

int getSizeArray(int* pTable);

void updateArraysToTurnOn(ArraysToTurnOn_t* pArray, Time_t currTime);

void turnLedOnArray(int* pTable, uint32_t rgbColors);

void turnLedOffArray(int* pTable);

void refreshLedArray(int* pOldTable, int* pNewTable, uint32_t rgbColors);

uint8_t getBrightnessLdr(void);

void updateTime(Time_t* pTime);

uint32_t setColor(int brightness, int minutes, int seconds);