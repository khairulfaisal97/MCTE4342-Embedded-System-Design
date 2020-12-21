int LED1 = 2;
int LED2 = 3;
int LED3 = 4;
int LED4 = 5;
int LED5 = 6;
int LED6 = 7;
int LED7 = 8;
int LED8 = 9;

void setup() {
  pinMode(LED1, INPUT);
  pinMode(LED2, INPUT);
  pinMode(LED3, INPUT);
  pinMode(LED4, INPUT);
  pinMode(LED5, INPUT);
  pinMode(LED6, INPUT);
  pinMode(LED7, INPUT);
  pinMode(LED8, INPUT);
}

void loop() {
  light(LED1,781,515);
  light(LED2,2014,1348);  
  light(LED3,343,573);  
  light(LED4,678,1839);  
  light(LED5,342,534);  
  light(LED6,1478,326);
  light(LED7,1859,351);
  light(LED8,777,888);
}

void light(int pin, int delayon, int delayoff){
  digitalWrite(pin, HIGH);
  delay(delayon);
  digitalWrite(pin, LOW);
  delay(delayoff);
}
