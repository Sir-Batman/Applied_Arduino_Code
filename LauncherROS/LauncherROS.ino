#include <Servo.h>
#include <Wire.h>
#include "Adafruit_VCNL4010.h"

Adafruit_VCNL4010 vcnl;
Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
int back = 15;  // extended position of servo
int forward = 120; // retracted position of servo
int rest = 60;
int readypin = 13; 

void setup() {
  pinMode(readypin, INPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  Serial.begin(115200);
  if (! vcnl.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(rest);
  
}




void loop() {
  Serial.println(vcnl.readProximity());
  if((vcnl.readProximity() > 4000) && (digitalRead(readypin)==LOW)) {
    myservo.write(back);
    delay(750);
    myservo.write(forward);
    delay(1000);
    myservo.write(rest);
    //delay(2500);
  }
}

