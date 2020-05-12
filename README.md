## LetterClock

_Read time with class and only with letters!_

This Arduino project is something I wanted to do during lockdown. It was not really fun until you get good results (unfortunately not before soldering three hundred wires).

To have your own version without electronics (a spreadsheet), go see MatrixLetters.ods
If you feel adventurous, feel free to use this project as an inspiration.

In the future, I will add a Bill of Materials (BOM) and detailled instructions. 

# Required

To get started, you will need a few tools:
* Hardware
- An **ATMega microcontroller**, with its FTDI chip (the one in Arduino cards -- no matter which, as long as it is little enough. I used a Pro Mini). Note for FTDI: in almost all cards, it is included.
- **100 WS2811B LEDs** (with the microcontroller). Take a few more because you may kill a couple of LEDs.
- A **soldering iron** (and the accessories: tin, unsoldier, scalpel, etc).
- A few meters of cable (multiwire is prefered)
- Some sheets of paper as a quick and cheap support (will not be seen)
- **Multimeter** for testing soldiers (and checking for short circuits).

* Software
- **Arduino IDE**
- VSCode (optional -- but integration for buildind on _Arduini_ is quite good)

* Librairies used
- **Adafruit_NeoPixel** : I can't say how many times the excellent work of Ladyada helped me to 
