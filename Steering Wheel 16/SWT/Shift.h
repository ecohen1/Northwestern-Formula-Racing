void shift() {
//	//Shifter actuation
//    if (shift_req == 1) gear_req = 1;
//    else if (shift_req == 2) gear_req = 2;   
//    if (gear_reset) gear_req = 0;
//	shift_req = 0;
//	//Clutch Actutation
//	//Convert analog readings into a scaled voltage 0-3.3V
//	clutch_p1 = analogRead(CLUTCH1)*TEENSY_VOL / an_lv;
//	clutch_p2 = analogRead(CLUTCH2)*TEENSY_VOL / an_lv;
//	//Convert clutch position into 0-100% scaling
//	//Note: clutch paddle 1's voltage decreases as it is depressed
//	//While clutch paddle 2's voltage increases as it is depressed
//	clutch_p1 = 100.*(MAX_CLUTCH1-clutch_p1) / (MAX_CLUTCH1 - MIN_CLUTCH1);
//	clutch_p2 = 100.*(clutch_p2 - MIN_CLUTCH2) / (MAX_CLUTCH2 - MIN_CLUTCH2);
//	//Constrain the values so they are definitely between 0 and 100%
//	clutch_p1 = constrain(clutch_p1,0,100);
//	clutch_p2 = constrain(clutch_p2,0,100);
//	//Grab greater of the two paddles
//	max_clutch_req = max(clutch_p1,clutch_p2);
//	//IF	clutch request is greater than 80%
//	//THEN	send a clutch request to TCU
	if (CLUTCH){
	  clutch_pos_req = 255; 
	}
	else {
	  clutch_pos_req = 0;
//    RPM_Cycle(10);
	}
}
