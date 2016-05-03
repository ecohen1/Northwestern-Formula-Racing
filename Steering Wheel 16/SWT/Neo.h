void setupNeo() {
	ring.begin();
    ring.setBrightness(40);
	//Intro options
	//Rainbow Cycle will loop a nice rainbow around the whole ring
	//The inputs are the speed of rotation (lower delay means greater speed), and the number of revolutions
	//rainbowCycle(10, 2); //Two Second Demo
	//RPM Cycle will do what a lot of cars do an rev the ring to redline and back
	rainbowCycle(22,1);
  NU_Cycle(27);
}

void refreshNeo() {

	for (int i = 0; i < RING_LEDS; i++){
		if (((i < rpm*RPM_LEDS / MAX_RPM) && (i < 17)) || (gear && (rpm >= shiftPoints[2*(gear-1)] && rpm <= shiftPoints[2*gear-1]))) {         
			if (gear == 0) ring.setPixelColor(i, BLUE);
			else {
                if (rpm >= REDLINE) ring.setPixelColor(i, RED);
                else {
                    if (i < 8) ring.setPixelColor(i, GREEN + ((i * 3) << 16));
			              else if (i < 14) ring.setPixelColor(i, YELLOW - (((i - 5)*11)<<8 ));                                  
                    if (rpm >= shiftPoints[2*(gear-1)] && rpm <= shiftPoints[2*gear-1]) ring.setPixelColor(i, BLUE);
					          else if (i >= shiftPoints[2*(gear-1)]*RPM_LEDS/MAX_RPM && i <= shiftPoints[2*gear-1]*RPM_LEDS/MAX_RPM) ring.setPixelColor(i, BLUE);
                }
			}
		}
		else ring.setPixelColor(i, 0x0);
	}
	ring.show();
}

void rainbowCycle(uint8_t wait, uint8_t N) {
	for (int j = 0; j<=RING_LEDS*N; j++) { // N cycles of rpm colors on wheel
		for (int i = 0; i< RING_LEDS; i++) {
			if (i < 4) ring.setPixelColor((i+j)%RING_LEDS, BLUE + ((i * 23) << 9));
			else if (i < 8) ring.setPixelColor((i+j)%RING_LEDS, CYAN - ((i - 11) * 23));
			else if (i < 12) ring.setPixelColor((i+j)%RING_LEDS, GREEN + (((i - 22) * 23) << 17));
			else if (i < 17) ring.setPixelColor((i+j)%RING_LEDS, GREEN - (((i - 33) * 23) << 9));
			else ring.setPixelColor((i+j)%RING_LEDS, NORTHWESTERN);
		}
		ring.show();
		delay(wait);
	}
}

void NU_Cycle(uint8_t wait) {
  for(int i =0; i < RPM_LEDS-2; i++)
  {
    delay(1);
    ring.setPixelColor(i, NORTHWESTERN);
    ring.setPixelColor(i+1, NORTHWESTERN);
    ring.setPixelColor(i+2, NORTHWESTERN);
    ring.show();
    delay(wait);
    ring.setPixelColor(i, 0x0); 
    ring.show();
  }
  for(int i = RPM_LEDS-2; i >= 0; i--)
  {
    delay(1);
    ring.setPixelColor(i, NORTHWESTERN);
    ring.setPixelColor(i+1, NORTHWESTERN);
    ring.setPixelColor(i+2, NORTHWESTERN);
    ring.show();
    delay(wait);
    ring.setPixelColor(i, 0x0); 
    ring.show();
  }
}

void RPM_Cycle(uint8_t wait) {
	delay(1);
	for (int i = 0; i < RPM_LEDS; i++) { // Fill in the first 45 LEDs one by one
		if (i < 4) ring.setPixelColor(i, CYAN - ((i - 11) * 23));
		else if (i < 8) ring.setPixelColor(i, GREEN);
		else if (i < 13) ring.setPixelColor(i, YELLOW);
		else if (i < 17) ring.setPixelColor(i, SCARLET);
		ring.show();
		delay(wait);
	}
  delay(wait);
	for (int i = RPM_LEDS-1; i > 0; i--) { // Remove color in the 45 LEDs one by one
		ring.setPixelColor(i, 0x0);
		ring.show();
		delay(wait);
	}
}
