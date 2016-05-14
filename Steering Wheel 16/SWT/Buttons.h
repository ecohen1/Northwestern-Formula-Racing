//SW STATE IS A BINARY NUMBER. THE SECOND BIT FROM LEFT IS CLUTCH. THIRD IS TRACTION, FOURTH IS LAUNCH. SIXTH AND SEVENTH ARE PADDLES.

void update_traction(int rot_pos){
  //sw_state &= 0b111000;
  if(rot_pos > 3400 || (rot_pos < 1400 && rot_pos > 800))
    sw_state |= 0b000001;
  else if(rot_pos > 2000 && rot_pos < 3400)
    sw_state |= 0b000010;
  else if(rot_pos > 1400 && rot_pos < 2000)
    sw_state |= 0b000100;
}

void update_launch(int rot_pos){
  //sw_state &= 0b000111;
  if(rot_pos > 3400 || (rot_pos < 1400 && rot_pos > 800))
    sw_state |= 0b001000;
  if(rot_pos > 2000 && rot_pos < 3400)
    sw_state |= 0b010000;
  if(rot_pos > 1400 && rot_pos < 2000)
    sw_state |= 0b100000;
}

//Toggles Switch State
void processInputs() {
	old_sw_state = sw_state;
  int rot_pos = analogRead(ROTARY);
  
  if (rot_pos < 1400 && rot_pos > 800) {
    auto_shift = 1;
  }
  else {
    auto_shift = 0;
  }

  if (sw_state & 0b000111 && rot_pos > 800)
  {
    sw_state &= 0b111000;
    update_traction(rot_pos);
  }
  if (sw_state & 0b111000 && rot_pos > 800)
  {
    sw_state &= 0b000111;
    update_launch(rot_pos);
  }

	//TRACTION switch debouncer
	if (digitalRead(TRACTION)) {
		up_counter[3]++;
		if (up_counter[3] == SHORT_PRESS) 
		{
      if(sw_state & 0b111)
        sw_state &= 0b111000;
      else
        update_traction(analogRead(ROTARY));
		}
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
		if (up_counter[4] == SHORT_PRESS) 
		{
      if(sw_state & 0b111000)
        sw_state &= 0b000111;
      else if(ground_speed > 20)
        sw_state &= 0b000111;
      else
        update_launch(analogRead(ROTARY));
		}
	}
	else if (up_counter[4] > 0) {
		dn_counter[4]++;
		if (dn_counter[4] == SHORT_PRESS) {
			up_counter[4] = 0;
			dn_counter[4] = 0;
		}
	}
  if (auto_shift){
    //auto_shifting for loss of shifting control
    if (gear < 5 && gear != 0 && rpm > 9250 && auto_shift_delay <= 0 && slip < 10 && ground_speed > MIN_SPEED)
    {
      shift_req = 1;
      auto_shift_delay = AUTO_DEBOUNCE;
      //delay(70);
    }
    else if (gear > 1 && rpm < 4000 && auto_shift_delay <= 0 && ground_speed > MIN_SPEED)
    {
      shift_req = 2;
      auto_shift_delay = AUTO_DEBOUNCE;
      //delay(70);
    }
    else
    {
      auto_shift_delay--;
      if (auto_shift_delay < 0)
      {
        auto_shift_delay = 0;
      }
    }
  }
  else {
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
}
//
void setSW_LEDS() {
//	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	//Change to represent what TCU says the actual state is
//	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  if (old_sw_state == sw_state) return;
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
//  digitalWrite(CCLR, LOW);
//  digitalWrite(CCLR, HIGH);
//  for (uint8_t i = 0; i <= sw_led_state; i++) {
//    digitalWrite(CCLK, HIGH);
//    digitalWrite(CCLK, LOW);
//  }
}
