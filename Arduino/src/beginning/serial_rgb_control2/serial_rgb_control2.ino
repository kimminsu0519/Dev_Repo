const int R_LED = 3;
const int G_LED = 5;
const int B_LED = 6;

void setup() {
  Serial.begin(9600);

  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);
}

void loop() {
  while (Serial.available() > 0)
  {
    Serial.println("-----");
    char input = Serial.read();
    Serial.println(input);

    if (input == 'R')
    {
      digitalWrite(R_LED, HIGH);
      Serial.println("Red LED is ON !!");
    }
    else if (input == 'G')
    {
      digitalWrite(G_LED, HIGH);
      Serial.println("Green LED is ON !!");
    }  
    else if (input == 'B')
    {
      digitalWrite(B_LED, HIGH);
      Serial.println("Blue LED is ON !!");
    }
    
    else
    {
      digitalWrite(R_LED, LOW);
      digitalWrite(G_LED, LOW);
      digitalWrite(B_LED, LOW);
      Serial.println("Not a command !!");
    }
  }
}
