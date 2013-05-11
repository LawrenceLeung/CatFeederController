/*
Cat feeder timer.  Runs One feeding every 8 hours or 3x a day
 */
 
#include <limits.h>

// Device pins
const int motorPin =  1;      // MISO
const int sensorPin = 0;      // MOSI
const int pushbuttonPin = 2;   // SCK

// Feeding interval
const unsigned long interval = 8*60*60000;

// Variables will change:
int motorState = LOW;             // ledState used to set the LED

boolean sensorState= 0;
unsigned long previousMillis;        // will store last time LED was updated


void setup() {  
  pinMode(motorPin, OUTPUT);
  pinMode(sensorPin,INPUT_PULLUP);
  pinMode(pushbuttonPin,INPUT_PULLUP);
  
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

// debounced read of the sensor pin.  Read it twice 100ms apart until it's stable
boolean readPin(int pin){
  
  boolean value;
  boolean value2=digitalRead(pin);

  do {  
    value=value2;
    delay(100); 
    value2=digitalRead(pin);
  } while (value!=value2);
        
  return value;
}

void loop()
{
  unsigned long currentMillis = millis();

  if (motorState == LOW) {
    // Reached feeding interval or Push button down
    if ((deltaT(currentMillis,previousMillis) > interval) ||
       readPin(pushbuttonPin) == LOW) {      
      
      // save the last time you turned it on if we're triggered by the timer
      if (digitalRead(pushbuttonPin) == HIGH) {
        previousMillis = millis();    
      }
      motorState = HIGH;
      digitalWrite(motorPin, motorState);
    }
  } else {  
    // motor is on and stop sense isn't engaged.  Run until the next click
    if (readPin(sensorPin) == LOW){  
      // wait until sensorPin is back to HIGH so we're past the low point
      do {
         delay(50); 
      } while (readPin(sensorPin) == LOW);
      
      // stop motor
      motorState = LOW;    
      digitalWrite(motorPin, motorState);
    }
  }
  
}

