int LED1 = 8;
int LED2 = 9;

void setup() {
  pinMode(LED1, INPUT);
  pinMode(LED2, INPUT);
}

void loop() {
  digitalWrite(LED1, HIGH);
  delay(750);
  digitalWrite(LED1, LOW);
  delay(350);

  digitalWrite(LED2, HIGH);
  delay(400);
  digitalWrite(LED2, LOW);
  delay(600);
}
