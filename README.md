# LetterClock

_Read time with class and only with letters!_

![Picture of the finished project](https://raw.githubusercontent.com/bob65536/LetterClock/master/pics/photoClock_800.jpg "Photo of the finished project. Wonderful!")

This Arduino project is something I wanted to do during lockdown. It was not really fun until you get good results (unfortunately not before soldering three hundred wires).

To have your own version without electronics (a spreadsheet), go see MatrixLetters.ods
If you feel adventurous, feel free to use this project as an inspiration.

In the future, I will add a Bill of Materials (BOM) and detailled instructions. 

## Required

To get started, you will need a few tools:
### Hardware
- An **ATMega microcontroller**, with its FTDI chip (the one in Arduino cards -- no matter which, as long as it is little enough. I used a Nano). Note for FTDI: in almost all cards, it is included.
- A **Mini USB cable** (for Arduino Nano).
- **100 WS2811B LEDs** (with the microcontroller). Take a few more because you may kill a couple of LEDs.
- **RTC module (DS3231)**. It keeps time in memory when you cut power to the clock. Useful if you don't want to reset time. DS1307 are good but time drifts a lot (2 seconds/day). Note on modules: you can put a battery on it so this is really convenient!
- A **soldering iron** (and the accessories: tin, unsoldier, scalpel, etc).
- A few meters of cable (multiwire is prefered).
- Some sheets of paper as a quick and cheap support (will not be seen).
- **Multimeter** for testing soldiers (and checking for short circuits).

#### For a nice final touch
- Something that can scatter light (I used tissues and parts from a dumped television).
- A frame (I used a 30cm x 30cm here).
- Transparent sheets (Rhodoid) to print [this](https://github.com/bob65536/LetterClock/blob/master/toPrint_MatrixLettersRV.pdf). You can use regular paper if you don't have this in stock.  

### Software
- **Arduino IDE**
- VSCode (optional -- but integration for buildind on _Arduini_ is quite good).

### Librairies used
- **[Adafruit_NeoPixel](https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use)** : I can't say how many times ressources offered by [Adafruit](https://learn.adafruit.com/) helped me for my projects. 
- **[Sodaq_DS3231](https://github.com/SodaqMoja/Sodaq_DS3231)** : Quick and easy to use. Likewise, they have developped a lot of libraries. If you are in Arduino world, it is likely you used at least once their libs!
- **[Wire](https://www.arduino.cc/en/Reference/Wire)** : Built-in library for supporting I2C interface. Required for using RTC module. 
