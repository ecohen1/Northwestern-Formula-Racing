#define LCD_WIDTH		800		//Number of X Pixels
#define LCD_HEIGHT		480		//Number of Y Pixels
#define LINE_WIDTH		8		//Width of Borders
#define TIME_OFFSET		455
#define BORDER1			70
#define BORDER2			415
#define BORDER3			700
#define COLUMN1			0
#define COLUMN2			267
#define COLUMN3			470
#define ROW1			0
#define ROW2			70
#define ROW3			100
#define ROW4			170
#define ROW5			200
#define ROW6			330
#define ROW7			300
#define BAR_ROW         419
#define BAR_COL         460
#define BAR_SPACE       80
#define BAR_WIDTH       70

uint32_t front_color = 0x0;		//Foreground Color
uint32_t back_color = 0x0;		//Background Color
uint32_t line_color = 0x0;		//Line Color	
uint32_t text_color = 0x0;		//Text Color
uint32_t tp_color = 0x00FF64;
uint32_t brake_color = 0xF22613;
uint32_t clutch_color = 0xFF5E00;

float oldOil = 0.;
float oldThrottle = 0.;
float oldTemp = 0.;
float oldSpeed = 0.;
float oldRPM = 0.;
float oldlambda = 0.;
float oldbattvolt = 0.;
float old_a = 0.;
float oldbrake = 0.;
float oldclutch = 0.;
float tol = 1;
float sec = 0;
void print_drive_mode();

void setupLCD() {
	tft.begin(Adafruit_800x480);
	tft.PWMout(1,255);
	//Custom Text Upload
    tft.changeMode(TEXT);
	uploadNum(char0, 24, 0);
	uploadNum(char1, 24, 24);
	uploadNum(char2, 24, 48);
	uploadNum(char3, 24, 72);
	uploadNum(char4, 24, 96);
	uploadNum(char5, 24, 120);
	uploadNum(char6, 24, 144);
	uploadNum(char7, 24, 168);
	uploadNum(char8, 24, 192);
	uploadNum(char9, 24, 216);
	uploadNum(char_engine, 10, 240);
    uint8_t hello[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    tft.uploadUserChar(hello, 255);
	tft.setScrollWindow(0, BORDER3, BORDER1, BORDER2);
    //Must upload an extra char to get rid of garbage in last upload
	//Setup Background
	resetLCD("");
	//setTime(1430691518);
}


void resetLCD(String message) {
	tft.fillScreen(back_color);
	tft.setFontScale(0);
	tft.setTextColor(front_color, back_color);
	printNum(16, 80, 0, 2, 0);
	tft.setFontScale(3);
	tft.setTextColor(text_color, back_color);
	tft.setCursor(TIME_OFFSET + 95, ROW1);
	tft.print(":");
	tft.setCursor(TIME_OFFSET + 215, ROW1);
	tft.print(":");
	tft.setCursor(10, ROW1);
	tft.print("NFR");
	tft.setFontScale(1);
	tft.setCursor(COLUMN1+115, ROW2);
	tft.print("OIL(PSI)");
	tft.setCursor(COLUMN1+115, ROW4);
	sprintf(temp, "ET (%cF)", 176);
	tft.print(temp);
	tft.setCursor(COLUMN2+30, ROW2);
	tft.print("BATT1 (V)");
	tft.setCursor(COLUMN2+30, ROW4);
	tft.print("CURR1 (A)");
	tft.setCursor(COLUMN3+70, ROW2);
	tft.print("LAMBDA");
	tft.fillCircle(360, 365, 70, line_color);
	tft.fillCircle(360, 365, 60, back_color);
	tft.fillRect(BORDER3 - LINE_WIDTH / 2, BORDER1, LINE_WIDTH, BORDER2 - BORDER1, line_color);
	tft.fillRect(0, BORDER2 + LINE_WIDTH / 2, LCD_WIDTH, LCD_HEIGHT - BORDER2, back_color);
	tft.fillRect(0, BORDER1 - LINE_WIDTH / 2, LCD_WIDTH, LINE_WIDTH, line_color);
	tft.fillRect(0, BORDER2 - LINE_WIDTH / 2, LCD_WIDTH, LINE_WIDTH, line_color);
	tft.fillRect(0, 300 - 5, 360, 9, line_color);  
    tft.fillRect(BAR_COL+2*BAR_SPACE, BAR_ROW, BAR_WIDTH, -throtpos*2-LINE_WIDTH, RGB565(0x00FF64)); //Throttle Position
    tft.fillRect(BAR_COL+BAR_SPACE, BAR_ROW, BAR_WIDTH, -brake_pressure*.4-LINE_WIDTH, RGB565(0xF22613)); //Brake Pressure
    tft.fillRect(BAR_COL, BAR_ROW, BAR_WIDTH, -oil_pressure*2-LINE_WIDTH, RGB565(0xFF5E00)); //Oil Pressure
	tft.setCursor(COLUMN2, 420);
	tft.setFontScale(3);
	tft.print(message);
	tft.setFontScale(0);
	printNum(oil_pressure, COLUMN1, ROW3, 5, 1); // oil pressure		
	printNum(engine_temp, COLUMN1, ROW5, 5, 1); // Engine temp
	printNum(rpm, 10, 330, 5, 0); // RPM
	printEngine(735, 100);
    print_drive_mode();
}

void refreshTime(){
	tft.setFontScale(0);
    tft.setTextColor(front_color,back_color);
	//Update Time
	//printNum((hour(t)), TIME_OFFSET, ROW1, 2, 0);
  printNum(0, TIME_OFFSET, ROW1, 2, 0);
  //if ((minute(t)) < 10){
	if (0 < 10) {
		printNum(0, TIME_OFFSET+120, ROW1, 1, 0);
    //printNum(minute(t), TIME_OFFSET+170, 0, 1, 0);
		printNum(0, TIME_OFFSET+170, 0, 1, 0);
	}
  //else printNum(minute(t), TIME_OFFSET + 120, ROW1, 2, 0);
	else printNum(0, TIME_OFFSET + 120, ROW1, 2, 0);
	//if (second(t) < 10) {
	if (sec < 10) {
		printNum(0, TIME_OFFSET+240, ROW1, 1, 0);
		//printNum(second(t), TIME_OFFSET+290, ROW1, 1, 0);
    printNum(sec, TIME_OFFSET+290, ROW1, 1, 0);
	}
	//else printNum(second(t), TIME_OFFSET+240, ROW1, 2, 0);
  else printNum(sec, TIME_OFFSET+240, ROW1, 2, 0);

  sec++;
}

void refreshLCD(int page) {
    refreshTime();
	tft.setFontScale(0);
    tft.setTextColor(front_color,back_color);
	if (abs(oil_pressure - oldOil) > tol) {
		printNum(oil_pressure, COLUMN1, ROW3, 5, 1); // Oil	
    tft.fillRect(BAR_COL, BAR_ROW, BAR_WIDTH, -100*2-LINE_WIDTH, back_color);
    tft.fillRect(BAR_COL, BAR_ROW, BAR_WIDTH, -oil_pressure*2-LINE_WIDTH, RGB565(0xFF5E00));
    
		oldOil = oil_pressure;
	}
	if (abs(brake_pressure - oldbrake) > tol) {
        tft.fillRect(BAR_COL+BAR_SPACE, BAR_ROW, BAR_WIDTH, -100*2-LINE_WIDTH, back_color);
        tft.fillRect(BAR_COL+BAR_SPACE, BAR_ROW, BAR_WIDTH, -brake_pressure*.4-LINE_WIDTH, RGB565(0xF22613));
		oldbrake = brake_pressure;
	}
	if (abs(throtpos - oldThrottle) > tol) {
		tft.fillRect(BAR_COL+2*BAR_SPACE, BAR_ROW, BAR_WIDTH, -100*2-LINE_WIDTH, back_color);
		tft.fillRect(BAR_COL+2*BAR_SPACE, BAR_ROW, BAR_WIDTH, -throtpos*2-LINE_WIDTH, RGB565(0x00FF64));
		oldThrottle = throtpos;
	}
	if (abs(engine_temp - oldTemp) > tol) {
		printNum(engine_temp, COLUMN1, ROW5, 5, 1); // Engine temp
		oldTemp = engine_temp;
	}
	if (abs(rpm - oldRPM) > 30*tol) {
		printNum(rpm, 10, 330, 5, 0); // RPM
		oldRPM = rpm;
	}
	if (abs(battvolt - oldbattvolt) > .2 * tol) {
		printNum(battvolt, COLUMN2, ROW3, 4, 1);
		oldbattvolt = battvolt;
	}
	if (abs(main_a - old_a) > .2 * tol) {
		printNum(main_a, COLUMN2, ROW5, 4, 1); 
		old_a = main_a;
	}
	if (abs(lambda1 - oldlambda) > .02 * tol) {
		printNum(lambda1, COLUMN3, ROW3, 4, 2);
		oldlambda = lambda1;
	}
	printNum(gear, 335, ROW6, 1, 0); // Gear
	tft.setCursor(COLUMN1,420);
	tft.setFontScale(3);
	if (old_auto_shift != auto_shift) {
		print_drive_mode();
	} 
}

void print_drive_mode() {
  tft.fillRect(0, BORDER2 + LINE_WIDTH / 2, LCD_WIDTH/3., LCD_HEIGHT - BORDER2, back_color);
  tft.setTextColor(front_color,back_color);
  tft.setCursor(COLUMN1,420);
  tft.setFontScale(3);
  switch(auto_shift) { // Drive mode
    case 0:
    tft.print("MANUAL");
    break;
    case 1:
    tft.print("AUTO");
    break;
  }
  old_auto_shift = auto_shift;
}

void uploadNum(const uint8_t c[][16], int len, int N) {
	int i, j;
	uint8_t upload[16];
	for (j = 0; j <len; j++) {
		for (i = 0; i<16; i++) {
			upload[i] = c[j][i];
		}
		tft.uploadUserChar(upload, j + N);
	}
}

void printNum(float num, int x, int y, int len, int dec) {
	int i = 0, j = 0;
	int spacing = 0;
	char digits[16];
	sprintf(digits, "%*.*f", len, dec, num);
	for (i = 0; i<len; i++) {
		if (digits[i] > 47){
			tft.setTextColor(front_color,back_color);
			for (j = 0; j<4; j++) {
				tft.setCursor(x + 54 * i - spacing, y + j * 16);
				tft.showUserChar(6 * j + 24 * (digits[i] - 48), 5);
			}
		}
		if (digits[i] == 46) {
			spacing = 40;
			tft.fillRect(x + 54 * i + 38 - spacing, y + 54, 10, 10, front_color);
		}
		if (digits[i] == 32) tft.fillRect(x + 54*i, y, 47, 63, back_color);
	}
}

void printEngine(int x, int y) {
	int i = 0, j = 0;
	if (emergency) tft.setTextColor(RGB565(YELLOW), back_color);
	else tft.setTextColor(back_color, back_color);
	for (j = 0; j < 2; j++) {
		tft.setCursor(x + 54*i, y + 16*j);
		tft.showUserChar(5*j + 240, 4);
	}
}

uint16_t RGB565(uint32_t rgb) {
    return (((rgb >> 19) & 0x1F) << 11) | (((rgb >> 10) & 0x3F) << 5) | (((rgb >> 3) & 0x1F));
}
