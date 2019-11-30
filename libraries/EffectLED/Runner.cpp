
/*
  Runner.cpp - Runner Class Effect with cobnfigurable params.
*/

#include "Runner.h"

/*******************************************************
		RUNNER FUNCTIONS 
*******************************************************
*/

// constructor receive number of leads on strip and pass it to supper class 
Runner::Runner(int numLeds, CRGB * leds) : EffectLED(numLeds, leds) 
{
	static uint8_t num = 0; 
	this->color.setRGB(255,255,0); 
	//this-> color.setColorCode (0); 
	this-> size = 1;                // defoult led size 
	this-> period =100;             // defoult period 
	this->effectName = "Runner ";   // Runner effect 
	this->effectName.concat(num++); // create unic name for runner 
	brightness = 50 ;               // default value for brightness  
	str_color = "FFFF00";           // default str_color 
}


// set period of leds on in milessecons 
void Runner::setPeriod(uint32_t p){
	this->period = p; 
}  
// set numbet of used leds 
void Runner::setSize(uint32_t s){
	this->size = s; 
} 

// set color of leds 
void Runner::setColor(uint32_t c){
	this->color.setColorCode (c); 
}

// set number of leds to be controlled 
void Runner::setNumLeds (int numLeds){
	clear_led(); 
	NUM_LEDS = numLeds; 
}     

// set time in loop for this effect 
void Runner::setTimeInLoop(int time){ til = time; } 

/// Set the global brightness scaling
/// @param scale a 0-255 value for how much to scale all leds before writing them out
void Runner::setBrightness(uint8_t scale) { 
    brightness = scale; 
}

// return effect name 
String Runner::getName(){ return this->effectName; }


// not blocking function 
// TODO :: control if changed period or size to allow enter into realisation loop .. 
// color , period to shine , runner size
// @ param : color 
// @ param : period in milliseconds , (-1) long period of time 
void Runner::run() {
  //Serial.println (last); 
  if(millis() - last > this->period) { // Реализация временной задержки
    last = millis();
    clear_led();
    if(pos <= NUM_LEDS - size) { // Реализация 

      for(uint8_t j = 0; j < size; ++j) {
        leds[pos + j] = this->color;
      }
      FastLED.setBrightness(brightness);    // set brightness
      FastLED.show();                       // show effect
      ++pos;
    } 
    else {
      pos = 0;
    }
  }
}

/*
  Parsing the command and associate value received from CLI user 
 */

String Runner::parseCommand (char*  cmd, char* sep) {

String res = String (effectName);
	

    // set parameter on effect 
    if (strcmp(cmd,RUNNER_PERIOD) == 0){      // period  : 100
      setPeriod(atoi(sep)); 
    }
    else if (strcmp(cmd,RUNNER_SIZE) == 0){      // size  : 100
      setSize (atoi(sep)); 
    }
    else if (strcmp(cmd,RUNNER_COLOR) == 0){      // color  : 100
      int length = strlen(sep);
      sep[length-1] = '\0'; 
      str_color = String(sep);                    // save color value in string representation 
      setColor ((uint32_t)strtol(sep, NULL, 16));   
    } 
     else if (strcmp(cmd,EFFECT_NUMLEDS) == 0){      // numl  : 100
      setNumLeds(atoi(sep));   
    }
    else if (strcmp(cmd,EFFECT_LIGHT) == 0){      // light  : 100
      int val = atoi(sep); 
      if (val > 255 || val <0){
        return res+"\t>> Brightness Value out of range "; 
      }
      setBrightness((uint8_t)val);   
    } 
    else {
      return (res+" Undefined Command");
    }
   
  return res+"\t>> "+String(cmd)+" : "+String(sep);
}


// return the list of configurable parameters for this effect 
String Runner::getListConfigParams(){
    String res = String (effectName); 
    res = res + " List of configurable Values:>> \n"; 
    res = res + "\t"+ String(EFFECT_NUMLEDS) + " = " + NUM_LEDS + "\n"; 
    res = res + "\t"+ String(RUNNER_PERIOD) + " = " + period + "\n"; 
    res = res + "\t"+ String(RUNNER_SIZE) + " = " + size + "\n";
    res = res + "\t"+ String(RUNNER_COLOR) + " = " + str_color + "\n"; 
    res = res + "\t"+ String(EFFECT_LIGHT) + " = " + brightness + "\n"; 
    
    return res; 
   
}
 


