#include <Servo.h>

Servo servo;
unsigned int middle = 70;
unsigned int down = 35;
unsigned int up = 80;
unsigned int jump_delay = 600;
unsigned int jump_down_delay = 0;
unsigned long interval = 15000;
unsigned long prevTime = 0;

void setup() {
  Serial.begin(9600);
  servo.attach(9);
  servo.write(middle);
}

void loop() {
  unsigned long now_time = millis();
  int is_obstacle_light = analogRead(A0);
  delay(100);

  servo.write(middle);
  if ((now_time - prevTime >= interval) && jump_delay >= 150)
  {
    prevTime = now_time;
    jump_delay -= 50;
    jump_down_delay = 600 - jump_delay;
  }
  Serial.println(is_obstacle_light);
  if ((is_obstacle_light > 500) && is_obstacle_light < 650) 
  {
    Serial.println("[아침] 장애물 감지");
    servo.write(up);
    delay(jump_delay);
    // if (jump_down_delay > 150)
    // {
    //   servo.write(down);
    //   delay(jump_down_delay);
    // }
  }
  if ((is_obstacle_light < 240) && is_obstacle_light > 150) 
  {
    Serial.println("[밤] 장애물 감지");
    servo.write(up);
    delay(350);
    servo.write(down);
    delay(150);
  }
}