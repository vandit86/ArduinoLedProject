#ifndef __VAD_RunnerEffectLED_h
#define __VAD_RunnerEffectLED_h

#include "EffectLED.h"
//class EffectLED; 
// constants Runner 
char * const RUNNER_PERIOD = "period" ; 
char * const RUNNER_SIZE = "size" ; 
char * const RUNNER_COLOR = "color" ; 
// effect Runner 
class Runner : public EffectLED {
	
	public :
		Runner(); 
		Runner (int numLeds, CRGB* leds); 
		~Runner (){}  
		virtual String getName(); 
		virtual void run();
		String parseCommand (char*  cmd); 
		void setPeriod(uint32_t);  
		void setSize(uint32_t);
		void setColor(uint32_t);   
	
	private : 
		const char* name = "Runner";  	
		uint32_t period; 
		uint32_t size;
		CRGB color; 
		
		 int pos = 0; /* Текущее положение ползунка */
  		 long last = millis(); 
	 
};

#endif
