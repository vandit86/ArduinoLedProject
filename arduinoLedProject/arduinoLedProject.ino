

#include "FastLED.h"
#include "EffectLED.h"
#include "Runner.h"

// How many leds in your strip?
#define NUM_LEDS 31

// GND to common GND 
#define DATA_PIN 5    // SI 
#define CLOCK_PIN 7   // CLK

#define BRIGHTNESS  50

// command max size  
#define CMD_SIZE 127       // max size of command from CLI 
#define DEL ':'            // delimiter character on command 
#define LIST_SIZE 10       // effect list size 

// buffers 
CRGB leds[NUM_LEDS];                                        // LED Array    
// Get next command from Serial (add 1 for final 0)
char inputCmd[CMD_SIZE + 1]={ 0 };                           // byffer to read command from CLI 


// constants Runner 
char * const EFFECT_NEW = "new" ;       // new effect (example new:  -> create new Runner effect )
char * const EFFECT_LIST = "list" ;     // new effect (example list:  -> show list of effects )
char * const EFFECT_DEL = "del" ;       // new effect (example del:1  -> delete first effect on list)
char * const EFFECT_SET = "set";
char * const EFFECT_END = "end";

EffectLED* effList [LIST_SIZE] = {  NULL };      // list of effects  

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 5000;           // interval at which to blink (milliseconds)

uint8_t exePos = 0;                  // execution position on array 
EffectLED* runner,*runnerSet = 0;  // effect to be executed on this step and configured

//EffectLED eLed for tests
//Runner runner_0 (NUM_LEDS, leds); 
//Runner runner_1 (NUM_LEDS, leds); 

void setup() { 

  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RBG>(leds, NUM_LEDS);
  //FastLED.clearData();  


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

    //  create new effect 
    if (strcmp(cmd,EFFECT_NEW) == 0){     
      EffectLED* effect =  new Runner (NUM_LEDS, leds); 
      insertEffect( effect );       // add new runner effect to the list of effects 
    }
    else if (strcmp(cmd,EFFECT_SET) == 0){      // start of configuration
      uint32_t pos = (uint32_t)atoi(sep); 
      if (pos < LIST_SIZE)
        runnerSet = effList[pos]; 
        Serial.print ("Configuring : " );
        Serial.println (runnerSet->getName()); 
        Serial.println (runnerSet->getListConfigParams().c_str()); // show list of configurable params of effect  
    }
    else if (strcmp(cmd,EFFECT_END) == 0){      // end of configuration 
      Serial.println ("SET is Done");
      runnerSet = NULL; 
    }

    else if (strcmp(cmd,EFFECT_LIST) == 0){      // size  : 100
      listEffects(); 
    }
    else if (strcmp(cmd,EFFECT_DEL) == 0){      // color  : 100
      deleteEffect(atoi(sep)) ;   
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


// insert new effect into array , find the empty position insert effect 
void insertEffect (EffectLED* eff){
  for (int i = 0; i < LIST_SIZE; i++){
    if ( !effList[i]){
      effList[i] = eff;
      Serial.print ("New Effect : "); 
      Serial.print ( eff->getName()); 
      Serial.print (", inserted into position : "); 
      Serial.println(i); 
      return;  
    }  
  }
  Serial.println("error to insert new effect: no more space for new effects"); 
}


// print all running effects effects 
void listEffects (){
  int j =0; 
  Serial.println("All running effects effects : "); 
  for (int i = 0; i < LIST_SIZE; i++){
    if ( effList[i]){
      Serial.print("[");
      Serial.print(i);  
      Serial.print("]");
      Serial.print("\t");
      Serial.println (effList[i]->getName().c_str());
      ++j; 
    }  
  }

  if (j == 0) Serial.println("List of effects is empty "); 
}

// realize del effect 
void deleteEffect (int num){
  if (num<0 || num >= LIST_SIZE){
    Serial.print (num); 
    Serial.println ("\t is out of range");  
  }
  else if (effList[num] == NULL){
    Serial.print (num); 
    Serial.println ("\t effect is not exists");
  }
  else if (effList[num] != NULL){
    delete effList[num];              // release memmory alocated for this effect 
    effList[num] = NULL;              // release the position on effect list array 
    Serial.print (num); 
    Serial.println ("\t is delated"); 
  }
  return ;  
}
