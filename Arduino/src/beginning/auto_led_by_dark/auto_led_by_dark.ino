const int LED_PIN = 9;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int light = analogRead(A0);
  delay(100);

  if (light < 300) 
  {
    light = 300;
  }
  if (light > 900) 
  {
    light = 900;
  }  
  int output = map(light, 300, 900, 5, 250);
  Serial.print(light);
  Serial.print(", ");
  Serial.println(output);

  output = 250 - output;
  analogWrite(LED_PIN, output);
}
