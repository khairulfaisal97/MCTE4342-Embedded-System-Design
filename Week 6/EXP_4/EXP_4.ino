void setup() {
 // to run once
  unsigned char*ddrb= (unsigned char*)0x24;
  char*ocr1al = (unsigned char*)0x88;
  unsigned char*tccr1a = (unsigned char*)0x80;
  unsigned char*tccr1b = (unsigned char*)0x81;
  
  *ddrb=2;
  *ocr1al =255;
  *tccr1a =0b10000011;
  *tccr1b =0b00000010;
}

void loop() {
  //to run repeatedly
}
