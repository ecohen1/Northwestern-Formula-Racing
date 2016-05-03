//Arduino Libraries
//External Dependencies
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <SPI.h>
#include <RA8875.h>
#include <Adafruit_NeoPixel.h>
#include <Metro.h>
#include <Time.h>
#include "global.h"

//NFR Libraries
#include "CAN.h"  
#include "Buttons.h"
#include "Fonts.h"
#include "Colors.h"
#include "LCD.h"
#include "Neo.h"
#include "Shift.h"
#include "Emergency.h"
#include "BootUp.h"

void setup() {
	setupNeo();
	setupLCD();
	setupoutput();
	setupinput();
	setupCAN();
}

void loop() {
	if (refresh.check()) {
		processInputs();
    processCAN();
		bright_adj();	
		check_for_emergency();
		shift();
		if (l % (REFRESH / REFRESH_RING) == 0) 	refreshNeo();
		if (l % (REFRESH / REFRESH_LCD) == 0) 	refreshLCD(1); //if adding page numbers input the correct page num
//		setSW_LEDS();
		sendCAN();	
		
		if (l % 500 == 0) debug(); //Every half second output debug information
		
		//Increase counter and loop over at one second
		l++;
		l %= 1000;
	}
}
void debug() {
	//Serial.println(digitalRead(AUTO_START));
	//Serial.println(digitalRead(MAIN_SW));
	//Serial.println(digitalRead(FUEL));
	//Serial.println(digitalRead(SW5));
	//Serial.println(digitalRead(IGN));
	//Serial.println(digitalRead(SW4));
	//Serial.println(digitalRead(NEUTRAL));
	//Serial.println(digitalRead(SW3));
	//Serial.println(digitalRead(LAUNCH));
	//Serial.println(digitalRead(CLUTCH));
	//Serial.println(digitalRead(MODE));
	//Serial.println(digitalRead(SW1));
	Serial.println(digitalRead(UP));
	Serial.println(digitalRead(DOWN));
	//Serial.println(analogRead(CLUTCH1)*TEENSY_VOL/an_lv);
	//Serial.println(analogRead(CLUTCH2)*TEENSY_VOL/an_lv);
	//Serial.println(clutch_p1);
	//Serial.println(clutch_p2);
	//Serial.println(gear_req);
	//Serial.println(sw_state);
	//Serial.println(sw_led_state);
	//Serial.println(bright);
	/*Serial.print((sw_state & 0b10000000) >> 7);
	Serial.print((sw_state & 0b01000000) >> 6);
	Serial.print((sw_state & 0b00100000) >> 5);
	Serial.print((sw_state & 0b00010000) >> 4);
	Serial.print((sw_state & 0b00001000) >> 3);
	Serial.print((sw_state & 0b00000100) >> 2);
	Serial.print((sw_state & 0b00000010) >> 1);
	Serial.println((sw_state & 0b00000001));
	Serial.print((sw_led_state & 0b10000000) >> 7);
	Serial.print((sw_led_state & 0b01000000) >> 6);
	Serial.print((sw_led_state & 0b00100000) >> 5);
	Serial.print((sw_led_state & 0b00010000) >> 4);
	Serial.print((sw_led_state & 0b00001000) >> 3);
	Serial.print((sw_led_state & 0b00000100) >> 2);
	Serial.print((sw_led_state & 0b00000010) >> 1);
	Serial.println((sw_led_state & 0b00000001));*/
	//Serial.println(clutch_pos);
	//Serial.println(brake_pressure);
    //Serial.println(sw_led_state);
	//Serial.println(timeStatus());
	//Serial.println(drive_mode);
	//Serial.println(gear_req);
	//Serial.println(gear);
	//Serial.println(shift_req);
	//Serial.println(rxmsg.id);
	//Serial.println(rpm);
	//Serial.println(oil_pressure);
	//Serial.println(drive_mode);
	//Serial.println(txmsg.buf[0]);
	//Serial.println(txmsg.buf[1]);
	//Serial.println(txmsg.buf[2]);
	//Serial.println("");
}
