//CAN Setup
FlexCAN CANbus(CAN_RATE);
CAN_message_t txmsg, rxmsg;
CAN_filter_t motec_mask = { 0, 0, 0x7F0 };  //Checks first 7 digits of ID
CAN_filter_t filters = { 0, 0, 0 };
uint32_t filter[8] = { 0x4F1, 0x4F2, 0x4F3, 0x4F4, 0x4F5, 0x4F6, 0x4F7, 0x4F8 };

void setupCAN() {
	CANbus.begin(motec_mask);
	for (uint8_t i = 0; i<8; i++) {
		filters.id = filter[i];
		CANbus.setFilter(filters, i);
	}
	txmsg.len = 8;
	txmsg.id = CAN_TT;
	txmsg.timeout = 2;
}

void processCAN() {
	while (CANbus.read(rxmsg)) {
		if (rxmsg.id == CAN_SW) {
			tt_sw = rxmsg.buf[0];

      if (tt_sw&(1<<traction_offset))
      {
        digitalWrite(TRACTION,HIGH);
      }
      else
      {
        digitalWrite(TRACTION,LOW);
      }
      if (tt_sw&(1<<launch_offset))
      {
        digitalWrite(LAUNCH,HIGH);
      }
      else
      {
        digitalWrite(LAUNCH,LOW);
      }

      //richard is the coolest boy in school ;)
      
			tt_trans = rxmsg.buf[1];
      gear_req = (tt_trans & 0b111);
			clutch_pos_req = rxmsg.buf[2];
      if (clutch_pos_req > 200){
        digitalWrite(13,HIGH);
      } else {
        digitalWrite(13,LOW);
      }
                        
		}

		if (rxmsg.id == CAN_DAQ1) {

		}
		if (rxmsg.id == CAN_DAQ2) {

		}
		if (rxmsg.id == CAN1) {
			rpm = (rxmsg.buf[0] << 8) | rxmsg.buf[1];
			throtpos = .1*((rxmsg.buf[2] << 8) | rxmsg.buf[3]);
			manifpres = .1*((rxmsg.buf[4] << 8) | rxmsg.buf[5]);
			engine_temp = .1*((rxmsg.buf[6] << 8) | rxmsg.buf[7]);
		}
		if (rxmsg.id == CAN2) {
			lambda1 = .001*((rxmsg.buf[0] << 8) | rxmsg.buf[1]);
			oil_pressure = 0.1*((rxmsg.buf[2] << 8) | rxmsg.buf[3]);
			if (oil_pressure > 200) oil_pressure = 0.;
			//gearvolt = .001*((rxmsg.buf[4] << 8) | rxmsg.buf[5]);
			//gear = ((rxmsg.buf[6] << 8) | rxmsg.buf[7]);
		}
		if (rxmsg.id == CAN3) {
			speed_FL = .1*((rxmsg.buf[0] << 8) | rxmsg.buf[1]);
			speed_FR = .1*((rxmsg.buf[2] << 8) | rxmsg.buf[3]);
			speed_RL = .1*((rxmsg.buf[4] << 8) | rxmsg.buf[5]);
			speed_RR = .1*((rxmsg.buf[6] << 8) | rxmsg.buf[7]);
			ground_speed = max(speed_FL, speed_FR);
			drive_speed = (speed_RL + speed_RR) / 2.;
		}
		if (rxmsg.id == CAN4) {
			battvolt = .01*((rxmsg.buf[0] << 8) | rxmsg.buf[1]);
			main_a = .1*((rxmsg.buf[2] << 8) | rxmsg.buf[3]);
		}
 
	}
}

void sendCAN() {
	//Steering Wheel Data Packet
	//Packet 1
	//Bit 0			ENGINE is ON
	//Bit 1			FUEL is ON
	//Bit 2			IGN is ON
	//Bit 3			LC is ON
	//Bit 4-5		Car Mode
	//Bit 6			Neutral

	//Packet 2
	//Bit 0-2		Gear
	//Bit 3			Clutch Bite
	//Bit 8-15		Clutch Position
	//txmsg.buf[0] = sw_state;
  uint8_t temp = constrain(255.*clutch.pos/1.05,0,255);
	txmsg.buf[1] = gear;
  txmsg.buf[1] = (txmsg.buf[1] & 0b111) | (gear_reset << 3);
	txmsg.buf[1] = (txmsg.buf[1] & 0b1111) | (clutch_bite << 4);
	txmsg.buf[2] = temp;
	CANbus.write(txmsg);
}

