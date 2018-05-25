#include <PID_v1.h> 
#include <ros.h> 
#include <std_msgs/UInt16.h> //temp
#include <std_msgs/String.h> 


const int r_sensor = 1;
const int l_sensor = 0;
bool r_status= false; 
bool l_status= false;
double time_trip_r= millis();
double time_trip_l= millis();
double rpm_r = 0;
double rpm_l = 0;
int threshold = 300;
double setspeed_r= 1000;
double setspeed_l= 1000;
double r_motor_in =200;
double l_motor_in =200;
double avgbuffer_r = 0;
int avgcounter_r = 0;
double avgbuffer_l = 0;
int avgcounter_l = 1;
double rpm_out_r = 0;
double rpm_out_l = 0;
int readypin = 13; 
//String inString = "";    // string to hold Serial input
int idlespeed = 150;
int diff = 25; // launch speed difference threshold

int movmean = 3;

int motorpin1 = 9;
int motorpin2 = 10;

double Kp = 2.25, Ki = 0.075 ,Kd =0;

PID r_PID(&rpm_out_r, &r_motor_in, &setspeed_r, Kp, Ki, Kd, DIRECT);
PID l_PID(&rpm_out_l, &l_motor_in, &setspeed_l, Kp, Ki, Kd, DIRECT);

void setspeed_cb(const std_msgs::UInt16& cmd_msg)
{
	if ( (cmd_msg.data >= 0) && (cmd_msg.data <= 3000) )
	{
		setspeed_r = cmd_msg.data;
		setspeed_l = cmd_msg.data;
	}
}

/* Set up ROS subscriber/publishers */
ros::NodeHandle nh;
std_msgs::String str_msg;
ros::Subscriber<std_msgs::UInt16> sub("/arduino/setspeed", setspeed_cb);
ros::Publisher p("/my_topic", &str_msg);
char hello[13] = "hello world!";

void setup()
{
	/* Initialize ROS node */
        //nh.getHardware()->setBaud(115200);
	nh.initNode();
	nh.subscribe(sub);
	nh.advertise(p);
	/* Initialize motor and arduino comunication pins */
	pinMode(motorpin1, OUTPUT);
	pinMode(motorpin2, OUTPUT);
	pinMode(r_sensor,INPUT);
	pinMode(l_sensor,INPUT);
	pinMode(readypin, OUTPUT);
	r_PID.SetMode(AUTOMATIC);
	l_PID.SetMode(AUTOMATIC);

	TCCR0B = TCCR0B & B11111000 | B00000001;    // set timer 0 divisor to 1 for PWM frequency of 62500.00 Hz
	TCCR1B = TCCR1B & B11111000 | B00000001;    // set timer 1 divisor to 1 for PWM frequency of 31372.55 Hz

	rpm_out_r = 0;
	rpm_out_l = 0;
}



void loop()
{
        nh.spinOnce();
        str_msg.data = hello;
        p.publish(&str_msg);

	int rsread = analogRead(r_sensor);
	int lsread = analogRead(l_sensor);

	/* check if right sensor is seeing white for the first time in a rotation */
	if ((rsread < threshold) && (r_status == false))
	{
		rpm_r = (1/((millis()-time_trip_r)/1000))*600*6;
		r_status = true;
		time_trip_r = millis();
		avgbuffer_r += rpm_r;
		avgcounter_r++;
	}

	if ((lsread < threshold) && (l_status == false))
	{
		rpm_l = (1/((millis()-time_trip_l)/1000))*600*6; 
		l_status = true;
		time_trip_l = millis();
		avgbuffer_l += rpm_l;
		avgcounter_l++;
	}
	/* Check if right sensor is seeing black
	 * for the first time in a rotation */
	if ((rsread > threshold) && (r_status == true))
	{
		r_status = false;
	}
	if ((lsread > threshold) && (l_status == true))
	{
		l_status = false;
	}

	if (avgcounter_r == movmean)
	{ 
		rpm_out_r = avgbuffer_r/movmean;
		avgbuffer_r = 0;
		avgcounter_r = 0;

		r_PID.Compute();
		if (r_motor_in > 255)
		{
			r_motor_in = 255;
		}
		if (r_motor_in < idlespeed)
		{
			r_motor_in = idlespeed;
		}
		analogWrite(motorpin1, r_motor_in);
		l_PID.Compute();
		if (l_motor_in > 255)
		{
			l_motor_in = 255;
		}
		if (l_motor_in < idlespeed)
		{
			l_motor_in = idlespeed;
		}
		analogWrite(motorpin2, l_motor_in);
	}
	if (avgcounter_l == movmean)
	{
		rpm_out_l = avgbuffer_l/movmean;
		avgbuffer_l = 0;
		avgcounter_l = 0;
		l_PID.Compute();
		if (l_motor_in > 255)
		{
			l_motor_in = 255;
		}
		if (l_motor_in < idlespeed)
		{
			l_motor_in = idlespeed;
		}
		analogWrite(motorpin2, l_motor_in);
		r_PID.Compute();
		if (r_motor_in > 255)
		{
			r_motor_in = 255;
		}
		if (r_motor_in < idlespeed)
		{
			r_motor_in = idlespeed;
		}
		analogWrite(motorpin1, r_motor_in);
	}

	if (	(abs(rpm_out_l-setspeed_l) < diff)
		&& (abs(rpm_out_r-setspeed_r) < diff))
	{
		digitalWrite(readypin,HIGH);
		digitalWrite(readypin,LOW);
	}
}
