void setupinput() {
	//Digital Inputs
	pinMode(TRACTION, INPUT);
	pinMode(LAUNCH, INPUT);
	pinMode(UP, INPUT);
	pinMode(DOWN, INPUT);
  pinMode(CLUTCH, INPUT);
	
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
  bright=255;
  bright = constrain(bright,20,255);
	backlight = bright;
	ringlight = 50;
	tft.PWMout(1,backlight);
	ring.setBrightness(ringlight);
}
