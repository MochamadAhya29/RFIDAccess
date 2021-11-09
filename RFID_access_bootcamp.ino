#define BLYNK_PRINT Serial

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <MFRC522.h>
#include <Servo.h>

char auth[] = "6WYp6PTFbFAy3UHkQUGBOHcXaT0_wd_t";
char ssid[] = "too";
char pass[] = "1234567890";

#define SDA_PIN D2
#define RST_PIN D1
#define motor D4

Servo servo;
MFRC522 mfrc522(SDA_PIN, RST_PIN);
WidgetLCD lcd(V1);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  SPI.begin();
  Blynk.begin(auth, ssid, pass);
  mfrc522.PCD_Init();
  pinMode(motor, OUTPUT);
  Serial.println("Approximate your card to the reader...");
  Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:

  Blynk.run();
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  } if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID tag : ");
  String content = "";
  byte letter;

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "" );
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "" ));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));

  }

  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();   // uid dijadikan huruf besar semua

  if (content.substring(1) == "04 EA F0 2B") {

    lcd.clear();
    lcd.print(0, 0, "UID for Card is");
    lcd.print(0, 1, content);
    delay(1000);
    buka(); // fungsi untuk menggerakkan servo

  } else {
    lcd.clear();
    lcd.print(0, 0, "Invalid Card");
    lcd.print(0, 1, "Access Denied");
  }

}

void buka() {
  servo.attach(motor);
  servo.write(90);
  delay(2000);
  servo.write(0);
}
