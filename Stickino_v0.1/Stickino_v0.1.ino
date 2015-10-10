#include "Maxbotix.h"

// costn definitions
#define DEBUG 1 // 1 turn on debug / 0 to turn off debug
#define VIBE_PIN 9 // vibe output PIN
#define PROBE_PIN 8 // sonar wave sensor PIN
#define PROBE_RANGE 150 // relevant distance for the sensor ( in cm )

// feedback calc. parameters
#

// init the Maxbotix to listen on the probe pin
Maxbotix rangeSensorPW(PROBE_PIN, Maxbotix::PW, Maxbotix::LV);

void setup() {
  
  // setup the vibe pin in output mode
  pinMode(VIBE_PIN, OUTPUT);

  // init the seril only in debug mode
  #ifdef DEBUG
    Serial.begin(9600);
  #endif

}

void loop() {
  
  unsigned long start; // loop start varibale
  int distance; // distance read form the sensor (in cm)

  
  // update loop start time
  start = millis();
  // read distance from sensor
  distance = int(rangeSensorPW.getRange());
  sendVibeFeed(distance);

}


boolean sendingViber = false;
unsigned long startViber = 0;
unsigned long endViber = 0;
int durationViber = 0;
int durationPause = 0;


/*
 * send a feedback to the  user based on the obstacle distance
 * no feed wiil be  sent for distance > 150 cm
 */
void sendVibeFeed(int distanceCm){
  #ifdef DEBUG
    Serial.print("\t\t<<\t - INPUT distance - \t>> ");
    Serial.println(distanceCm);
  #endif 
  // if ther is an obstacle in range
  if (distanceCm < 150){
    // check id the feedback has been sent
    if(sendingViber == false){
      
      if(distanceCm < 50){
        durationPause = (int(distanceCm / 5) + 1) * 25;
      }else if(distanceCm > 50 && distanceCm < 70 ){
        durationPause = (int(distanceCm / 5) + 1) * 45;
      }else{
        durationPause = (int(distanceCm / 5) + 1) * 65;  
      }
    }else{
      //durationViber = 500;
      if(distanceCm < 50){
        durationViber = (int(distanceCm / 5) + 1) * 50;
      }else if(distanceCm > 50 && distanceCm < 70 ){
        durationViber = (int(distanceCm / 5) + 1) * 60;
      }else{
        durationViber = 400;//(int(distanceCm / 5) + 1) * 55;  
      }
    }
    if(sendingViber == false && (millis() - endViber) > durationPause){
      // if not
      // set up the state 
      endViber = 0;
      startViber = millis();
      digitalWrite(VIBE_PIN, HIGH);
      sendingViber = true;
       // evey 5cm the viber duration goes up of 25ms
      durationPause = 0;

      #ifdef DEBUG
        Serial.print("\t\t<<\t - Start viber for - \t>> ");
        Serial.println(durationViber);
      #endif
      
    }else if(sendingViber == true){
      // if the feedback has been sent
      // check if the feed must be terminated
      if(millis() - startViber > durationViber){
          endViber = millis();
          startViber = 0;
          digitalWrite(VIBE_PIN, LOW);
          sendingViber = false;
          durationViber = 0;
          #ifdef DEBUG
            Serial.print("\t\t<<\t - Stop viber for - \t>> "); Serial.println(durationPause);
          #endif
      }
    }    
  }else{
    // if no obstacle in range reset the state
    digitalWrite(VIBE_PIN, LOW);
    sendingViber = false;
    startViber = 0;
    endViber = 0;
    durationViber = 0;
    durationPause = 0;
    #ifdef DEBUG
      Serial.println("\t\t<<\t - Stop viber no obstavle- \t>> ");  
    #endif
  }
}



