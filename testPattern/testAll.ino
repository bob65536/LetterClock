// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        7 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 100 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int heart[34] = {2,3,8,9,11,12,13,14,17,18,19,20,21,24,25,26,27,30,31,35,36,40,41,50,52,59,63,68,73,78,84,87,95,96};
int innerHeart[34] = {22,23,28,29,32,33,34,37,38,39,42,43,44,45,46,47,48,49,53,54,55,56,57,58,64,65,66,67,74,75,76,77,85,86};
#define DELAYVAL 5000 // Time (in milliseconds) to pause between pixels
#define BRIGHT 10

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for (int bright = 40;bright<-50; bright+=10)
  {
    
    for(int i=0; i<34; i++) { // For each pixel...
      pixels.setPixelColor(heart[i]-1, pixels.Color(bright, 0, 0));
      pixels.setPixelColor(innerHeart[i]-1, pixels.Color(bright/2, 0, 0));
    }
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL);

    for(int i=0; i<34; i++) { // For each pixel...
      pixels.setPixelColor(heart[i]-1, pixels.Color(0, bright, 0));
      pixels.setPixelColor(innerHeart[i]-1, pixels.Color(0, bright/2, 0));
    }
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL);

    for(int i=0; i<34; i++) { // For each pixel...
      pixels.setPixelColor(heart[i]-1, pixels.Color(0, 0, bright));      
      pixels.setPixelColor(innerHeart[i]-1, pixels.Color(0, 0, bright/2));

    }
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL);
    
    for(int i=0; i<34; i++) { // For each pixel...
      pixels.setPixelColor(heart[i]-1, pixels.Color(bright, bright, bright));
    }
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL);
  }
  
  for (int i=0;i<100;i++) {
    pixels.setPixelColor(i, pixels.Color(BRIGHT, 0, 0));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.

  delay(DELAYVAL);
  for (int i=0;i<100;i++) {
    pixels.setPixelColor(i, pixels.Color(0, BRIGHT, 0));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.

  delay(DELAYVAL);
  for (int i=0;i<100;i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, BRIGHT));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.

  delay(DELAYVAL);
  for (int i=0;i<100;i++) {
    pixels.setPixelColor(i, pixels.Color(BRIGHT, BRIGHT, BRIGHT));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(DELAYVAL*2);
  
      
  
  
  // delay(10000);
}
