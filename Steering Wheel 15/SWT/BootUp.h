void setupinput() {
	//Digital Inputs
	pinMode(SW1, INPUT);
	pinMode(SW2, INPUT);
	pinMode(SW3, INPUT);
	pinMode(SW4, INPUT);
	pinMode(SW5, INPUT);
	pinMode(MAIN_SW, INPUT);
	pinMode(UP, INPUT);
	pinMode(DOWN, INPUT);
	
	//Analog Setup
	analogReadResolution(RES);
	analogReadAveraging(AVG);
	analogReference(0);
	processInputs();
}

void setupoutput() {
	pinMode(NEO, OUTPUT);
	digitalWrite(NEO, LOW);
	pinMode(CCLK, OUTPUT);
	digitalWrite(CCLK, LOW);
	digitalWrite(CCLK, HIGH);
	pinMode(CCLR, OUTPUT);
	digitalWrite(CCLR, LOW);
	digitalWrite(CCLR, HIGH);
	for (int i = 0; i <= 257; i++) {
		digitalWrite(CCLK, HIGH);
		digitalWrite(CCLK, LOW);
	}
}

void bright_adj() {
	//bright = 255.*analogRead(PHOTO)/an_lv;
    //bright = constrain(bright,20,255);
	backlight = 255;
	ringlight = max(.4*bright,.1*255);
	tft.PWMout(1,backlight);
	ring.setBrightness(ringlight);
}
