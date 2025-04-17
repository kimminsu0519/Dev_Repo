#include <SPI.h>
#include <MFRC522.h>
#include <List.hpp>
#include <LiquidCrystal.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

LiquidCrystal lcd(2,3,4,5,6,7); //RS,E,DB4,DB5,DB6,DB7
const int R_LED_PIN = A0;
const int G_LED_PIN = A1;
const int B_LED_PIN = A2;
const int LED_HIGH = 255;
const int LED_LOW = 0;
const int BUTTON_PIN = 8;
bool is_register_mode = true;     // 기본값은 Register Mode
bool prev_button_state = LOW;       // 이전 버튼 상태 저장
bool curr_button_state = LOW;
bool is_available_tag_check = true;

List<MFRC522::Uid> tag_list;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522

  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.write("Register Mode");

  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);  
  analogWrite(R_LED_PIN, LED_HIGH);
  analogWrite(G_LED_PIN, LED_HIGH);
  analogWrite(B_LED_PIN, LED_HIGH);  

  pinMode(BUTTON_PIN, INPUT);

	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
}

void loop() 
{
  curr_button_state = digitalRead(BUTTON_PIN);

  // 버튼 눌림 감지: HIGH → LOW 변화 (떼었다가 누른 순간)
  if (prev_button_state == HIGH && curr_button_state == LOW) 
  {
    is_register_mode = !is_register_mode; // 모드 토글

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(is_register_mode ? "Register Mode" : "Read Mode");
  }

  // 이전 버튼 상태 저장
  prev_button_state = curr_button_state;

  if (is_available_tag_check == true)
  {
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }

    if (is_register_mode == true)
    {
      // Dump debug info about the card; PICC_HaltA() is automatically called
      // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

      // Read UID
      Serial.print("Read UID TAG : ");
      for (byte i = 0; i < mfrc522.uid.size; i++)
      {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.println();

      Serial.print("Registered Tag List (");
      Serial.print(tag_list.getSize());
      Serial.print(") : ");
      Serial.println();

      for (int i = 0; i < tag_list.getSize(); i++)
      {
        for (byte j = 0; j < 4; j++)
        {
          Serial.print(tag_list.get(i).uidByte[j] < 0x10 ? " 0" : " ");
          Serial.print(tag_list.get(i).uidByte[j], HEX);
        }
        Serial.println();
      }

      bool registered = false;
      for (int i = 0; i < tag_list.getSize(); i++)
      {
        if (memcmp(tag_list.get(i).uidByte, mfrc522.uid.uidByte, 4) == 0)
        {
          registered = true;
        }
      }

      if(!registered)
      {
        tag_list.addLast(mfrc522.uid);
      }
    }
    else
    {
      bool registered = false;
      for (int i = 0; i < tag_list.getSize(); i++)
      {
        if (memcmp(tag_list.get(i).uidByte, mfrc522.uid.uidByte, 4) == 0)
        {
          registered = true;
        }
      }

      if(registered == true)
      {
        is_available_tag_check = false;
        for (int i = 0; i < 3; i++) // 파란색 3번 깜빡이기
        {
          analogWrite(R_LED_PIN, LED_LOW);
          analogWrite(G_LED_PIN, LED_LOW);
          analogWrite(B_LED_PIN, LED_HIGH);
          delay(500);
          analogWrite(R_LED_PIN, LED_LOW);
          analogWrite(G_LED_PIN, LED_LOW);
          analogWrite(B_LED_PIN, LED_LOW);
          delay(500);          
        }
      }
      else
      {
        is_available_tag_check = false;
        for (int i = 0; i < 3; i++) // 빨간색 3번 깜빡이기
        {
          analogWrite(R_LED_PIN, LED_HIGH);
          analogWrite(G_LED_PIN, LED_LOW);
          analogWrite(B_LED_PIN, LED_LOW);
          delay(500);
          analogWrite(R_LED_PIN, LED_LOW);
          analogWrite(G_LED_PIN, LED_LOW);
          analogWrite(B_LED_PIN, LED_LOW);
          delay(500);          
        }
      }
        analogWrite(R_LED_PIN, LED_HIGH);
        analogWrite(G_LED_PIN, LED_HIGH);
        analogWrite(B_LED_PIN, LED_HIGH);      
        is_available_tag_check = true;
    }
  }
  else
  {
    return;
  }
  delay(100);
}