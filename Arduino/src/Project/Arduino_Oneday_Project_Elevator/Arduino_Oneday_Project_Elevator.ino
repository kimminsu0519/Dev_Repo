const int MAX_FLOOR = 3;
const int LED_COUNT_BETWEEN_FLOORS = 2;
const float FLOOR_DELAY_TIME = 3000;

const int RED_LED_PINS[MAX_FLOOR] = {2, 6, 10};
const int GREEN_LED_PINS[MAX_FLOOR] = {3, 7, 11};
const int YELLOW_LED_PINS[LED_COUNT_BETWEEN_FLOORS * (MAX_FLOOR - 1)] = {4, 5, 8, 9};
const int BUTTON_PIN_LIST[MAX_FLOOR] = {A0, A1, A2};
const int EMERGENCY_LED_PIN = 13;
const int EMERGENCY_BUTTON_PIN = A5;

enum ElevatorDirection
{
  DOWN = 0,
  UP
};

enum ElevatorStatus
{
  STOP = 0,
  MOVING
};

enum CallState
{
  DONTCALL = 0,
  CALL
};

int now_position = 1;
ElevatorStatus now_status = STOP;
ElevatorDirection now_direction = UP;
bool floor_status_list[MAX_FLOOR] = {LOW};
CallState previous_button_state_list[MAX_FLOOR] = {DONTCALL};
CallState current_button_state_list[MAX_FLOOR] = {DONTCALL};
CallState emergency_previous_button_state = DONTCALL;
CallState emergency_current_button_state = DONTCALL;
CallState emergency_check = DONTCALL;
unsigned long last_moving_time = 0;
unsigned long emergency_previous_moving_keep_time = 0;
unsigned long now_time;

bool status_one_time_check = true;
bool moving_one_time_check = true;
bool call_one_time_check = true;
ElevatorDirection led_tmp_direction;
int led_tmp_position;

int isCall (bool status_list[], int size);
int getCallDirection (bool status_list[], int size, int current_position, int dir);
void onCallElevatorLED (int current_position, int dir);
void checkEmergencyButton();
void checkFloorButton();

void setup () 
{
  // Serial.begin(9600);

  for (int i = 0; i < MAX_FLOOR; i++) {
    pinMode(RED_LED_PINS[i], OUTPUT);
    pinMode(GREEN_LED_PINS[i], OUTPUT);
    pinMode(BUTTON_PIN_LIST[i], INPUT);
  }
  for (int i = 0; i < LED_COUNT_BETWEEN_FLOORS * (MAX_FLOOR - 1); i++) {
    pinMode(YELLOW_LED_PINS[i], OUTPUT);
  }
  pinMode(EMERGENCY_LED_PIN, OUTPUT);
  pinMode(EMERGENCY_BUTTON_PIN,INPUT);

  digitalWrite(RED_LED_PINS[now_position - 1], HIGH);
}

// --------------------------------------------------------MAIN LOOP------------------------------------------------
void loop () 
{
  now_time = millis();

  checkEmergencyButton();
  checkFloorButton();

  // 버튼입력은 계속 받고, 이것과 비동기적으로 LED는 계속 동작하기
  if (now_status == MOVING)
  {
    onCallElevatorLED(now_position, now_direction);

    if (call_one_time_check == false)
    {
      return;
    }

    if (isCall(floor_status_list, MAX_FLOOR) == static_cast<int>(CallState::CALL))
    {
      call_one_time_check = false;
      now_direction = (ElevatorDirection)getCallDirection(floor_status_list, MAX_FLOOR, now_position, now_direction);
    }
    
    return;
  }
  
  call_one_time_check = true;

  // 호출 및 엘리베이터 이동 방향 체크
  if (isCall(floor_status_list, MAX_FLOOR) == static_cast<int>(CallState::CALL))
  {
    if (getCallDirection(floor_status_list, MAX_FLOOR, now_position, now_direction) == (int)now_direction)
    {
      if (now_status == STOP)
      {
        now_status = MOVING;
        last_moving_time = millis();
        onCallElevatorLED(now_position, now_direction);
      }
    }
    else
    {
      if (now_direction == UP)
      {
        now_direction = DOWN;
      }
      else
      {
        now_direction = UP;
      }

      if (now_status == STOP)
      {
        now_status = MOVING;
        last_moving_time = millis();
        onCallElevatorLED(now_position, now_direction);
      }      
      return;
    }
  }
  else
  {
    return;
  }
}
// -------------------------------------------------------MAIN LOOP------------------------------------------------


// -----------------------------------------------------isCall function------------------------------------------------
int isCall (bool status_list[], int size)
{
  for (int i = 0; i < size; i++)
  {
    if (status_list[i] != LOW)
    {
      return HIGH;
    }
  }

  return LOW;
}
// -----------------------------------------------------isCall function------------------------------------------------


// -------------------------------------------------getCallDirection function------------------------------------------------
int getCallDirection (bool status_list[], int size, int current_position, int dir)
{
  if (status_list[current_position - 1] == HIGH)
  {
    return dir;
  }

  if (dir == static_cast<int>(ElevatorDirection::UP))
  {
    for (int i = current_position - 1; i < size; i++)
    {
      if (status_list[i] == HIGH)
      {
        return static_cast<int>(ElevatorDirection::UP);
      }
    }

    return static_cast<int>(ElevatorDirection::DOWN);
  }
  else
  {
    for (int i = current_position - 1; i >= 0 ; i--)
    {
      if (status_list[i] == HIGH)
      {
        return static_cast<int>(ElevatorDirection::DOWN);
      }
    }

    return static_cast<int>(ElevatorDirection::UP);
  }

  // Serial.println("[ERROR] getCallDirection ");
  return dir;
}
// -------------------------------------------------getCallDirection function------------------------------------------------


// -------------------------------------------------onCallElevatorLED function------------------------------------------------
void onCallElevatorLED(int current_position, int dir)
{
  if (moving_one_time_check == true)
  {
    moving_one_time_check = false;
    led_tmp_direction = (ElevatorDirection)dir;
    led_tmp_position = current_position;

    if (led_tmp_direction == UP)
    {
      now_position = led_tmp_position + 1;
      floor_status_list[led_tmp_position] = LOW;
    }
    else
    {
      now_position = led_tmp_position - 1;
      floor_status_list[led_tmp_position - 2] = LOW;
    }
  }

  if (emergency_check == static_cast<int>(CallState::CALL))
  {
    return;
  }

  // onCallElevatorLED 실행 후 0~1초 안
  if (now_time - last_moving_time + emergency_previous_moving_keep_time < (FLOOR_DELAY_TIME / (LED_COUNT_BETWEEN_FLOORS + 1)))
  {
    // 이전 RED, GREEN LED OFF 그리고 호출상태 OFF
    digitalWrite(RED_LED_PINS[led_tmp_position - 1], LOW);
    if (status_one_time_check == true)
    {
      digitalWrite(GREEN_LED_PINS[led_tmp_position - 1], LOW);
      floor_status_list[led_tmp_position - 1] = LOW;
      status_one_time_check = false;
    }

    // YELLOW LED ON
    if (led_tmp_direction == UP)
    {
      digitalWrite(YELLOW_LED_PINS[LED_COUNT_BETWEEN_FLOORS * (led_tmp_position - 1)], HIGH);
    }
    else
    {
      digitalWrite(YELLOW_LED_PINS[(LED_COUNT_BETWEEN_FLOORS * (led_tmp_position - 1)) - 1], HIGH);
    }

    return;
  }
  // onCallElevatorLED 실행 후 1~2초 동안
  else if (now_time - last_moving_time + emergency_previous_moving_keep_time < (FLOOR_DELAY_TIME / (LED_COUNT_BETWEEN_FLOORS + 1)) * 2)
  {
    // YELLOW LED ON/OFF
    if (led_tmp_direction == UP)
    {
      digitalWrite(YELLOW_LED_PINS[LED_COUNT_BETWEEN_FLOORS * (led_tmp_position - 1)], LOW);
      digitalWrite(YELLOW_LED_PINS[LED_COUNT_BETWEEN_FLOORS * (led_tmp_position - 1) + 1], HIGH);
    }
    else
    {
      digitalWrite(YELLOW_LED_PINS[(LED_COUNT_BETWEEN_FLOORS * (led_tmp_position - 1)) - 1], LOW);
      digitalWrite(YELLOW_LED_PINS[(LED_COUNT_BETWEEN_FLOORS * (led_tmp_position - 1)) - 2], HIGH);
    }

    return;
  }
  // onCallElevatorLED 실행 후 2~3초 동안
  else if (now_time - last_moving_time + emergency_previous_moving_keep_time < (FLOOR_DELAY_TIME / (LED_COUNT_BETWEEN_FLOORS + 1)) * 3)
  {
    // 이전 YELLOW LED OFF, RED ON 그리고 호출 상태 OFF 및 GREEN LED OFF
    if (led_tmp_direction == UP)
    {
      digitalWrite(YELLOW_LED_PINS[LED_COUNT_BETWEEN_FLOORS * (led_tmp_position - 1) + 1], LOW);
      digitalWrite(RED_LED_PINS[led_tmp_position], HIGH);
      digitalWrite(GREEN_LED_PINS[led_tmp_position], LOW);
      floor_status_list[led_tmp_position] = LOW;
    }
    else
    {
      digitalWrite(YELLOW_LED_PINS[LED_COUNT_BETWEEN_FLOORS * (led_tmp_position - 1) - 2], LOW);
      digitalWrite(RED_LED_PINS[led_tmp_position - 2], HIGH);
      digitalWrite(GREEN_LED_PINS[led_tmp_position - 2], LOW);
      floor_status_list[led_tmp_position - 2] = LOW;
    }

    return;
  }
  // onCallElevatorLED 실행 후 3초가 지남
  else
  {
    now_status = STOP;
    status_one_time_check = true;
    moving_one_time_check = true;
    emergency_previous_moving_keep_time = 0;

    return;
  }

  // Serial.println("[ERROR] onCallElevatorLED ");
  return;
}
// -------------------------------------------------onCallElevatorLED function------------------------------------------------


// -------------------------------------------------checkEmergencyButton function------------------------------------------------
void checkEmergencyButton()
{
  emergency_current_button_state = (CallState)digitalRead(EMERGENCY_BUTTON_PIN);

  if ((emergency_current_button_state == DONTCALL) && (emergency_previous_button_state == CALL))
  {
    if (emergency_check == CALL)
    {
      emergency_check = DONTCALL;
    }
    else
    {
      emergency_check = CALL;
    }

    if (emergency_check == CALL)
    {
      digitalWrite(EMERGENCY_LED_PIN, HIGH);
      emergency_previous_moving_keep_time = now_time - last_moving_time;
    }
    else
    {
      digitalWrite(EMERGENCY_LED_PIN, LOW);
      last_moving_time = millis();
    }
  }
  // 비상 정지 버튼 이전상태 저장
  emergency_previous_button_state = emergency_current_button_state;
}
// -------------------------------------------------checkEmergencyButton function------------------------------------------------


// -------------------------------------------------checkFloorButton function------------------------------------------------
void checkFloorButton()
{
  for (int i = 0; i < MAX_FLOOR; i++)
  {
    current_button_state_list[i] = (CallState)digitalRead(BUTTON_PIN_LIST[i]);

    if ((current_button_state_list[i] == CALL) && (previous_button_state_list[i] == DONTCALL))
    {
      if (floor_status_list[i] == HIGH) // 이미 켜져있다면, OFF하기
      {
        floor_status_list[i] = LOW;
        digitalWrite(GREEN_LED_PINS[i], LOW);        
      }
      else
      {
        floor_status_list[i] = HIGH;
        digitalWrite(GREEN_LED_PINS[i], HIGH);
      }
    }
  }
  // 버튼 이전상태 저장
  for (int i = 0; i < MAX_FLOOR; i++)
  {
    previous_button_state_list[i] = current_button_state_list[i];
  }
}
// -------------------------------------------------checkFloorButton function------------------------------------------------