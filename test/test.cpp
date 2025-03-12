#include <Arduino.h>

int button = 1; // Declaration of the sensor input pin
int value; // Temporary variable
  
void setup () {
  pinMode(button, INPUT); // Initialization sensor pin
  digitalWrite(button, HIGH); // Activation of internal pull-up resistor
  Serial.begin(1152000); // Initialization of the serial monitor
  Serial.println("KY-004 Button test\n");
}
  
void loop () {
  // The current signal at the sensor is read out.
  value = digitalRead(button); 
  // If a signal could be detected, this is displayed on the serial monitor.
  if (value == LOW) {
    Serial.println("Signal detected");
    delay(300); // 300 ms break
    }
}
