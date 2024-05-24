#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9

void releaseKey();
void pressKey(uint8_t key);

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

uint8_t buf[8] = {0}; /* Keyboard report buffer */

int cardCount = 0; 
bool isLocked = true;

void setup() 
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  delay(200);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
 

}
void loop(){
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  String content= "";
  byte letter;
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  content.toUpperCase();
  
  if (content.substring(1) == "39 36 F0 7A" , "26 42 E6 AD" ) //change here the UID of the card/cards that you want to give access
  { 
    if(!isLocked)
    {
      pressKey(0xE3, 0x0F); // win+L keys
      
      delay(2000);
      
      isLocked = true;
    }
    else
    {
        delay(50);

        pressKey(0x2A,0); // Dummy key to bypass lock screen with no input field

        delay(150);

        pressKey(0x00,0); // password letter

        pressKey(0x00,0); // password letter

        pressKey(0x00,0); // password letter

        pressKey(0x00,0); // password letter

        delay(150);

        pressKey(0x28,0); // enter press
        
        delay(2000);

        isLocked = false;

        cardCount++;

        delay(500);
      }    
  }      
  else
  {
   return;
  }
              
}

void pressKey(uint8_t key, uint8_t key2)
{
  delay(50);

  buf[0] = 0;
  buf[2] = key;
  buf[3] = key2;
  Serial.write(buf, 8);
  releaseKey();

  delay(50);
}

void releaseKey()
{
  buf[0] = 0;
  buf[2] = 0;
  buf[3] = 0;
  Serial.write(buf, 8); // Release key
}