#define KNOB 0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(KNOB);
  Serial.println(val);
}
