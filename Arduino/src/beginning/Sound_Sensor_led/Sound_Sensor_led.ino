const int SOUND_PIN = A0;
const int SOUND_MIN = 128;
const int SOUND_MAX = 512;
const int LED_MIN = 0;
const int LED_MAX = 8;
const int LED_SYCLE = 250;
const int LED_PINS[8] = {2,3,4,5,6,7,8,9};

int target_sound;
int target_led_level;
int tmp_led_level;
int count = 0;
int now_led_level = 0;

void setup() 
{
  for (int i = 0; i <= 7; i++)
  {
    pinMode(LED_PINS[i], OUTPUT);
  }
}

void loop() 
{
  target_sound = analogRead(SOUND_PIN);

  if (target_sound < SOUND_MIN)
  {
    target_sound = SOUND_MIN;
  }
  else if (target_sound > SOUND_MAX)
  {
    target_sound = SOUND_MAX;
  }

  target_led_level = map(target_sound, SOUND_MIN, SOUND_MAX, LED_MIN, LED_MAX);

  // LED_SYCLE 마다 led 제어
  if (count == LED_SYCLE)
  {
    if (now_led_level < target_led_level)
    {
      now_led_level++;
      digitalWrite(LED_PINS[now_led_level - 1], HIGH);
    }
    else
    {
      if (now_led_level != LED_MIN || now_led_level != LED_MAX)
      {
        digitalWrite(LED_PINS[now_led_level - 1], LOW);
        now_led_level--;
      }
    }
    count = 0;
  }
  else
  {
    count++;
  }
}