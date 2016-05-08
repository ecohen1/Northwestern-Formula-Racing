void shift() {
//IF	clutch requested
//THEN Button state for clutch pin on
//THEN Send clutch request to TCU
  //Shifter actuation
  if (shift_req == 1) gear_req = 1;
  else if (shift_req == 2) gear_req = 2;   
  if (gear_reset) gear_req = 0;
  shift_req = 0;
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
