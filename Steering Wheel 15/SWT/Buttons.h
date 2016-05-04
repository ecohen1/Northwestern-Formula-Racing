#define FUEL		SW5
#define IGN			SW4
#define NEUTRAL		SW3
#define LAUNCH		SW2
#define MODE		SW1
#define AUTO_START	MAIN_SW

//Toggles Switch State
void processInputs() {
	old_sw_state = sw_state;
	if (ground_speed < MIN_SPEED) {
		//Auto-start debouncer
		if (digitalRead(AUTO_START)) {
			up_counter[0]++;
			if (up_counter[0] == SHORT_PRESS) sw_state ^= digitalRead(AUTO_START);
		}
		else if (up_counter[0] > 0) {
			dn_counter[0]++;
			if (dn_counter[0] == SHORT_PRESS) {
				up_counter[0] = 0;
				dn_counter[0] = 0;
			}
		}
		//Fuel switch debouncer
		if (digitalRead(FUEL)) {
			up_counter[1]++;
			if (up_counter[1] == SHORT_PRESS) sw_state ^= (digitalRead(FUEL) << 1);
		}
		else if (up_counter[1] > 0) {
			dn_counter[1]++;
			if (dn_counter[1] == SHORT_PRESS) {
				up_counter[1] = 0;
				dn_counter[1] = 0;
			}
		}
		//Ignition switch debouncer
		if (digitalRead(IGN)) {
			up_counter[2]++;
			if (up_counter[2] == SHORT_PRESS) sw_state ^= (digitalRead(IGN) << 2);
		}
		else if (up_counter[2] > 0) {
			dn_counter[2]++;
			if (dn_counter[2] == SHORT_PRESS) {
				up_counter[2] = 0;
				dn_counter[2] = 0;
			}
		}
		//Neutral switch debouncer
		if (digitalRead(NEUTRAL)) {
			up_counter[3]++;
			if (up_counter[3] == SHORT_PRESS) sw_state ^= (digitalRead(NEUTRAL) << 3);
		}
		else if (up_counter[3] > 0) {
			dn_counter[3]++;
			if (dn_counter[3] == SHORT_PRESS) {
				up_counter[3] = 0;
				dn_counter[3] = 0;
			}
		}
                if (gear == 0) sw_state &= 0xFF;
                else sw_state &= 0b11110111; 
		//Launch Control switch debouncer 
		if (digitalRead(LAUNCH)) {
			up_counter[4]++;
			if (up_counter[4] == SHORT_PRESS) sw_state ^= (digitalRead(LAUNCH) << 4);
		}
		else if (up_counter[4] > 0) {
			dn_counter[4]++;
			if (dn_counter[4] == SHORT_PRESS) {
				up_counter[4] = 0;
				dn_counter[4] = 0;
			}
		}
	}
	//SW6 debouncer
	//Long Press	= Mode change
	//Short Press	= Menu change
	if (digitalRead(MODE)) {
		up_counter[5]++;
		if (up_counter[5] == LONG_PRESS) {
			drive_mode++;
			drive_mode %= 3;
			sw_state = (sw_state & 0b00011111) | (drive_mode << 5);
		}
	}
	else if (up_counter[5] > 0) {
		dn_counter[5]++;
		if (dn_counter[5] == SHORT_PRESS) {
			if ((up_counter[5] >= SHORT_PRESS) && (up_counter[5] < LONG_PRESS)) {
				/*menu++;
				menu %= 2;*/
			}
			up_counter[5] = 0;
			dn_counter[5] = 0;
		}
	}
	//Debounce up paddle
	if (digitalRead(UP)) {
		up_counter[6]++;
		if (up_counter[6] == SHORT_PRESS) shift_req = digitalRead(UP);
	}
	else if (up_counter[6] > 0) {
		dn_counter[6]++;
		if (dn_counter[6] == SHORT_PRESS) {
			up_counter[6] = 0;
			dn_counter[6] = 0;
		}
	}
	//Debounce down paddle
	if (digitalRead(DOWN)) {
		up_counter[7]++;
		if (up_counter[7] == SHORT_PRESS) shift_req = (digitalRead(DOWN) << 1);
	}
	else if (up_counter[7] > 0) {
		dn_counter[7]++;
		if (dn_counter[7] == SHORT_PRESS) {
			up_counter[7] = 0;
			dn_counter[7] = 0;
		}
	}
}

void setSW_LEDS() {
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Change to represent what TCU says the actual state is
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (old_sw_state == sw_state) return;
	sw_led_state = sw_state & 0b00000001;							//ENGINE		
	if ((sw_state & 0b00000010) >> 1) sw_led_state |= 0b00100000;	//FUEL
	else sw_led_state &= 0b00011111;
	if ((sw_state & 0b00000100) >> 2) sw_led_state |= 0b00010000;	//IGNITION
	else sw_led_state &= 0b00101111;
	if (gear == 0) sw_led_state |= 0b00001000;						//NEUTRAL
	else sw_led_state &= 0b00110111;
	if ((sw_state & 0b00010000) >> 4) sw_led_state |= 0b00000100;	//LAUNCH
	else sw_led_state &= 0b00111011;
	if (drive_mode == 2) sw_led_state |= 0b00000010;				//MODE
	else sw_led_state &= 0b00111101;
	digitalWrite(CCLR, LOW);
	digitalWrite(CCLR, HIGH);
	for (uint8_t i = 0; i <= sw_led_state; i++) {
		digitalWrite(CCLK, HIGH);
		digitalWrite(CCLK, LOW);
	}
}
