void shift(){

  if (gear_req == 1){
  //upshift
    //immediately tells SW to reset shift request!
    gear_reset = 1;
    gear_req = 0;
    sendCAN();
    del = 0;
    //if (gear == 1) del = 25;
    //digitalWrite(clutchpin,HIGH);
    //delay(d1);//btw shifter middle, clutch in
    digitalWrite(IGN_CUT, HIGH);
    delay(CUT_TO_SHIFT);
    digitalWrite(upshiftpin,HIGH);
    delay(ud2+del);//btw shifter middle, clutch in
    digitalWrite(upshiftpin,LOW);
    delay(SHIFT_TO_ON);
    digitalWrite(IGN_CUT, LOW);
    //delay(d3);//btw shifter middle, clutch in
    //increment gear, update SW data
    gear_reset = 1;
    gear_req = 0;
    gear++;
    if (gear > 6) gear = 6;
    sendCAN();
  }
  else if (gear_req == 2 && gear >= 1) {
  //downshift
    gear_reset = 1;
    gear_req = 0;
    sendCAN();
    del = 0;
    //if (gear == 2) del = 100;
    digitalWrite(clutchpin,HIGH);
    delay(dd1+del);
    digitalWrite(dnshiftpin,HIGH);
    delay(dd2+del);
    digitalWrite(dnshiftpin,LOW);
    delay(dd3+del);
    gear_reset = 1;    
    gear_req = 0;
    gear--;
    if (gear < 1) gear = 1;
    sendCAN();  
  }
  else {
    if (clutch_pos_req < 200){
      digitalWrite(clutchpin,LOW);
    }
    else {
      digitalWrite(clutchpin,HIGH);
    }    
  }
  
}
