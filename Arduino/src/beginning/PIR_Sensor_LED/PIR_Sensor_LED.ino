const int PIR_PIN = 2;
const int LED_PIN = 4;
const unsigned long LED_ON_TIME = 5000; // pir센서가 한번 감지되면 1초동안 유지되는 것 유의하기

int pir_check;
bool led_on_check = false;
unsigned long now_led_on_time = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() 
{
  pir_check = digitalRead(PIR_PIN);

  if (pir_check == HIGH)
  {
    led_on_check = true;
    now_led_on_time = millis();
  }

  if (millis() - now_led_on_time < LED_ON_TIME && led_on_check == true)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);    
    led_on_check = false;
  }
}