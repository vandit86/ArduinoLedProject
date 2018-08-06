#ifndef __VAD_EffectLED_h
#define __VAD_EffectLED_h

#include "FastLED.h"

class EffectLED 
{
public : 
  
   EffectLED ();
   EffectLED (int numLeds, CRGB* leds); 
   ~EffectLED() {}  
  
  void setNumLeds (int);  
  virtual String getName() = 0;   // virtual function to return name of the effect 
  virtual void run() = 0; 		  // derived class defines whatto do 
  virtual String parseCommand (char*  cmd, char* val)=0; 
  
protected : 
 	int NUM_LEDS;			// number of leds 
 	CRGB* leds;				// led strip to be controlled 
 	void clear_led();		// put leds to black 
 	String effectName; 
 	void trimStr (char* str);
 		
}; 

#endif



