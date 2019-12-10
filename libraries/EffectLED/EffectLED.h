#ifndef __VAD_EffectLED_h
#define __VAD_EffectLED_h

#include "FastLED.h"


class EffectLED 
{
public : 
  
   EffectLED ();
   EffectLED (int numLeds, CRGB* leds,long til); 
   ~EffectLED() {}  
  
  void setNumLeds (int);  
  virtual String getName() = 0;   // virtual function to return name of the effect 
  virtual void run() = 0; 		  // derived class defines whatto do 
  virtual String parseCommand (char*  cmd, char* val)=0; 
  virtual String getListConfigParams()=0; 
  virtual void setTimeInLoop(int tyme)=0; // 
  
protected : 
 	int NUM_LEDS;			// number of leds 
 	CRGB* leds;				// led strip to be controlled 
 	long til; // time in loop 
 	String effectName;
 	
 	void clear_led();		                                    // put leds to black  
 	void trimStr (char* str);
 	
 	void setPixel(int Pixel, byte red, byte green, byte blue);  // set color to one led 
 	void setAll(byte red, byte green, byte blue);               // set color to all leds  
 	void showStrip();                                           // same ass FastLED.show(); 
               
 	
 		
}; 

#endif



