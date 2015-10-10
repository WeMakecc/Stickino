/*
  Melody

 Plays a melody

 circuit:
 * 8-ohm speaker on digital pin 8

 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Tone

 */
#include "pitches.h"
#include "Maxbotix.h"

#define DEBUG 1

#define VIBE 9     // attach Vibe motor to pin n°
#define VIBELENGHT 1000     // 
#define VIBEPAUSE 200     // 

#define SUONA_PIN 6

Maxbotix rangeSensorPW(8, Maxbotix::PW, Maxbotix::LV);

int distancePrevious;  

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  // iterate over the notes of the melody:
  /*
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(6, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(6);
  }
  */

  pinMode(VIBE, OUTPUT);
  
  Serial.begin(9600);
  
}



void suona(int distance){

const int triggerFar     = 120;
  const int triggerBetween =  70;
  const int triggerNear    =  30;

  int beep=0;
  
  int durata = 0;
    if (distance < triggerFar){
      durata = 2000;
      beep = NOTE_A2;
      if(distance < triggerBetween){
        durata = 1000;
        beep = NOTE_DS5;
        if (distance < triggerNear){
          durata = 500;
          beep = NOTE_DS8;
        }
      }
    tone(6,beep,durata);

    }
    delay(2000);
  
}

void vibra(int distanza){
  #ifdef DEBUG
    Serial.print("\t\t<<\t - VIBRA2 - \t>> freq. - ");
    Serial.print(distanza);
  #endif 
  if (distanza < 100){
          int beep = NOTE_DS5;
          digitalWrite(VIBE, HIGH);   // turn the LED on (HIGH is the voltage level)
          tone(6,beep);
          delay( (int(distanza / 10) + 1) * 100);
          digitalWrite(VIBE, LOW);    // turn the LED off by making the voltage LOW
          noTone(6);
          delay( (int(distanza / 10) + 1) * 100);
          
  }
}



void loop()
{
  
  unsigned long start;

  int distance;

  
  // PW
  start = millis();
  distance = int(rangeSensorPW.getRange()); //modula();

  int suonaBool = 1;
  int vibraBool = 1;


  
  
  // //  Centimeters
  
#ifdef DEBUG
  Serial.print("Reading...\t");  
  Serial.print("PW: ");
  Serial.print(distance);
  Serial.print(" - ");
  Serial.print(distancePrevious);
  Serial.print("cm - ");
  Serial.print(millis() - start);
  Serial.print("ms");
#endif 

  if(suonaBool == 1){
    //suona(distance); 
   } 
   if(vibraBool == 1){
    vibra(distance); 
   }    
 
  distancePrevious = distance;
  
#ifdef DEBUG
  Serial.println(" ");
#endif 
 
//  delay(100);
}
