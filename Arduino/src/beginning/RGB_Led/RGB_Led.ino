const int R_LED = 3;
const int G_LED = 5;
const int B_LED = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 5; i < 250; i++)
  {
    analogWrite(R_LED, i);
    analogWrite(G_LED, i);
    analogWrite(B_LED, i);
    delay(10);
  } 
  for (int i = 250; i > 5; i--)
  {
    analogWrite(R_LED, i);
    analogWrite(G_LED, i);
    analogWrite(B_LED, i);
    delay(10);
  } 
}
