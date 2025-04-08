#include <Servo.h>

Servo servo;

void setup() {
  Serial.begin(9600);
  servo.attach(9);
}

void loop() {
  while (Serial.available() > 0)
  {
    String input = Serial.readStringUntil('\n');
    float pos = input.toFloat();

    Serial.println(pos);
    servo.write(pos);
  }
}
