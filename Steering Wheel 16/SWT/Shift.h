void shift() {
//IF	clutch requested
//THEN Button state for clutch pin on
//THEN Send clutch request to TCU
	if (digitalRead(CLUTCH)){
    //sw_state |= (digitalRead(CLUTCH) << 2);
	  clutch_pos_req = 255; 
	}
	else {
	  clutch_pos_req = 0;
    //sw_state &= (digitalRead(CLUTCH) << 2);
//    RPM_Cycle(10);
	}
}
