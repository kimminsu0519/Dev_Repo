const int BUTTON_PIN = 13;
const bool UP = true;
const bool DOWN = false;

bool direction = UP;
int press;
int button_state;
int pre_button_state = LOW;
int click = 0;
int digits[10][7]
{
  {1,1,1,1,1,1,0}, // digit 0
  {0,1,1,0,0,0,0}, // digit 1
  {1,1,0,1,1,0,1}, // digit 2
  {1,1,1,1,0,0,1}, // digit 3
  {0,1,1,0,0,1,1}, // digit 4
  {1,0,1,1,0,1,1}, // digit 5
  {1,0,1,1,1,1,1}, // digit 6
  {1,1,1,0,0,0,0}, // digit 7
  {1,1,1,1,1,1,1}, // digit 8
  {1,1,1,1,0,1,1} // digit 9
};

void setup() 
{
  Serial.begin(9600);
  for (int i = 2; i <= 8; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(BUTTON_PIN, INPUT);

  // click을 7세그먼트로 출력하기
  for (int i = 0; i <= 6; i++)
  {
    if (digits[click][i] == 1)
    {
      digitalWrite(i + 2, HIGH);
    }
    else 
    {
      digitalWrite(i + 2, LOW);
    }
  }
}

void loop() 
{
  button_state = digitalRead(BUTTON_PIN);

  // 버튼 눌렸는지 체크
  if (pre_button_state == HIGH && button_state == LOW)
  {
    press = HIGH;
  }
  else
  {
    press = LOW;
  }

  if (press == HIGH)
  {
    if (direction == UP)
    {
      click++;
      if (click == 9)
      {
        direction = DOWN;
      }
    }
    else
    {
      click--;
      if (click == 0)
      {
        direction = UP;
      }
    }
  }

  // click을 7세그먼트로 출력하기
  for (int i = 0; i <= 6; i++)
  {
    if (digits[click][i] == 1)
    {
      digitalWrite(i + 2, HIGH);
    }
    else 
    {
      digitalWrite(i + 2, LOW);
    }
  }
  pre_button_state = button_state; // 버튼 이전상태 저장
}