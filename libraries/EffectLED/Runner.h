#ifndef __VAD_RunnerEffectLED_h
#define __VAD_RunnerEffectLED_h

#include "EffectLED.h"
//class EffectLED; 
// constants Runner 
char * const RUNNER_PERIOD = "period" ; 
char * const RUNNER_SIZE = "size" ; 
char * const RUNNER_COLOR = "color" ;
char * const EFFECT_NUMLEDS = "numl" ; // number of leds used for effect
char * const EFFECT_LIGHT = "light" ; // brightness 
// effect Runner 
class Runner : public EffectLED {
	
	public :
		Runner(); 
		Runner (int numLeds, CRGB* leds); 
		~Runner (){}  
		virtual String getName(); 
		virtual String getListConfigParams(); 
		virtual void run();
		String parseCommand (char*  cmd, char* val); 
		void setPeriod(uint32_t);  
		void setSize(uint32_t);
		void setColor(uint32_t);
		void setNumLeds (int numLeds); 
		void setTimeInLoop(int time);
		void setBrightness(uint8_t scale);      /// Set the global brightness scaling   
	
	private : 
		const char* name = "Runner";  	
		uint32_t period; 
		uint32_t size;
		CRGB color; 
		uint8_t brightness; 
		String str_color; 
		int pos = 0; /* Текущее положение ползунка */
  		long last = millis(); 
	 
};

#endif
