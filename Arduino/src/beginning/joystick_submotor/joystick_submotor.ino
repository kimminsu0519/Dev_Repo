#include <Servo.h>

const int X_AXIS = A0;
const int Y_AXIS = A1;
const int X_ZERO_POS = 514;
const int Y_ZERO_POS = 534;
const int SERVO_PIN = 2;

int x;
int y;
double degree;
int x_pos;
int y_pos;
Servo servo;

void setup() 
{
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
}

void loop() 
{
  x = analogRead(X_AXIS);
  y = analogRead(Y_AXIS);

  x = x - X_ZERO_POS;
  y = (y - Y_ZERO_POS) * (-1);

  // 미세하게 튀는 값 제어
  if (x > -5 && x < 5)
  {
    x = 0;
  }
  if (y > -5 && y < 5)
  {
    y = 0;
  }

  x_pos = map(x, X_ZERO_POS * (-1), X_ZERO_POS, 180, 0);

  degree = atan2(x, y) * 180 / PI;

  if (degree < 0)
  {
    degree = degree * (-1);
  }

  servo.write(180 - degree);

  delay(100);
}