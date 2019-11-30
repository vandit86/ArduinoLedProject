
/*
  Runner.cpp - Runner Class Effect with cobnfigurable params.
*/

#include "Runner.h"

/*******************************************************
		RUNNER FUNCTIONS 
*******************************************************
*/

// constructor receive number of leads on strip and pass it to supper class 
Runner::Runner(int numLeds, CRGB * leds, long timeInLoop) : EffectLED(numLeds, leds, timeInLoop) 
{
	static uint8_t num = 0;         // static value, unic Runner id 
	this->color.setRGB(255,255,0);  // initial color 
	this-> size = 1;                // defoult led size 
	this-> period =100;             // defoult period 
	this->effectName = "Runner ";   // Runner effect 
	this->effectName.concat(num++); // create unic name for runner 
	brightness = 50 ;               // default value for brightness  
	str_color = "FFFF00";           // default str_color 
	always_random = false;          // assign random values to param on each loop step? 
	used_leds = NUM_LEDS;           // using all leds by defoult 
}


// set period of leds on in milessecons 
void Runner::setPeriod(uint32_t p){
	this->period = p; 
}  
// set numbet of used leds 
void Runner::setSize(uint32_t s){
	if (s > used_leds) s = used_leds; // control size 
	this->size = s; 
} 

// set color of leds 
void Runner::setColor(uint32_t c){
	this->color.setColorCode (c); 
	//color.setColorCode(CRGB::Black); 
}

// set number of leds to be controlled 
void Runner::setNumLeds (int numLeds){
	clear_led();
	if (numLeds>NUM_LEDS) numLeds = NUM_LEDS; // control size 
	used_leds = numLeds; 
}     

// set time in loop for this effect 
void Runner::setTimeInLoop(int time){ til = time; } 

/// Set the global brightness scaling
/// @param scale a 0-255 value for how much to scale all leds before writing them out
void Runner::setBrightness(uint8_t scale) { 
    brightness = scale; 
}

/// Set the rundom 
/// @param bool scale if change values every spep 
void Runner::setRandom(bool scale){ always_random = scale; }

// return effect name 
String Runner::getName(){ return this->effectName; }


// not blocking function 
// TODO :: control if changed period or size to allow enter into realisation loop .. 
// color , period to shine , runner size
// @ param : color 
// @ param : period in milliseconds , (-1) long period of time 
void Runner::run() { 
  
  if(always_random && (millis() - lastInLoop > til) ){
       lastInLoop = millis(); 
       clear_led(); 
       assignRundomValues(); // assign random values to params 
   }
  
  if(millis() - last > period) { // Реализация временной задержки
    last = millis();
    clear_led();
    FastLED.setBrightness(brightness);    // set brightness
    if(pos <= used_leds - size) { // Реализация 
      for(uint8_t j = 0; j < size; ++j) {
        leds[pos + j] = color;
      }
      FastLED.show();                       // show effect
      ++pos;
    } 
    else {
      pos = 0;
    }
  }
}

/*
    Assign random values to configurable parameters of  Runner effect 
*/
void Runner::assignRundomValues(){
    uint8_t r = random(256);
    uint8_t g = random(256);
    uint8_t b = random(256); 
    color.setRGB(r,g,b);           // set random color  
	//used_leds = random (NUM_LEDS/2,NUM_LEDS); // used leds < total NUM leds
	size = random(1,used_leds);       //  runner size < used leds
	period =random (20,1000);      // random period 
	brightness = random(25,256) ;   // random value for brightness  
	str_color = "RANDOM";          
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
    else if (strcmp(cmd,EFFECT_ALWAYS_RANDOM) == 0){      // arand  : 1
      always_random = atoi(sep); // 0 cast to false , other true  
    }
    else if (strcmp(cmd,EFFECT_RANDOM) == 0){      // rand  : 
      //always_random = atoi(sep); // 0 cast to false , other true 
      assignRundomValues();  
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
    res = res + "\t"+ String(EFFECT_NUMLEDS) + " = " + used_leds + "\n"; 
    res = res + "\t"+ String(RUNNER_PERIOD) + " = " + period + "\n"; 
    res = res + "\t"+ String(RUNNER_SIZE) + " = " + size + "\n";
    res = res + "\t"+ String(RUNNER_COLOR) + " = " + str_color + "\n"; 
    res = res + "\t"+ String(EFFECT_LIGHT) + " = " + brightness + "\n"; 
    
    return res; 
   
}
 


