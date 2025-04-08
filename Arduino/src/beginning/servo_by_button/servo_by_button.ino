#include <Servo.h>

bool button_direction = true;
int PUSH_BUTTON = 2;
int pos = 0;
Servo servo;

void setup() {
  Serial.begin(9600);
  servo.attach(9);
}

void loop() {
  int button_status = digitalRead(PUSH_BUTTON);
  if (button_status == HIGH) 
  {
    Serial.println("----------------------------------------");
    Serial.println("button is pressed !!");

    if (button_direction == true)
    {
      Serial.println("direction : [+] ");

      for (int i = pos; i < pos + 10; i++)
      {
        servo.write(i);
        delay(15);
      }
      pos += 10;
      Serial.print("Position is swithed. Position : ");
      Serial.println(pos);

      if (pos == 180)
      {
        button_direction = false;
        Serial.println("direction is swithed. [-]");
      }
    }
    
    else
    {
      Serial.println("direction : [-] ");

      for (int i = pos; i > pos - 10; i--)
      {
        servo.write(i);
        delay(15);
      }
      pos -= 10;
      Serial.print("Position is swithed. Position : ");
      Serial.println(pos);     

      if (pos == 0)
      {
        button_direction = true;
        Serial.println("direction is swithed. [+]");
      }      
    }
  }
}
