//CAN Setup
FlexCAN CANbus(CAN_RATE);
CAN_message_t txmsg, rxmsg;
CAN_filter_t motec_mask = { 0, 0, 0x7F0 };  //Checks first 7 digits of ID
CAN_filter_t filters = { 0, 0, 0 };
uint32_t filter[8] = {CAN_TT, CAN_DAQ1, CAN_DAQ2, CAN1, CAN2, CAN3, CAN4, CAN5};
int swcounter = 0;

void setupCAN() {
	CANbus.begin(motec_mask);
	for (uint8_t i = 0; i<8; i++) {
		filters.id = filter[i];
		CANbus.setFilter(filters, i);
	}
	txmsg.len = 8;
	txmsg.id = CAN_SW;
	txmsg.timeout = 2;
}

void processCAN() {
	while (CANbus.read(rxmsg)) {
		if (rxmsg.id == CAN_TT) {
			tt_sw = rxmsg.buf[0];
			tt_trans = rxmsg.buf[1];
            //gear = (tt_trans & 0b111);
            gear_reset = (tt_trans & 0b1000) >> 3;
	        clutch_bite = (tt_trans & 0b10000) >> 4;
			clutch_pos = 100*rxmsg.buf[2]/255.;
		}
		if (rxmsg.id == CAN_DAQ1) {
			//brake_pressure = rxmsg.buf[0];
		}
		if (rxmsg.id == CAN_DAQ2) {

		}
		//MoTeC sends 16 bit integers as two bytes in reverse order
		//So we have to bit shift the lower byte by 8 (1 byte)
		//And OR it with the upper byte to recover the 16 bit integer.
		//You then have to scale this integer by whatever the MoTeC
		//CAN document says
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
			gearvolt = .001*((rxmsg.buf[4] << 8) | rxmsg.buf[5]);
			gear = ((rxmsg.buf[6] << 8) | rxmsg.buf[7]);
		}
		if (rxmsg.id == CAN3) {
			speed_FL = .1*((rxmsg.buf[0] << 8) | rxmsg.buf[1]);
			speed_FR = .1*((rxmsg.buf[2] << 8) | rxmsg.buf[3]);
			speed_RL = .1*((rxmsg.buf[4] << 8) | rxmsg.buf[5]);
			speed_RR = .1*((rxmsg.buf[6] << 8) | rxmsg.buf[7]);
			ground_speed = max(speed_FL, speed_FR);
			drive_speed = (speed_RL + speed_RR) / 2.;
      slip = ((drive_speed - ground_speed)/ground_speed)*100;
		}
		if (rxmsg.id == CAN4) {
			battvolt = .01*((rxmsg.buf[0] << 8) | rxmsg.buf[1]);
			main_a = .1*((rxmsg.buf[2] << 8) | rxmsg.buf[3]);
      brake_pressure = .1*((rxmsg.buf[4] << 8) | rxmsg.buf[5]);
      if (brake_pressure > 1000) brake_pressure = 0;
      brake_pressure = constrain(brake_pressure,0,500);
		}
	}
}

void sendCAN() {
	//Steering Wheel Data Packet
	//Packet 1
	//Bit 0			ENGINE is ON
	//Bit 1			FUEL is ON
	//Bit 2			TC is ON
	//Bit 3			LC is ON
	//Bit 4-5		Car Mode
	//Bit 6			Neutral

	//Packet 2
	//Bit 0-2		Gear
	//Bit 3			Clutch Bite
	//Bit 8-15		Clutch Position
  txmsg.id = CAN_SW;
  txmsg.buf[0] = sw_state;
  txmsg.buf[1] = (gear_req & 0b1111) | (clutch_bite_req << 4);
  txmsg.buf[2] = clutch_pos_req;
	CANbus.write(txmsg);
  swcounter++;
}
