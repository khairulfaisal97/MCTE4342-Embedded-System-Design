void setup() {
  // to run once
 unsigned char*ddrd=(unsigned char*)0x2A;
 unsigned char*ocr0a =(unsigned char*)0x47;
 unsigned char*ocr0b =(unsigned char*)0x48;
 unsigned char*tccr0a = (unsigned char*)0x44;
 unsigned char*tccr0b = (unsigned char*)0x45;
 
 *ddrd=1 <<5;
 *ocr0a =5;
 *ocr0b =2;
 *tccr0a =0b00100011;
 *tccr0b =0b00001001;
}

void loop() {
 // to run repeatedly 

}
