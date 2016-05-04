#include <SPI.h>
#include <SD.h>
#include <FlexCAN.h>

#include "global.h"
#include "CAN.h"


File myFile;
File susFile;
int d = 6;
int count = 0;
int temp = -999;
int susP1 = -999;
int susP2 = -999;
int tempPin = 14;
int susPin1 = 20;
int susPin2 = 21;

void setup()
{
 // Open serial communications and wait for port to open:
//  Serial.begin(9600);
//   while (!Serial) {
////    ; // wait for serial port to connect. Needed for Leonardo only
//  }
//  Serial.println("test");
//  delay(d);
//  Serial.print("starting up");
  pinMode(10, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(tempPin, INPUT);
  pinMode(susPin1, INPUT);
  pinMode(susPin2, INPUT);

  setupCAN();
//  delay(d);

  while (!SD.begin(10)) {
//    Serial.print("Initializing SD card...");
//    Serial.println("initialization failed!");
    delay(d);
  }
  myFile = SD.open("temp.txt", FILE_WRITE);
  if (myFile.size() < 20) {
    myFile = SD.open("temp.txt", FILE_WRITE);
    myFile.println("Temp FR, Temp FL, Temp BR, Temp BL, SusPot FR, SusPot FL, Brake Pressure, Clutch Req");
  }
  myFile.close();
//  Serial.println("initialization done.");
}


void loop()
{
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.

  count++;
  processCAN();
  if(count % 4 == 0) {//1
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
  }
  else if(count % 4 == 1) {//2
    digitalWrite(5,HIGH);
    digitalWrite(6,LOW);
  }
  else if(count % 4 == 2) {//3
    digitalWrite(5,LOW);
    digitalWrite(6,HIGH);
  }
  else if(count % 4 == 3) {//4
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
  }
  delay(d);
  temp = analogRead(tempPin);
  analogReadRes(16);
  susP1 = analogRead(susPin1);
  susP2 = analogRead(susPin2);
  analogReadRes(10);
//  Serial.print(temp);
//  Serial.print("\t");
//  Serial.print((count%4)+1);
//  Serial.print(" is ");
  float degree = -999;
  if(count%4==0)
    degree = temp4(temp);
  else if(count%4==1)
    degree = temp2(temp);
  else if(count%4==2)
    degree = temp2(temp);
  else if(count%4==3)
    degree = temp4(temp);
//  Serial.print(degree);
//  Serial.println(" degrees");
  // open a new file and immediately close it:
//  delay(d);
//  Serial.println("Creating data.txt...");
  myFile = SD.open("temp.txt", FILE_WRITE);
//  delay(d);
  myFile.print(degree);
  myFile.print(",");
  if (count % 4 == 3){
//    myFile.println(clutch_pos_req);
//    Serial.println(brake_pressure);
    myFile.print(susP1);
    myFile.print(",");
    myFile.print(susP2);
    myFile.print(",");
    myFile.print(brake_pressure);
    myFile.print(",");
    myFile.println(clutch_pos_req);
  }
  myFile.close();

//  susFile = SD.open("suspension.txt", FILE_WRITE);
//  susFile.print(susP1);
//  susFile.print(",");
//  susFile.println(susP2);
//  susFile.close();

  delay(d);
  if (SD.exists("temp.txt")) {
//    Serial.println("data.txt exists.");
  }
  else {
//    Serial.println("data.txt doesn't exist.");
    while (!SD.begin(10)) {
//      Serial.print("Initializing SD card...");
//      Serial.println("initialization failed!");
    }
  }

}

float temp1(int voltage) {
  return (3*pow(10,-5)*pow(voltage,3) - 0.02*pow(voltage,2) + 5.6323*voltage - 185.06);
}

float temp2(int voltage) {
  return (3*pow(10,-5)*pow(voltage,3) - .0197*pow(voltage,2) + 5.3303*voltage - 147.75);
}

float temp4(int voltage) {
  return (3*pow(10,-5)*pow(voltage,3) - .0198*pow(voltage,2) + 5.4179*voltage - 150.4);
}
