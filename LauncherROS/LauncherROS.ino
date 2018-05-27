/* 
 * LauncherROS.ino
 * 2018-05-27
 * Connor Yates
 *
 * Controls the loading mechanism for the throwing portion of the robot.
 * This actuates a servo attached to the loading arm between two preset positions.
 * Each servo control loop moves the servo to a preset position.
 * This is done through a loop to create a smoother motion, as moving directly 
 * to the target position creates jerky behavior.
 * The delay waits 15ms for the servo to reach the desired intermediary position.
 */

#include <Servo.h>
#include <Wire.h>
#include "Adafruit_VCNL4010.h"
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h> //temp

Adafruit_VCNL4010 vcnl;
Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
int back = 15;  // extended position of servo
int forward = 120; // retracted position of servo
int rest = 60;
int readypin = 13; 

ros::NodeHandle  nh;

std_msgs::String str_msg;
char hello[256] = "hello world!";

void setspeed_cb(const std_msgs::UInt16& cmd_msg)
{
	/* TODO switch off the command recieved, and set the pins 
	 * appropriate for that command signal, so the message is
	 * propogated to the motor controller
	 */
	
	Serial.print("Int array recieved: ");
	Serial.println(cmd_msg.data);
}

ros::Subscriber<std_msgs::UInt16> sub("set_speed", setspeed_cb);
ros::Publisher pub("arduino_debug", &str_msg);

void setup() {
	pinMode(readypin, INPUT);
	myservo.attach(9);  // attaches the servo on pin 9 to the servo object
	myservo.write(rest);

	Serial.begin(57600);
	if (! vcnl.begin())
	{
		Serial.println("Sensor not found :(");
		while (1);
	}

	nh.initNode();
	nh.advertise(pub);
	nh.subscribe(sub);
}




void loop() {
	str_msg.data = hello;
	pub.publish( &str_msg );
	nh.spinOnce();
	Serial.print("Proximity Sensor: ");
	Serial.println(vcnl.readProximity());
	if ((vcnl.readProximity() > 4000) && (digitalRead(readypin)==LOW))
	{
		myservo.write(back);
		delay(750);
		myservo.write(forward);
		delay(1000);
		myservo.write(rest);
		//delay(2500);
	}
}
