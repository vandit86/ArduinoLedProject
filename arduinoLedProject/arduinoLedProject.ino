/* 
arduinoLedProject.ino 
CLI interface to configure effects on led strip
find more detailed description on attached .pdf file 
sketch for use with EffectLed and FastLed librarys
autor: Vadym Hapanchak 

source : https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/

*/

#include "FastLED.h"

#define NUM_LEDS 31     // How many leds in your strip?
#define NUM_EFF 7       // number of effects 

// GND to common GND 
#define DATA_PIN 5    // SI 
#define CLOCK_PIN 7   // CLK

CRGB leds[NUM_LEDS]; 

unsigned long previousMillis = 0;                 // will store last time LED was updated
const long interval = 7500;                       // interval at which to blink (milliseconds)
uint8_t exePos = 6;                               // effect on execution  
uint8_t red =   255;
uint8_t green = 255;
uint8_t blue =  255; 
uint8_t brightness = 50; 


// runner 
uint8_t r_pos = 0 ; 
uint8_t r_size = 5 ;
uint8_t r_del = 100; 

//cylon 
uint8_t c_size = 5; 
uint8_t c_del = 100; 

//twinkie
uint8_t t_count = 5;                    //  how many pixels will be done in one run
uint8_t t_del = 100;                    //  time will be paused between individual pixels


void setup() { 

  
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RBG>(leds, NUM_LEDS);
 

}

void loop() {

  unsigned long currentMillis = millis();
  // search the next effect to be executed (increment pos)
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    exePos = random (NUM_EFF);                  // change to next effect if interval is passed 
    setRandom();                                  // cange global random values 
   
  }

  runEffect(exePos); // run selected effect 

}

// 
void setRandom(){
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random());
  red = random(256);
  green = random(256);
  blue = random(256);
  brightness = random (10,256);
  FastLED.setBrightness(brightness); 

  r_del = random (20,1000);
  r_size = random(1,NUM_LEDS);

  c_size = random (1,NUM_LEDS/3);
  c_del = random (50, 250);  

  t_count = random (1,NUM_LEDS/3); 
  t_del = random (100, 500); 
  
}

void showStrip() {
   // FastLED
   FastLED.show();
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

// set all led to black 
void clear_led (){
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB(0,0,0);
  }    
  FastLED.show();

}

/** EFFECTS Functions  */

void runEffect (int num){

  switch (num){
    case 0 : fadeInfadeOut(); break;  
    case 1 : runner(); break; 
    case 2 : cylon (); break;
    case 3 : twinkle (); break;
    case 4 : rlight(); break;
    case 5 : rainbowCycle(); break; 
    case 6 : theaterChase(); break;
     
  }
}


/* O */
void fadeInfadeOut () {
  float r, g, b;
  uint8_t del = random (10); // random delay      
    for(int k = 0; k < 256; k=k+1) {
      r = (k/256.0)*red;
      g = (k/256.0)*green;
      b = (k/256.0)*blue;
      setAll(r,g,b);
      showStrip();
      delay (del); 
    }
       
    for(int k = 255; k >= 0; k=k-2) {
      r = (k/256.0)*red;
      g = (k/256.0)*green;
      b = (k/256.0)*blue;
      setAll(r,g,b);
      showStrip();
      delay (del); 
    }
}

/* 1 */
void runner (){
    clear_led();
    if(r_pos <= NUM_LEDS - r_size) { // Реализация 
      for(uint8_t j = 0; j < r_size; ++j) {
        setPixel (r_pos + j, red, green, blue); 
      }
      FastLED.show();                       // show effect
      delay (r_del); 
      ++r_pos;
    } 
    else {
      r_pos = 0;
    }
}


/*2*/
void cylon (){
  for(int i = 0; i < NUM_LEDS-c_size-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= c_size; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+c_size+1, red/10, green/10, blue/10);
    showStrip();
    delay(c_del);
  }

  delay(50);

  for(int i = NUM_LEDS-c_size-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= c_size; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+c_size+1, red/10, green/10, blue/10);
    showStrip();
    delay(c_del);
  }
 
  delay(50);
  
}

/* 3 */

void twinkle() {
  setAll(0,0,0);
 
  for (int i=0; i< t_count; i++) {
     setPixel(random(NUM_LEDS),red,green,blue);
     showStrip();
     delay(t_del);
   }
  delay(t_del);
}


/* 4 */
void rlight() {
  int Position=0;
 
  for(int j=0; j<NUM_LEDS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      showStrip();
      delay(c_del);
  }
}

/* 5 */

void rainbowCycle() {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(10);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

/* 6 */
void theaterChase() {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        setPixel(i+q, red, green, blue);    //turn every third pixel on
      }
      showStrip();
     
      delay(c_del);
     
      for (int i=0; i < NUM_LEDS; i=i+3) {
        setPixel(i+q, 0,0,0);        //turn every third pixel off
      }
    }
  }
}
