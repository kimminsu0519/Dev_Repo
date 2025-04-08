#include <Servo.h>

Servo servo;

void setup() {
  Serial.begin(9600);
  servo.attach(9);
  servo.write(35);
}

void loop() {
  int is_obstacle_light = analogRead(A0);
  delay(40);
  Serial.println(is_obstacle_light);

  if ((is_obstacle_light > 500) && is_obstacle_light < 720) 
  {
    Serial.println("[아침] 장애물 감지");
    servo.write(80);
    delay(350);
    servo.write(35);
    delay(150);
  }
  if ((is_obstacle_light < 240) && is_obstacle_light > 150) 
  {
    Serial.println("[밤] 장애물 감지");
    servo.write(85);
    delay(350);
    servo.write(25);
  }
    // servo.write(85);
    // delay(350);
    // servo.write(30);
    // delay(1000);
}