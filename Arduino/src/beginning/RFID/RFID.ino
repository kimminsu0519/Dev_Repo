#include <SPI.h>
#include <MFRC522.h>
#include <List.hpp>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

List<MFRC522::Uid> tag_list;
// const byte MY_TAG[4] = {0xCC, 0xF0, 0xC3, 0x01};

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

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

  delay(1000);
}




















