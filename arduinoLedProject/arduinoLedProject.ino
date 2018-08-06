#include "FastLED.h"
#include "EffectLED.h"
#include "Runner.h"

// How many leds in your strip?
#define NUM_LEDS 31

#define DATA_PIN 5
#define CLOCK_PIN 7

#define BRIGHTNESS  50

// command max size  
#define CMD_SIZE 127       // max size of command from CLI 
#define DEL ':'            // delimiter character on command 
#define LIST_SIZE 10       // effect list size 

// buffers 
CRGB leds[NUM_LEDS];                                        // LED Array    
// Get next command from Serial (add 1 for final 0)
char inputCmd[CMD_SIZE + 1]={
  0  };                           // byffer to read command from CLI 


// constants Runner 
char * const EFFECT_NEW = "new" ;       // new effect 
char * const EFFECT_LIST = "list" ; 
char * const EFFECT_DEL = "del" ;
char * const EFFECT_SET = "set";
char * const EFFECT_END = "end";

EffectLED* effList [LIST_SIZE] = {
  NULL};      // list of effects  

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 5000;           // interval at which to blink (milliseconds)

uint8_t exePos = 0;                  // execution position on array 
EffectLED* runner,*runnerSet = 0;  // effect to be executed on this step and configured

//EffectLED eLed 
//Runner runner_0 (NUM_LEDS, leds); 
//Runner runner_1 (NUM_LEDS, leds); 

void setup() { 
  // Uncomment/edit one of the following lines for your leds arrangement.
  // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);

  // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<P9813, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<DOTSTAR, RGB>(leds, NUM_LEDS);


  //  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  //FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RBG>(leds, NUM_LEDS);   /// TODO :: controller ; RBG ; data pin ; clockpin ... pass to constructor of effect class 

  // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);



  FastLED.setBrightness(  BRIGHTNESS );


  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RBG>(leds, NUM_LEDS); 


  // serial communication 
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Ready To Receive Command's.");  // prints hello with ending line break 

  //effList[0] = &runner_0;
  //effList[1] = &runner_1;  

}

void loop() {

  if (exePos >= LIST_SIZE) exePos = 0; 
  unsigned long currentMillis = millis();

  // search the next effect to be executed (increment pos)
  if (currentMillis - previousMillis >= interval || runner == NULL) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (effList[exePos]){ 
      runner = effList[exePos];
    }
    else {
      runner = NULL;  
    }
    exePos++; 
  }

  if (runner) {
    runner->run();
    //Serial.println(runner->getName().c_str());
  }


  if(Serial.available())
  {
    //выполнить, если имеются данные
    //int x=Serial.available();  // bytes number , max 64 buff 
    //String inputCmd = Serial.readString();

    byte size = Serial.readBytes(inputCmd, CMD_SIZE);
    // Add the final 0 to end the C string
    inputCmd[size] = 0;
    //if (runnerSet)
    //  Serial.println(runnerSet->parseCommand(inputCmd).c_str()); //  // parsing command
    parseCommand(inputCmd); 

  }

}

/*
  Parsing the command and associate value received from CLI user 
 */

void parseCommand (char*  cmd) {

  char* sep = strchr(cmd,':');      // pointer to separator 
  if (sep){
    *sep = '\0';                    // end string , devide two parts original string 
    ++sep;

    trimStr (cmd); // Remove Spaces From CMD
    trimStr (sep); // Remove Spaces From value

    // set parameter on effect 
    if (strcmp(cmd,EFFECT_NEW) == 0){     
      int pos = insertEffect( new Runner (NUM_LEDS, leds));       // add new runner effect to the list of effects 

        if (pos <0) Serial.println("error to insert new effect");
      else  Serial.println(effList[pos]->getName()); 

    }
    else if (strcmp(cmd,EFFECT_SET) == 0){      // start of configuration
      uint32_t pos = (uint32_t)atoi(sep); 
      if (pos < LIST_SIZE)
        runnerSet = effList[pos]; 
    }
    else if (strcmp(cmd,EFFECT_END) == 0){      // end of configuration 
      Serial.println ("SET is Done");
      runnerSet = NULL; 
    }

    else if (strcmp(cmd,EFFECT_LIST) == 0){      // size  : 100
      listEffects(); 
    }
    else if (strcmp(cmd,EFFECT_DEL) == 0){      // color  : 100
      //setColor ((uint32_t)strtol(sep, NULL, 16));   
    }
    else if (runnerSet) {
        Serial.println(runnerSet->parseCommand(cmd,sep).c_str()); //  // parsing command
    } 

    else {
      Serial.println ("Undefined Command");
    }

  }
  else {
    Serial.println ("Error enter command");
  }

}


// Auxiliary function 
// Remove Spaces From String
void trimStr (char* str){
  int i,j=0;
  for(i=0;str[i]!='\0';++i)
  {
    if(str[i]!=' ')
      str[j++]=str[i];
  }
  str[j]='\0';
}


// insert new effect into array , find the empty position insert effect and return position or -1 if array is full  
int insertEffect (EffectLED* eff){
  for (int i = 0; i < LIST_SIZE; i++){
    if ( !effList[i]){
      effList[i] = eff;
      return i;  
    }  
  }
  return -1 ; // if no more space on array 
}


// list all effects 
void listEffects (){
  int j =0; 
  for (int i = 0; i < LIST_SIZE; i++){
    if ( effList[i]){
      ++j; 
      Serial.println (effList[i]->getName().c_str()); 
    }  
  }

  if (j == 0) Serial.println("empty list "); 
}


























