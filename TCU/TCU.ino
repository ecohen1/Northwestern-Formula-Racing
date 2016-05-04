#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <Metro.h>
#include <Time.h>
#include <math.h>
#include <string.h>
#include "global.h"

int test_num = 0;
void setup() {
  Serial.begin(115200);
  setupCAN();
  pinMode(clutchpin,OUTPUT);
  pinMode(upshiftpin,OUTPUT);
  pinMode(dnshiftpin,OUTPUT);
  pinMode(IGN_CUT,OUTPUT);
  pinMode(LAUNCH,OUTPUT);
  pinMode(TRACTION,OUTPUT);
  digitalWrite(clutchpin,LOW);
  digitalWrite(upshiftpin,LOW);
  digitalWrite(dnshiftpin,LOW);
  digitalWrite(IGN_CUT,LOW);
  digitalWrite(LAUNCH,LOW);
  digitalWrite(TRACTION,LOW);
}


void loop() {
  processCAN();
  if (test_num%2){//prevents double shifting - investigate  
    shift();
  }
  //test_num also for serial printing:
  //printing CAN on serial monitor would freeze,
  //this shows whether vals are changing
  test_num++;
  //if (test_num > 9) test_num = 0;
  delay(50);//prob unnecessary
  Serial.println(clutch_pos_req);
  gear_reset = 0;//boolean, if true sets gear_req to 0 (SW code)
  sendCAN();
  Serial.print("Hello");
}


