#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

#define PIN_SERVO 8
#define PIN_BUTTON 5

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

Servo myservo;
int buttonState = 0;

#define CARD_NUM 9
String uid[]={"FFFFFFFF","FFFFFFFF","FFFFFFFF","FFFFFFFF","FFFFFFFF","FFFFFFFF","FFFFFFFF","FFFFFFFF","FFFFFFFF"};

void setup()
{
  Serial.begin(9600);
  Serial.println("OK!");
  
  //RFID
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  
  //舵机
  myservo.attach(PIN_SERVO);
  myservo.write(45);
}

void open_door()
{
    myservo.write(150);
    delay(3000);
    myservo.write(45);
}

void button_down()
{
  if (analogRead(PIN_BUTTON)>1000){
    Serial.println("Button Down!");
    delay(500);
    open_door();
  }
}

String getuid()
{
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content; 
}

void access(){
  for (int i=0;i<CARD_NUM;i++)
  {
  if (getuid() == uid[i])
  {
    open_door();
    delay(200);
  }
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    if (Serial.readString()=="open")
    {
      open_door();
      delay(200);
    }
  }
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.println(getuid());
  access();
}
