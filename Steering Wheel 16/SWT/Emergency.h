

void check_for_emergency()	{
	if (engine_temp > ET_MAX){
		emergency = 1;
		emergencyMessage = "ENGINE OVERHEAT!";
	} else if (oil_pressure < OP_MIN && rpm > IDLE){
		emergency = 1;
		emergencyMessage = "OIL PRESS LOW!";
	} else if (rpm > REDLINE){
		emergency = 1;
		emergencyMessage = "ENGINE REDLINE!";
	} else {
		emergency = 0;
		emergencyMessage = "";
	}
	//Debounce Emegency signals so screen does not 
	//rapidly change colors
	if (emergency) e++;
	else e--;
	constrain(e,0,1000);
	//If an emergency condition is true for 600 ms
	//Then trigger the emergency Flag
	//Note 200 ms of hysteresis
	if (e > 600) eFlag = 1;
	if (e < 400) eFlag = 0;
	
	//Check that the emergency has lasted long enough and 
	if (eFlag && !eFlag1){
		back_color 	= RGB565(RED); 				//Background Color
		front_color = RGB565(MILK_WHITE); 		//Foreground Color
		line_color 	= RGB565(SMOKEY_GRAY); 		//Line Color	
		text_color 	= RGB565(BLACK_CAT); 		//Text color
        resetLCD(emergencyMessage);
		eFlag1 = 1;	//Latch eFlag1 so that screen does not reset again
		eFlag2 = 0;	//Unlatch eFlag2 so that when emergency goes away screen can change to normal
	}
	if (!eFlag && !eFlag2){
		back_color	= RGB565(0x121212); 			//Background Color
		front_color = RGB565(MILK_WHITE); 		//Foreground Color
		line_color 	= RGB565(0x565051); 	//Line Color	
		text_color 	= RGB565(0x36D986); 	//Text color
		resetLCD(emergencyMessage);
		eFlag1 = 0; //Unlatch eFlag1 so that when emergency triggers screen can change to emergency
		eFlag2 = 1; //Latch eFlag2 so that screen does not reset again
	}
}
