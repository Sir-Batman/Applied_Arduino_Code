#include <PID_v1.h>

const int r_sensor=1;
const int l_sensor=0;
bool r_status= false; 
bool l_status= false;
double time_trip_r= millis();
double time_trip_l= millis();
double rpm_r=0;
double rpm_l=0;
int threshold=300;
double setspeed_r= 1000;
double setspeed_l= 1000;
double r_motor_in =200;
double l_motor_in =200;
double avgbuffer_r=0;
int avgcounter_r=0;
double avgbuffer_l=0;
int avgcounter_l=1;
double rpm_out_r=0;
double rpm_out_l=0;
int readypin = 13; 
String inString = "";    // string to hold Serial input
int idlespeed=50;
int maxspeed = 230;
int diff = 25; // launch speed difference threshold

int movmean=10;

int motorpinr=9;
int motorpinl=10;

int IN_COMM_P1 = 2;
int IN_COMM_P2 = 3;
int IN_COMM_P3 = 4;
int P1_VAL = 0;
int P2_VAL = 0;
int P3_VAL = 0;


double Kp=1, Ki=0.001 ,Kd =0;

PID r_PID(&rpm_out_r, &r_motor_in, &setspeed_r, Kp, Ki, Kd, DIRECT);
PID l_PID(&rpm_out_l, &l_motor_in, &setspeed_l, Kp, Ki, Kd, DIRECT);

void setup() {  
	Serial.begin(115200);
	pinMode(motorpinr, OUTPUT);
	pinMode(motorpinl, OUTPUT);
	pinMode(r_sensor,INPUT);
	pinMode(l_sensor,INPUT);
	r_PID.SetMode(AUTOMATIC);
	l_PID.SetMode(AUTOMATIC);
	TCCR0B = TCCR0B & B11111000 | B00000001;    // set timer 0 divisor to     1 for PWM frequency of 62500.00 Hz
	TCCR1B = TCCR1B & B11111000 | B00000001;    // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz
	TCCR2B = TCCR1B & B11111000 | B00000001;    // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz
	//setPwmFrequency(motorpinr, 1);
	//setPwmFrequency(motorpinl, 1);

	rpm_out_r=0;
	rpm_out_l=0;
	/* Set up basic binary communication pins */
	pinMode(readypin, OUTPUT);
	pinMode(IN_COMM_P1, INPUT);
	pinMode(IN_COMM_P2, INPUT);
	pinMode(IN_COMM_P3, INPUT);
}

void loop()
{
	/* Read the desired speed from the input pins */
	P1_VAL = digitalRead(IN_COMM_P1);
	P2_VAL = digitalRead(IN_COMM_P2);
	P3_VAL = digitalRead(IN_COMM_P3);
	/* P2 P1
	    0  0 -- zone 1, soft throw
	    0  1 -- zone 2, medium throw
	    1  0 -- zone 3, hard throw
	    1  1 -- competitive throw
	*/
	if (P2_VAL == LOW && P1_VAL == LOW)
	{
		setspeed_r = 1200;
		setspeed_l = 1200;
	}
	else if (P2_VAL == LOW && P1_VAL == HIGH)
	{
		setspeed_r = 1600;
		setspeed_l = 1600;
	}
	else if (P2_VAL == HIGH && P1_VAL == LOW)
	{
		setspeed_r = 1900;
		setspeed_l = 1900;
	}
	else (P2_VAL == HIGH && P1_VAL == HIGH)
	{
		setspeed_r = 2200;
		setspeed_l = 2200;
	}
		
	// Read serial input from serial terminal (TODO NOT WORKING)
	while (Serial.available()) {
		int inChar = Serial.read();
		int tempSpeed = 0;
		if (isDigit(inChar))  // convert the incoming byte to a char and add it to the string:
		{
			inString += (char)inChar;
		}

		// if you get a newline, print the string, then the string's value:
		if (inChar == '\n')
		{
			tempSpeed = inString.toInt();
			setspeed_r = tempSpeed;
			setspeed_l = tempSpeed;
			//Serial.print("You entered:");
			//Serial.println(val);
			inString = "";  // clear the string for new input:
		}
	}

	int rsread=analogRead(r_sensor);
	int lsread=analogRead(l_sensor);

	//check if right sensor is seeing white for the first time in a rotation
	if ((rsread<threshold) && (r_status==false))
	{
		rpm_r=(1/((millis()-time_trip_r)/1000))*600*6;
		r_status=true;
		time_trip_r=millis();
		avgbuffer_r+=rpm_r;
		avgcounter_r++;
	}
	/* Check left sensor is seeing white for first time in rotation */
	if ((lsread < threshold) && (l_status == false))
	{
		rpm_l=(1/((millis()-time_trip_l)/1000))*600*6; 
		l_status=true;
		time_trip_l=millis();
		avgbuffer_l+=rpm_l;
		avgcounter_l++;
	}
	/* Check if right sensor is seeing black for the first time in a rotation */
	if ((rsread > threshold) && (r_status==true))
	{
		r_status=false;
	}
	/* Check if left sensor is seeing black for the first time in a rotation */
	if ((lsread > threshold) && (l_status==true))
	{
		l_status=false;
	}

	if (avgcounter_r == movmean || avgcounter_l == movmean)
	{ 
		if (avgcounter_r == movmean)
		{
			rpm_out_r = avgbuffer_r/movmean;
			avgbuffer_r = 0;
			avgcounter_r = 0;
		}
		if (avgcounter_l == movmean)
		{
			rpm_out_l = avgbuffer_l/movmean;
			avgbuffer_l = 0;
			avgcounter_l = 0;
		}
		r_PID.Compute();
		if (r_motor_in > maxspeed)
		{
			r_motor_in = maxspeed;
		}
		if (r_motor_in < idlespeed)
		{
			r_motor_in = idlespeed;
		}
		analogWrite(motorpinr, r_motor_in);
		l_PID.Compute();
		if (l_motor_in > maxspeed)
		{
			l_motor_in = maxspeed;
		}
		if (l_motor_in < idlespeed)
		{
			l_motor_in = idlespeed;
		}
		analogWrite(motorpinl, l_motor_in);

		Serial.print("L RPM Measured ");
		Serial.print(rpm_out_l);
		Serial.print("L RPM Desired ");
		Serial.print(setspeed_l);
		Serial.print("L PWM OUT ");
		Serial.print(l_motor_in);
		Serial.print("R RPM Measured ");
		Serial.print(rpm_out_r);
		Serial.print("R RPM Desired ");
		Serial.print(setspeed_r);
		Serial.print("R PWM OUT ");
		Serial.println(r_motor_in);
	}

	if((abs(rpm_out_l - setspeed_l) < diff) && (abs(rpm_out_r - setspeed_r) < diff))
	{
		digitalWrite(readypin,HIGH);
		Serial.println("Ready to launch");
		digitalWrite(readypin,LOW);
	}
}
