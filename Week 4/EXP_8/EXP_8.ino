int Button1 = 2;
int Button2 = 3;
int Button3 = 4;
int Button4 = 5;

int countA=0,countB=0,countC=0,countD=0;

void setup() {
  Serial.begin(9600);
  pinMode(Button1, INPUT);
  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);
  pinMode(Button4, INPUT);
}

void loop() { 
  if(digitalRead(Button1) != 0){
    countA++;
    printcount();
  }
  else if(digitalRead(Button2) != 0){
    countB++;
    printcount();
  }
  else if(digitalRead(Button3) != 0){
    countC++;
    printcount();
  }
  else if(digitalRead(Button4) != 0){
    countD++;
    printcount();
  }
}

void printcount(){
  Serial.print("A = ");
  Serial.print(countA);

  Serial.print(", B = ");
  Serial.print(countB);

  Serial.print(", C = ");
  Serial.print(countC);

  Serial.print(", D = ");
  Serial.print(countD);

  Serial.print('\n');
  delay(400);
}
