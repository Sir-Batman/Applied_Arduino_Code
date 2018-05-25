/* Sweep
   by BARRAGAN <http://barraganstudio.com>
   This example code is in the public domain.

   modified 8 Nov 2013
   by Scott Fitzgerald
http://www.arduino.cc/en/Tutorial/Sweep
 */

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int forward = 0;
int back = 35;

void setup() {
	myservo.attach(9);  // attaches the servo on pin 9 to the servo object
	int start = myservo.read();
	if(start < forward) {
		for (pos = start; pos <= forward; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
			myservo.write(pos);              // tell servo to go to position in variable 'pos'
			delay(50);                       // waits 15ms for the servo to reach the position
		}
	}
	else if(start > forward) {
		for (pos = start; pos >= forward; pos -= 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
			myservo.write(pos);              // tell servo to go to position in variable 'pos'
			delay(50);                       // waits 15ms for the servo to reach the position
		}
	}
}

void loop() {
	for (pos = forward; pos <= back; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
		myservo.write(pos);              // tell servo to go to position in variable 'pos'
		delay(50);                       // waits 15ms for the servo to reach the position
	}

	delay(2000);

	for (pos = back; pos >= forward; pos -= 1) { // goes from 180 degrees to 0 degrees
		myservo.write(pos);              // tell servo to go to position in variable 'pos'
		delay(50);                       // waits 15ms for the servo to reach the position
	}

	delay(1000);

}

