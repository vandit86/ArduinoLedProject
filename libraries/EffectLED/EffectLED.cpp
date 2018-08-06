

/*
  EffectLED.cpp - Library for flashing Morse code.
*/

#include "EffectLED.h"

EffectLED::EffectLED(){}

// constructor receive number of leads on strip 
EffectLED::EffectLED(int numLeds, CRGB * leds){

  this->leds = leds;  
  this->setNumLeds(numLeds);
  //this->effectName = "";   
  
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





