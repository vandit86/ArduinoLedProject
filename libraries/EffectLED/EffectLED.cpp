

/*
  EffectLED.cpp - Library for flashing Morse code.
*/

#include "EffectLED.h"

EffectLED::EffectLED(){}

// constructor receive number of leads on strip 
EffectLED::EffectLED(int numLeds, CRGB * leds, long timeInLoop){

  this->leds = leds;  
  this->setNumLeds(numLeds);
  til = timeInLoop;   
  
}


//static void EffectLED::addLeds(unsigned char dp, unsigned char clockPin) {
// //FastLED.addLeds<WS2801, dataPin , clockPin, RBG>(this->leds, numLeds); 
//} 

// set number of leds to be controlled 
void EffectLED::setNumLeds (int numLeds){
	NUM_LEDS = numLeds; 
}


// set all led to black 
void EffectLED::clear_led (){
  for(int i=0; i<(this->NUM_LEDS); i++){
    leds[i] = CRGB(0,0,0);
  }    
  FastLED.show();

}


void EffectLED::setPixel(int Pixel, byte red, byte green, byte blue) {
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}

void EffectLED::setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void EffectLED::showStrip() {
   // FastLED
   FastLED.show();
}


// Auxiliary function 
// Remove Spaces From String
void EffectLED::trimStr (char* str){
  int i,j=0;
  for(i=0;str[i]!='\0';++i)
  {
    if(str[i]!=' ')
      str[j++]=str[i];
  }
  str[j]='\0';
}





