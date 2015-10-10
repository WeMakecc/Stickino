#include "Maxbotix.h"

// costn definitions
#define DEBUG 1 // 1 turn on debug / 0 to turn off debug
#define VIBE_PIN 9 // vibe output PIN
#define PROBE_PIN 8 // sonar wave sensor PIN
#define PROBE_RANGE 150 // relevant distance for the sensor ( in cm )

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
int durationViber = 0;

/*
 * send a feedback to the  user based on the obstacle distance
 * no feed wiil be  sent for distance > 150 cm
 */
void sendVibeFeed(int distanceCm){
  #ifdef DEBUG
    Serial.print("\t\t<<\t - INPUT distance - \t>> ");
    Serial.print(distanceCm);
  #endif 
  // if ther is an obstacle in range
  if (distanceCm < 150){
    // check id the feedback has been sent
    if(sendingViber == false){
      // if not
      // set up the state
      startViber = millis();
      digitalWrite(VIBE_PIN, HIGH);
      sendingViber = true;
      durationViber = (int(distanceCm / 5) + 1) * 25;
    }else if(sendingViber == true){
      // if the feedback has been sent
      // check if the feed must be terminated
      if(millis() - startViber < durationViber){
        digitalWrite(VIBE_PIN, LOW);
        sendingViber = false;
      }
    }    
  }else{
    // if no obstacle in range reset the state
    digitalWrite(VIBE_PIN, LOW);
    sendingViber = false;
  }
}



