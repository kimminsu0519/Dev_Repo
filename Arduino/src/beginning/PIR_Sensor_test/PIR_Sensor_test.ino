const int PIR_PIN = 2;

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  Serial.println(digitalRead(PIR_PIN));
}
