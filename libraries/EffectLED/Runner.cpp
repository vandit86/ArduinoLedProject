

/*
  EffectLED.cpp - Library for flashing Morse code.
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
	this-> size = 0; 
	this-> period =-1;  
	this->effectName = "Runner ";
	this->effectName.concat(num++);   
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

// return effect name 
String Runner::getName(){
	return this->effectName; 
}


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
      FastLED.show();

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
      setColor ((uint32_t)strtol(sep, NULL, 16));   
    } 

    else {
      return (res+" Undefined Command");
    }
   
  return res+"\t>> "+String(cmd)+" : "+String(sep);
}


