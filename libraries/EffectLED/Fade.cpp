
/*
  Fade.cpp - Fade Class Effect with cobnfigurable params.
*/

#include "Fade.h"

/*******************************************************
		Fade FUNCTIONS 
*******************************************************
*/

// constructor receive number of leads on strip and pass it to supper class 
Fade::Fade(int numLeds, CRGB * leds, long timeInLoop) : EffectLED(numLeds, leds, timeInLoop) 
{
	static uint8_t num = 0;         // static value, unic Fade id 
	this->color.setRGB(255,255,0);  // initial color 
	this-> size = 1;                // defoult led size 
	this-> period =100;             // defoult period 
	this->effectName = "Fade ";   // Fade effect 
	this->effectName.concat(num++); // create unic name for Fade 
	brightness = 50 ;               // default value for brightness  
	str_color = "FFFF00";           // default str_color 
	always_random = false;          // assign random values to param on each loop step? 
	used_leds = NUM_LEDS;           // using all leds by defoult 
}


// set period of leds on in milessecons 
void Fade::setPeriod(uint32_t p){
	this->period = p; 
}  
// set numbet of used leds 
void Fade::setSize(uint32_t s){
	if (s > used_leds) s = used_leds; // control size 
	this->size = s; 
} 

// set color of leds 
void Fade::setColor(uint32_t c){
	this->color.setColorCode (c); 
	//color.setColorCode(CRGB::Black); 
}

// set number of leds to be controlled 
void Fade::setNumLeds (int numLeds){
	clear_led();
	if (numLeds>NUM_LEDS) numLeds = NUM_LEDS; // control size 
	used_leds = numLeds; 
}     

// set time in loop for this effect 
void Fade::setTimeInLoop(int time){ til = time; } 

/// Set the global brightness scaling
/// @param scale a 0-255 value for how much to scale all leds before writing them out
void Fade::setBrightness(uint8_t scale) { 
    brightness = scale; 
}

/// Set the rundom 
/// @param bool scale if change values every spep 
void Fade::setRandom(bool scale){ always_random = scale; }

// return effect name 
String Fade::getName(){ return this->effectName; }


// not blocking function 
// @ param : color 
// @ param : period in milliseconds , (-1) long period of time 
void Fade::run() { 
  
//  if(always_random && (millis() - lastInLoop > til) ){
//       lastInLoop = millis(); 
//       clear_led(); 
//       assignRundomValues(); // assign random values to params 
//   }
//  
  //if(millis() - last > period) { // Реализация временной задержки
  //  last = millis();
    clear_led();
    uint8_t r,g,b; 
    uint8_t red = random(256);
    uint8_t green = random(256);
    uint8_t blue = random(256); 
    int del = random(10);
    brightness = random(25,256) ;   // random value for brightness    
    FastLED.setBrightness(brightness);    // set brightness
    
    // Реализация
    for(int k = 0; k < 256; k=k+1) {
        r = (k/256.0)*red;
        g = (k/256.0)*green;
        b = (k/256.0)*blue;
        setAll(r,g,b);
        showStrip();
        delay(del); 
    }
     
      for(int k = 255; k >= 0; k=k-2) {
        r = (k/256.0)*red;
        g = (k/256.0)*green;
        b = (k/256.0)*blue;
        setAll(r,g,b);
        showStrip();
        delay(del); 
      }
}

/*
    Assign random values to configurable parameters of  Fade effect 
*/
void Fade::assignRundomValues(){
    uint8_t r = random(256);
    uint8_t g = random(256);
    uint8_t b = random(256); 
    color.setRGB(r,g,b);           // set random color  
	//used_leds = random (NUM_LEDS/2,NUM_LEDS); // used leds < total NUM leds
	size = random(1,used_leds);       //  Fade size < used leds
	period =random (20,1000);      // random period 
	brightness = random(25,256) ;   // random value for brightness  
	str_color = "RANDOM";          
}


/*
  Parsing the command and associate value received from CLI user 
 */

String Fade::parseCommand (char*  cmd, char* sep) {

String res = String (effectName);	
    // set parameter on effect 
    if (strcmp(cmd,Fade_PERIOD) == 0){      // period  : 100
      setPeriod(atoi(sep)); 
    }
    else if (strcmp(cmd,Fade_SIZE) == 0){      // size  : 100
      setSize (atoi(sep)); 
    }
    else if (strcmp(cmd,Fade_COLOR) == 0){      // color  : 100
      int length = strlen(sep);
      sep[length-1] = '\0'; 
      str_color = String(sep);                    // save color value in string representation 
      setColor ((uint32_t)strtol(sep, NULL, 16));   
    } 
    else if (strcmp(cmd,Fade_NUMLEDS) == 0){      // numl  : 100
      setNumLeds(atoi(sep));   
    }
    else if (strcmp(cmd,Fade_ALWAYS_RANDOM) == 0){      // arand  : 1
      always_random = atoi(sep); // 0 cast to false , other true  
    }
    else if (strcmp(cmd,Fade_RANDOM) == 0){      // rand  : 
      //always_random = atoi(sep); // 0 cast to false , other true 
      assignRundomValues();  
    }
    
    else if (strcmp(cmd,Fade_LIGHT) == 0){      // light  : 100
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


// return the list of configurable parameters for this Fade 
String Fade::getListConfigParams(){
    String res = String (effectName); 
    res = res + " List of configurable Values:>> \n"; 
    res = res + "\t"+ String(Fade_NUMLEDS) + " = " + used_leds + "\n"; 
    res = res + "\t"+ String(Fade_PERIOD) + " = " + period + "\n"; 
    res = res + "\t"+ String(Fade_SIZE) + " = " + size + "\n";
    res = res + "\t"+ String(Fade_COLOR) + " = " + str_color + "\n"; 
    res = res + "\t"+ String(Fade_LIGHT) + " = " + brightness + "\n"; 
    
    return res; 
   
}
 


