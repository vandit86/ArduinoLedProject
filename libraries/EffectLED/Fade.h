#ifndef __VAD_FadeEffectLED_h
#define __VAD_FadeEffectLED_h

#include "EffectLED.h"
//class EffectLED; 
// constants Fade 
char * const Fade_PERIOD = "period" ; 
char * const Fade_SIZE = "size" ; 
char * const Fade_COLOR = "color" ;
char * const Fade_NUMLEDS = "numl" ; // number of leds used for Fade
char * const Fade_LIGHT = "light" ; // brightness
char * const Fade_ALWAYS_RANDOM = "arand" ; //  (0;1)
char * const Fade_RANDOM = "rand" ; // 
// Fade Fade 
class Fade : public EffectLED {
	
	public :
		Fade(); 
		Fade (int numLeds, CRGB* leds, long til); 
		~Fade (){}  
		virtual String getName(); 
		virtual String getListConfigParams(); 
		virtual void run();
		String parseCommand (char*  cmd, char* val); 
		void setPeriod(uint32_t);  
		void setSize(uint32_t);
		void setColor(uint32_t);
		void setNumLeds (int numLeds); 
		void setTimeInLoop(int time);
		void setBrightness(uint8_t scale);  // Set the global brightness scaling 
		void setRandom(bool random);         // set random indicator value that assign random values to configurable class params
		void assignRundomValues();          // function to assign random values to configurable class params  
	
	private : 
		const char* name = "Fade";  	
		uint32_t period; 
		uint32_t size;
		CRGB color; 
		uint8_t brightness; 
		String str_color; 
		int pos = 0; /* Текущее положение ползунка */
  		long last = millis(); 
  		long lastInLoop = millis(); 
  		bool always_random;     // change params values for each step 
  		int used_leds; 
  		//extern HTMLColorCode code; 
  		
	 
};

#endif
