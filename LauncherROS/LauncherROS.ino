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

#define COMM_P1 11 //D5
#define COMM_P2 12 // D6
//#define COMM_P3 12 // D7
bool FLAG_P1 = false; 
bool FLAG_P2 = false; 
//bool FLAG_P3 = true; 

ros::NodeHandle  nh;

std_msgs::String str_msg;
char hello[25] = "hello world!";

void setspeed_cb(const std_msgs::UInt16& cmd_msg)
{
	switch (cmd_msg.data)
	{
		case 0: 
			FLAG_P1 = false;
			FLAG_P2 = false;
			//FLAG_P3 = false;
			break;
		case 1: 
			FLAG_P1 = true;
			FLAG_P2 = false;
			//FLAG_P3 = false;
			break;
		case 2: 
			FLAG_P1 = false;
			FLAG_P2 = true;
			//FLAG_P3 = false;
			break;
		case 3: 
			FLAG_P1 = true;
			FLAG_P2 = true;
			//FLAG_P3 = false;
			break;
		default:
			FLAG_P1 = false;
			FLAG_P2 = false;
			//FLAG_P3 = false;
			break;
	}
}

ros::Subscriber<std_msgs::UInt16> sub("set_speed", setspeed_cb);
ros::Publisher pub("arduino_debug", &str_msg);

void setup()
{
	pinMode(COMM_P1, OUTPUT);
	pinMode(COMM_P2, OUTPUT);
	//pinMode(COMM_P3, OUTPUT);
	pinMode(readypin, INPUT);
	myservo.attach(9);  // attaches the servo on pin 9 to the servo object
	myservo.write(rest);

	Serial.begin(115200);
	if (! vcnl.begin())
	{
		//Serial.println("Sensor not found :(");
		while (1);
	}

	nh.initNode();
	nh.advertise(pub);
	nh.subscribe(sub);
}

void set_comm_pins()
{
	if (FLAG_P1)
	{
		digitalWrite(COMM_P1, HIGH);
	}
	else
	{
		digitalWrite(COMM_P1, LOW);
	}
	if (FLAG_P2)
	{
		digitalWrite(COMM_P2, HIGH);
	}
	else
	{
		digitalWrite(COMM_P2, LOW);
	}
/*
	if (FLAG_P3)
	{
		digitalWrite(COMM_P3, HIGH);
	}
	else
	{
		digitalWrite(COMM_P3, LOW);
	}
*/
}

void loop() {
	str_msg.data = hello;
	pub.publish( &str_msg );
	nh.spinOnce();
	//Serial.print("Proximity Sensor: ");
	//Serial.println(vcnl.readProximity());
	/* Send flywheel command signal */
	set_comm_pins();
	nh.spinOnce();
	delay(100);
	if ((vcnl.readProximity() > 4000)
		&& (digitalRead(readypin) == LOW))
	{
		nh.spinOnce();
		myservo.write(back);
		delay(750);
		nh.spinOnce();
		myservo.write(forward);
		delay(1000);
		nh.spinOnce();
		myservo.write(rest);
		//delay(2500);
	}
}
