//Teensy 3.1 Pinout
#define LAUNCH		5
#define TRACTION  6
#define IGN_CUT		7
#define CAN_TX		3
#define CAN_RX		4
#define TX3	        8
#define CLUTCH_BUT      13
#define CLUTCH_POS	A0
#define MMA_POS		A1
#define START		A2
#define GEAR		A3
#define NEUTRAL		A4
#define BAT2_V		A5
#define BAT2_A		A6
#define BAT2_C		A7
#define BAT1_V		A8
#define BAT1_C		A9

#define TEENSY_VOL		3.3f		//Teensy Logic Voltage
#define CAN_RATE		1000000		//1 Mbit/s
#define CAN_SW			0x4F0		//CAN Data from Steering Wheel
#define CAN_TT			0x4F1		//CAN Data from Transmission Teensy
#define CAN_DAQ1		0x4F2		//CAN Data from DAQ
#define CAN_DAQ2		0x4F3		//CAN Data from DAQ
#define CAN1			0x4F4		//1st MoTeC CAN Packet
#define CAN2			0x4F5		//2nd MoTeC CAN Packet
#define CAN3			0x4F6		//3rd MoTeC CAN Packet
#define CAN4			0x4F7		//4th MoTeC CAN Packet
#define RES				12			//ADC Resolution
#define AVG				32			//change ADC AVG to 1,2,4,8,16,32

#define GPS_TOL 0.1  // Voltage range for GPS
#define DT 1
#define ALPHA .8

#define UPSHIFT_POS 0.7
#define DOWNSHIFT_POS -0.7
#define NEUTRAL_POS 0.3
#define CLUTCH_HOME 2.25f //0 position in inches
#define CLUTCH_MIN_POS -.10f  //Max movement reverse, max travel of actuator is 1.25 inches
#define FULL_CLUTCH .75f //clutch fully disengaged with respect to home
#define CLUTCH_BITE 0.46f //clutch biting with respect to home
#define CLUTCH_RETIME 350.f // clutch reengaages 
#define SHORT_PRESS 5 // Threshold time for clutch_bite
#define RES 12
#define AVG 32
#define REFRESH 1 //1 millisecond
#define LED 13

//Transmission Teensy Data Packet
//Byte 1
//Bit 0			ENGINE is ON
//Bit 1			FUEL is ON
//Bit 2			IGN is ON
//Bit 3			LC is ON
//Bit 4-5		Car Mode
//Bit 6			STARTER is ON

//Byte 2
//Bit 0-2		Gear
//Bit 3			Ignoring Gear Request
//Bit 4			Clutch Bite

//Byte 3
//Bit 8-15		Clutch Position

//Steering Wheel Teensy Data Packet
//Byte 1
//Bit 0			Turn ENGINE ON
//Bit 1			Turn FUEL ON
//Bit 2			Turn IGNITION ON
//Bit 3			Turn Launch Control ON
//Bit 4-5       Car Mode    

//Byte 2
//Bit 0-2       Gear Request
//Bit 3			N/A
//Bit 4			Clutch Bite
//Bit 8-15		Clutch Position Request


//Shifting
uint8_t shift_req = 0;
uint8_t gear = 1;
uint8_t gear_req = 0;
bool	gear_reset = 0;
uint8_t clutch_pos_req = 0;
bool clutch_bite = 0;
bool clutch_bite_req = 0;
byte drive_mode = 0;
int up_counter = 0;
int dn_counter = 0;

float gps_up[] = {
  0, 0, 0, 0, 0, 0 };
float gps_down[] = {
  0, 0, 0, 0, 0, 0 };

int clutch_time = 0;
int shift_time = 0;


//MoTeC CAN Bus Output
//Use CAN0 output on MoTeC
//Set Data rate to 1 Mbit/s
//Set Refresh rate to 200 Hz (MAX)
//Ensure Data Set matches

int menu = 1; //Which screen is on
bool emergency = 0;
int l = 0;  //Generic counter

uint16_t rpm = 0; // Page 1
float throtpos = 0;
float manifpres = 0;
float engine_temp = 0;
float lambda1 = 0;
float oil_pressure = 0;
float gearvolt = 0;
float speed_FL = 0;
float speed_FR = 0;
float speed_RL = 0;
float speed_RR = 0;
float drive_speed = 0;
float ground_speed = 0;
float battvolt = 0;
float main_a = 0;

//Transmission Teensy
uint8_t tt_sw = 0x0;
uint8_t tt_trans = 0x0;

struct motor{
  bool ready = false;
  int pot;
  int port;
  int up;
  int down;
  int duty;
  int min_duty;
  int max_duty;
  int timer;
  int timeout;
  float p = 0;
  float i = 0;
  float d = 0;
  float e_int = 0;
  float e_old = 0;
  float e_dot = 0;
  float e = 0;
  float home = 0;
  float pos = 0;
  float go = 0;
  float max_a = 0;
  float max_v = 0;
  float min_pos = 0;
  float max_pos = 0;
  float tol = 0;
} shifter, clutch;


//global vars
int dt = 1;
String str;
int clutchpin = 9;//A0;
int dnshiftpin = 10;//A1;
int upshiftpin = 8;//5;
int dd1 = 100;//clutch open
int dd2 = 250;//shifter out
int dd3 = 50;//shifter to middle
int ud2 = 60;//shifter out
int del = 0;
int CUT_TO_SHIFT = 20;
int SHIFT_TO_ON = 10;
Metro refresh = Metro(REFRESH);

//NFR Libaries
#include "CAN.h"
#include "motor.h"

