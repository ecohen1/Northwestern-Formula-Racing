//CAN Setup
FlexCAN CANbus(CAN_RATE);
CAN_message_t txmsg, rxmsg;
CAN_filter_t motec_mask = { 0, 0, 0x7F0 };  //Checks first 7 digits of ID
CAN_filter_t filters = { 0, 0, 0 };
uint32_t filter[8] = {CAN_TT, CAN_DAQ1, CAN_DAQ2, CAN1, CAN2, CAN3, CAN4, CAN5};

void setupCAN() {
	CANbus.begin(motec_mask);
	for (uint8_t i = 0; i<8; i++) {
		filters.id = filter[i];
		CANbus.setFilter(filters, i);
	}
	txmsg.len = 8;
	txmsg.id = CAN_DAQ1;
	txmsg.timeout = 2;
}

void processCAN() {
	while (CANbus.read(rxmsg)) {
		if (rxmsg.id == CAN_TT) {
		}
		if (rxmsg.id == CAN_DAQ2) {
		}
		//MoTeC sends 16 bit integers as two bytes in reverse order
		//So we have to bit shift the lower byte by 8 (1 byte)
		//And OR it with the upper byte to recover the 16 bit integer.
		//You then have to scale this integer by whatever the MoTeC
		//CAN document says
		if (rxmsg.id == CAN_SW) {
      clutch_pos_req = rxmsg.buf[2];
    }
		if (rxmsg.id == CAN_SW2) {
			brake_pressure = rxmsg.buf[0];
		}
    if (rxmsg.id == CAN1) {
		}
		if (rxmsg.id == CAN2) {
		}
		if (rxmsg.id == CAN3) {
		}
		if (rxmsg.id == CAN4) {
            // brake_pressure = .1*((rxmsg.buf[4] << 8) | rxmsg.buf[5]);
            // brake_pressure = constrain(brake_pressure,0,500);
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
	txmsg.buf[0] = daqcounter % 1000;
//	txmsg.buf[1] = (gear_req & 0b1111) | (clutch_bite_req << 4);
//	txmsg.buf[2] = clutch_pos_req;
	CANbus.write(txmsg);
	daqcounter++;
}
