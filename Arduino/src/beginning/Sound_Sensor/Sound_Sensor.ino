const int SOUND_PIN = A0;
unsigned int max = 1;
unsigned int min = 1023;


void setup() {
  Serial.begin(9600);
}

void loop() {
  int sound_level = analogRead(SOUND_PIN);
  delay(50);

  if (sound_level < min)
  {
    min = sound_level;
  }
  else if (sound_level > max)
  {
    max = sound_level;
  }
  Serial.print(sound_level);
  Serial.print(", ");
  Serial.print(min);
  Serial.print(", ");
  Serial.println(max);

}