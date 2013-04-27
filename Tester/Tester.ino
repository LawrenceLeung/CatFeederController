/*
Really simple tester for the cat feeder.  Runs One feeding every 60 seconds
 */
 
#include <limits.h>

// Device pins
const int motorPin =  1;      // MISO
const int sensorPin = 0;      // MOSI

// Feeding interval
const unsigned long interval = 60000;

// Variables will change:
int motorState = LOW;             // ledState used to set the LED

boolean sensorState= 0;
unsigned long previousMillis;        // will store last time LED was updated


void clock()
{
  sensorState=!sensorState;
  digitalWrite(sensorPin, sensorState);
}

void setup() {
  // set the digital pin as output:
  pinMode(motorPin, OUTPUT);
  pinMode(sensorPin,INPUT_PULLUP);
  
  previousMillis = ULONG_MAX - interval; // run immediately on powerup
  
}

// time diff that handles rollover
unsigned long deltaT(unsigned long latest, unsigned long last){
 if (last>latest){
   return (ULONG_MAX-last) + latest;
 } else {
   return latest-last;
 }
  
}

void loop()
{
  unsigned long currentMillis = millis();
  

  // Reached feeding interval? 
  if(deltaT(currentMillis,previousMillis) > interval) {      
    // save the last time you turned it on
    previousMillis = millis();
    
    motorState = HIGH;
    digitalWrite(motorPin, motorState);
  }
 
  if (motorState == HIGH && digitalRead(sensorPin) == LOW){  
    // wait until sensorPin is back to HIGH so we're past the low point
    while (digitalRead(sensorPin) == LOW){
       delay(10); 
    }
    
    // stop motor
    motorState = LOW;    
    digitalWrite(motorPin, motorState);
  }
  
}

