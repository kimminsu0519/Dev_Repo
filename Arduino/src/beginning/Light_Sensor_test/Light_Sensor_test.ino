int max_light_value;
int min_light_value;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  max_light_value = 0;
  min_light_value = 9999;
}

void loop() {
  // put your main code here, to run repeatedly:
  int light = analogRead(A0);
  delay(250);
  if (light > max_light_value)
  {
    max_light_value = light;
  }
  if (light < min_light_value)
  {
    min_light_value = light;
  }

  Serial.print(min_light_value);
  Serial.print(", ");
  Serial.println(max_light_value);
}
