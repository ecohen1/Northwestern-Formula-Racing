//Teensy 3.1 Pinout
#define RX1				0 //Serial Receive. OG for Xbee
#define TX1				1 //Serial Transmit. OG for Xbee
#define CAN_TX			3 //CAN HI
#define CAN_RX			4 //CAN LO
#define NEO				5 //NeoPixel Data out
#define CCLK			6 //
#define CCLR			7 //
#define MAIN_SW			8 //
#define RESET			9 //
#define CS_LCD			10//
#define MOSI			11
#define MISO			12
#define SCK				13
#define SW1				14
#define SW2				15
#define SW3				16
#define SW4				17
#define SW5				18
#define UP				19
#define DOWN			20
#define CLUTCH1			A7
#define CLUTCH2			A8
#define PHOTO			A9

//Global defines for convenience
#define TEENSY_VOL		3.3f		//Teensy Logic Voltage
#define CAN_RATE		1000000		//1 Mbit/s
#define CAN_SW		 	0x4F0		//CAN Data from Steering Wheel
#define CAN_TT		 	0x4F1		//CAN Data from Transmission Teensy
#define CAN_DAQ1	 	0x4F2		//CAN Data from DAQ
#define CAN_DAQ2	 	0x4F3		//CAN Data from DAQ
#define CAN1		    0x4F4		//1st MoTeC CAN Packet
#define CAN2		    0x4F5		//2nd MoTeC CAN Packet
#define CAN3		    0x4F6		//3rd MoTeC CAN Packet
#define CAN4		    0x4F7		//4th MoTeC CAN Packet
#define	CAN5			0x4F8		//5th MoTeC CAN Packet
#define RES			    12			//ADC Resolution
#define AVG			    32			//change ADC AVG to 1,2,4,8,16,32
#define BUTTONS		  	6			//Six Panel Buttons
#define SHORT_PRESS		3			//Short press debounce time
#define LONG_PRESS		200			//Long press confirmation time
#define RPM_LEDS		45			//75% of ring dedicated to RPM
#define RING_LEDS		60			//60 Neopixel Ring of LEDs
#define REFRESH			1000		//1000 Hz Data Refresh Rate
#define REFRESH_RING	60			//60  Hz RPM  Refresh Rate
#define REFRESH_LCD		30			//30  Hz LCD  Refresh Rate
#define MAX_BRIGHT		0.3f		//Voltage at Maximum Brightness
#define MIN_CLUTCH1		1.8f		//Minimum voltage for moving clutch (Measure)
#define MAX_CLUTCH1		2.3f		//Voltage for 100% clutch disengagement (Measure)
#define MIN_CLUTCH2		1.33f		//Minimum voltage for moving clutch (Measure)
#define MAX_CLUTCH2		1.8f		//Voltage for 100% clutch disengagement (Measure)
#define CLUTCH_BITE		0.5f		//Percent Engagement for clutch bite
#define MIN_SPEED		10.0f		//10 MPH button lock out
#define OP_MIN			5			//Low Oil Pressure !!! (PSI)
#define ET_MAX			200			//Engine Overheating !!! (degF)
#define CRANK_RPM		450			//Cranking RPM
#define RUNNING_RPM		800			//Minimum RPM for engine to run
#define IDLE			1200		//Engine Idle
#define REDLINE			9000		//Engine Redline
#define MAX_RPM			10000		//You shall not pass

//Global Variables

int  menu = 1; //Which screen is on
bool emergency = 0;
int	 e = 0;  //Emergency debounce
int  e_millis = 500;
bool eFlag  = 0;
bool eFlag1 = 0;
bool eFlag2 = 0;
String emergencyMessage = "";
	
int  l = 0;  //Generic counter

//MoTeC CAN Bus Output
//Use CAN0 output on MoTeC
//Set Data rate to 1 Mbit/s
//Set Refresh rate to 200 Hz (MAX)
//Ensure Data Set matches

// Page 1 There is only one page currently
uint16_t rpm = 0;         	//RPM      
float throtpos = 0;      	//Throttle Position
float manifpres = 0;     	//Manifold Air Pressure
float engine_temp = 0;   	//Engine Temperature
float lambda1 = 0;       	//Lambda
float oil_pressure = 0;  	//Oil Pressure
float gearvolt = 0;      	//Gear Sensor Voltage
float speed_FL = 0;      	//Wheel Speed: Front Left
float speed_FR = 0;      	//Wheel Speed: Front Right
float speed_RL = 0;      	//Wheel Speed: Rear Left
float speed_RR = 0;      	//Wheel Speed: Rear Right
float drive_speed = 0;   	//Average of Rear Wheels
float ground_speed = 0;  	//Fastest of Front Wheels
float battvolt = 0;      	//Battery Voltage
float main_a = 0;        	//Current Draw from PDM (note not from battery, battery can be greater)
float brake_pressure = 0;	//Brake Pressure
//Grab these from Final Drive Document
int shiftPoints[12] = {9000,10000,9000,10000,9000,10000,9000,10000,9000,10000,9000,10000};

//NFR CAN Data

//Transmission Teensy
uint8_t tt_sw = 0x0;
uint8_t tt_trans = 0x0;

//DAQ
float a_x = 0.;
float a_y = 0.;

//UPDATE FOR NEW CAR
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

//DAQ Data Packet
//Packet 1
//Bit 0-15		Ax
//Packet 2
//Bit 0-15		Ay

//Refresh Rate is 200 Hz for all data
Metro refresh = Metro(1000/REFRESH);

//# of Analog Levels
const uint16_t an_lv = pow(2, RES) - 1;

//Active Switches 
byte old_sw_state = 0x0;
byte sw_state = 0x0;
int up_counter[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
int dn_counter[8] = { 0, 0, 0, 0, 0, 0, 0, 0}; 
bool change_screen = 0;
//Switch LEDs
byte old_led_state = 0x0;
byte sw_led_state = 0x0;

//Shifting
uint8_t shift_req = 0;
uint8_t gear = 0;
uint8_t gear_m = 0;
int 	wheel_rpm = 0;
uint8_t gear_req = 0;
bool	gear_reset = 0;
float	clutch_pos = 0;
float	clutch_p1 = 0;
float	clutch_p2 = 0;
float	max_clutch_req	= 0;
uint8_t clutch_pos_req = 0;
bool 	clutch_bite = 0;
bool 	clutch_bite_req = 0;

//Drive Mode
byte old_drive_mode = 0x1;
byte drive_mode = 0;

//LCD
RA8875 tft = RA8875(CS_LCD,RESET);
uint16_t tx, ty;
char temp[100];

//Neopixel Ring
Adafruit_NeoPixel ring = Adafruit_NeoPixel(RING_LEDS, NEO, NEO_GRB + NEO_KHZ800);
byte neo_status = 0x0;
uint32_t status_color[7] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

//Lighting
float bright = 100; //Photosensor
uint8_t backlight = 0;
uint8_t ringlight = 0;

//Global Functions

//CAN Functions
void setupCAN();
void processCAN();
void sendCAN();
//Read Digital Inputs
void processInputs();
//Output to Button LEDs
void setSW_LEDS();
//Check if Shift or Clutch Requests exist
void shift();
//LCD Functions
void setupLCD();
void refreshLCD();
void resetLCD(String message);
void uploadNum(const uint8_t c[][16], int len, int N);
void printNum(float num, int x, int y, int len, int dec);
void printEngine(int x, int y);
void printNum();
//NeoPixel Functions
void setupNeo();
void refreshNeo();
void rainbowCycle(uint8_t wait, uint8_t N);
void RPM_Cycle(uint8_t wait);
uint16_t RGB565(uint32_t rgb);
void bright_adj();
void setupinput();
void setupoutput();
void refreshTime();		// Resets the clock
