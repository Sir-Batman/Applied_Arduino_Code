#include <PID_v1.h> 

const int r_sensor = 2;
const int l_sensor = 3;
volatile bool right_sensor_triggered = false; 
volatile bool left_sensor_triggered = false;
double time_trip_r = 0;
double time_trip_l = 0;
double rpm_r = 0;
double rpm_l = 0;
//int threshold = 300;
double setspeed_r = 1000;
double setspeed_l = 1000;
double r_motor_in = 200;
double l_motor_in = 200;
double avgbuffer_r = 0;
int avgcounter_r = 0;
double avgbuffer_l = 0;
int avgcounter_l = 1;
double rpm_out_r = 0;
double rpm_out_l = 0;
int readypin = 13; 
String inString = "";  // string to hold Serial input
int idlespeed = 150;
int diff = 25; // launch speed difference threshold (RPM)

int movmean = 3;

int motorpinr = 9;
int motorpinl = 6;

double Kp = 1.75, Ki = 0.075 ,Kd = 0;

PID r_PID(&rpm_out_r, &r_motor_in, &setspeed_r, Kp, Ki, Kd, DIRECT);
PID l_PID(&rpm_out_l, &l_motor_in, &setspeed_l, Kp, Ki, Kd, DIRECT);

/* TODO write interrupt functions */
void right_sensor_ISR()
{
	right_sensor_triggered = true;
/*
	rpm_r = (1/((millis()-time_trip_r)/1000))*600*6;
	//r_status = true;
	time_trip_r = millis();
	avgbuffer_r += rpm_r;
	avgcounter_r++;
*/
}

void left_sensor_ISR()
{
	left_sensor_triggered = true ;
/*
	rpm_l = (1/((millis()-time_trip_l)/1000))*600*6; 
	//l_status = true;
	time_trip_l = millis();
	avgbuffer_l += rpm_l;
	avgcounter_l++;
*/
}

void setup() {  
	Serial.begin(115200);
	pinMode(motorpinr, OUTPUT);
	pinMode(motorpinl, OUTPUT);
	//pinMode(r_sensor,INPUT);
	//pinMode(l_sensor,INPUT);
	pinMode(readypin, OUTPUT);
	r_PID.SetMode(AUTOMATIC);
	l_PID.SetMode(AUTOMATIC);
	TCCR0B = TCCR0B & B11111000 | B00000001;  // set timer 0 divisor to 1 for PWM frequency of 62500.00 Hz
	TCCR1B = TCCR1B & B11111000 | B00000001;  // set timer 1 divisor to 1 for PWM frequency of 31372.55 Hz

	/* Initialize trip starting time point */
	time_trip_r = millis();
	time_trip_l = millis();
	/* TODO Set up l_sensor and r_sensor as interrupts */
	digitalWrite(l_sensor, HIGH);
	digitalWrite(r_sensor, HIGH);
	attachInterrupt(0, left_sensor_ISR, RISING);
	attachInterrupt(1, right_sensor_ISR, RISING);
	rpm_out_r = 0;
	rpm_out_l = 0;
}

void loop() {
	/* Read serial input to set target speed */
	while (Serial.available())
	{
		int inChar = Serial.read();
		int tempSpeed = 0;
		if (isDigit(inChar))  // convert the incoming byte to a char and add it to the string:
		{
			inString += (char)inChar;
		}
		/* End of user input, set target speed from input */ 
		if (inChar == '\n')
		{
			tempSpeed = inString.toInt();
			setspeed_r = tempSpeed;
			setspeed_l = tempSpeed;
			inString = "";  // clear the string for new input:
		}
	}
	/* BEGIN */
	//int rsread = analogRead(r_sensor);
	//int lsread = analogRead(l_sensor);


	//check if right sensor is seeing white for the first time in a rotation
	if (right_sensor_triggered)
	{
		rpm_r = (1/((millis()-time_trip_r)/1000))*600*6;
		right_sensor_triggered = false;
		time_trip_r = millis();
		avgbuffer_r += rpm_r;
		avgcounter_r++;
	}

	if (left_sensor_triggered)
	{
		rpm_l = (1/((millis()-time_trip_l)/1000))*600*6; 
		left_sensor_triggered = false;
		time_trip_l = millis();
		avgbuffer_l += rpm_l;
		avgcounter_l++;
	}
/*
	//check if right sensor is seeing black for the first time in a rotation
	if ((rsread > threshold) && (r_status == true))
	{
		r_status = false;
	}
	if ((lsread > threshold) && (l_status == true))
	{
		l_status = false;
	}
*/
	/* END */

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
		analogWrite(motorpinr, r_motor_in);
		l_PID.Compute();
		if (l_motor_in > 255)
		{
			l_motor_in = 255;
		}
		if (l_motor_in < idlespeed)
		{
			l_motor_in = idlespeed;
		}
		analogWrite(motorpinl, l_motor_in);

		
		Serial.print(rpm_out_r);
		Serial.print(" ");
		Serial.print(setspeed_r);
		Serial.print(" ");
		Serial.print(r_motor_in);
		Serial.print(" ");
		Serial.print(rpm_out_l);
		Serial.print(" ");
		Serial.print(setspeed_l);
		Serial.print(" ");
		Serial.println(l_motor_in);
		
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
		analogWrite(motorpinl, l_motor_in);
		r_PID.Compute();
		if (r_motor_in > 255)
		{
			r_motor_in = 255;
		}
		if (r_motor_in < idlespeed)
		{
			r_motor_in = idlespeed;
		}
		analogWrite(motorpinl, r_motor_in);
		
		Serial.print(rpm_out_r);
		Serial.print(" ");
		Serial.print(setspeed_r);
		Serial.print(" ");
		Serial.print(r_motor_in);
		Serial.print(" ");
		Serial.print(rpm_out_l);
		Serial.print(" ");
		Serial.print(setspeed_l);
		Serial.print(" ");
		Serial.println(l_motor_in);
		
	}
	/* If the speeds are within the difference tolerance, signal to other arduino
	** that we are ready to fire, by writing to a pin */
	if ((abs(rpm_out_l-setspeed_l) < diff) && (abs(rpm_out_r-setspeed_r) < diff))
	{
		digitalWrite(readypin,HIGH);
		//Serial.println("Ready to launch");
		digitalWrite(readypin,LOW);
	}
}
