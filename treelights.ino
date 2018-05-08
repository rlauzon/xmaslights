#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 11

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  Serial.begin(19200);
  strip.begin();
  for(int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i,get_random_color());
  }
  strip.show();
}

void loop() {
  // Go through the pixels
  for(int i=0; i < strip.numPixels(); i++) {
    // and update the color
    update_color(i);
  }
	
  // Update the pixels
  strip.show();
	
  // Pause to let people enjoy the colors
  delay(100);
}

uint32_t update_color(int pixel_index) {
  uint32_t c;
  uint8_t r, g, b;
  int clr;

  // Get the current color
  c = strip.getPixelColor(pixel_index);

  // Split up the RGB values
  r = (uint8_t)(c >> 16) & 255;
  g = (uint8_t)(c >>  8) & 255;
  b = (uint8_t)c & 255;

  // If the values are too dim, reset the color
  if (r == 0 && g == 0 && b == 0) {
    c = get_random_color();
  }
  else { // otherwise
    // Pick a color element to reduce
    clr = random(0,3);
    
    // And reduce that color element by half
    switch(clr) {
      case 0:
        r = reduce_color(r);
        break;
      case 1:
        g = reduce_color(g);
        break;
      case 2:
        b = reduce_color(b);
        break;
    }
    
    c = strip.Color(r,g,b);
  }
  
  strip.setPixelColor(pixel_index,c);
}

//#define reducer 10

uint8_t reduce_color(uint8_t old_color) {
  uint8_t new_color;
  int reducer;
  
  reducer = random(5,20);
  
  if (old_color < reducer) {
    new_color = 0;
  } else  {
    new_color = old_color - reducer;
  }
  
  return new_color;
}

uint32_t get_random_color() {
  int red, green, blue;

  // Generate a random red, green, blue value
  // and randomly decide to turn off the red, green or blue part
  red = random(128,255) * random(0,2);
  green = random(128,255) * random(0,2);
  blue = random(128,255) * random(0,2);

  return strip.Color(red, green, blue);
}


