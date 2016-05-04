#define TRACTION		SW3
#define LAUNCH		SW2

//SW STATE IS A BINARY NUMBER. THE SECOND BIT FROM LEFT IS CLUTCH. THIRD IS TRACTION, FOURTH IS LAUNCH. SIXTH AND SEVENTH ARE PADDLES.

//Toggles Switch State
void processInputs() {
	old_sw_state = sw_state;
	if (ground_speed < MIN_SPEED) {

		//TRACTION switch debouncer
		if (digitalRead(TRACTION)) {
			up_counter[3]++;
			if (up_counter[3] == SHORT_PRESS) sw_state ^= (digitalRead(TRACTION) << 3);
		}
		else if (up_counter[3] > 0) {
			dn_counter[3]++;
			if (dn_counter[3] == SHORT_PRESS) {
				up_counter[3] = 0;
				dn_counter[3] = 0;
			}
		}
   
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
//
void setSW_LEDS() {
//	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	//Change to represent what TCU says the actual state is
//	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if (old_sw_state == sw_state) return;
//	sw_led_state = sw_state & 0b00000001;							//ENGINE		
//	if ((sw_state & 0b00000010) >> 1) sw_led_state |= 0b00100000;	//FUEL
//	else sw_led_state &= 0b00011111;
//	if ((sw_state & 0b00000100) >> 2) sw_led_state |= 0b00010000;	//IGNITION
//	else sw_led_state &= 0b00101111;
//	if (gear == 0) sw_led_state |= 0b00001000;						//TRACTION
//	else sw_led_state &= 0b00110111;
//	if ((sw_state & 0b00010000) >> 4) sw_led_state |= 0b00000100;	//LAUNCH
//	else sw_led_state &= 0b00111011;
//	if (drive_mode == 2) sw_led_state |= 0b00000010;				//MODE
//	else sw_led_state &= 0b00111101;
  digitalWrite(CCLR, LOW);
  digitalWrite(CCLR, HIGH);
  for (uint8_t i = 0; i <= sw_led_state; i++) {
    digitalWrite(CCLK, HIGH);
    digitalWrite(CCLK, LOW);
  }
}
