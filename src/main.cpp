//
//  920MHz LoRa/FSK RF module ES920LR3
//  Firmware updater
//   for M5Stack
//
//  MaiaR Create

#include <Arduino.h>
#include <M5Stack.h>

#define RX_pin 16
#define TX_pin 17

int boot_pin = 22;
int reset_pin = 13;

void LoRa_Reset() {
  pinMode(reset_pin, OUTPUT);
  digitalWrite(reset_pin, LOW); // NRST "L"
  delay(10);
  pinMode(reset_pin, INPUT); // NRST open
  delay(1);
}

void Command(String s) {
  String rx;
  Serial2.print(s);
  Serial2.print("\r\n");
  Serial2.flush();
  delay(10);
}

void bootmode() {
  M5.lcd.fillRect(0, 70, 320, 80, BLACK);

  pinMode(boot_pin, OUTPUT);
  digitalWrite(boot_pin, HIGH); // boot mode
  LoRa_Reset();

  M5.Lcd.setTextSize(1);
  M5.Lcd.drawCentreString("Firmware update", 160, 70, 4);

  Serial.end();
  Serial2.end();
  Serial.begin(115200, SERIAL_8E1);
  Serial2.begin(115200, SERIAL_8E1, RX_pin, TX_pin);
}

void normalmode() {
  String rx;
  M5.lcd.fillRect(0, 70, 320, 30, BLACK);

  pinMode(boot_pin, OUTPUT);
  digitalWrite(boot_pin, LOW); // normal mode
  LoRa_Reset();

  M5.Lcd.setTextSize(1);
  M5.Lcd.drawCentreString("    version check    ", 160, 70, 4);

  delay(100);
  Serial.end();
  Serial2.end();
  Serial2.setTimeout(50);
  Serial.begin(115200, SERIAL_8N1);
  Serial2.begin(115200, SERIAL_8N1, RX_pin, TX_pin);

  delay(100);
  Command("2");
  rx = Serial2.readString();

  while (1) {
    Command("v");
    rx = Serial2.readString();
    M5.Lcd.drawCentreString(rx, 160, 120, 4);

    M5.BtnA.read();
    if (M5.BtnA.wasPressed())
      break;
  }
  bootmode();
}

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.drawCentreString("ES920LR3", 160, 20, 4);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawCentreString("update", 65, 220, 1);
  M5.Lcd.drawCentreString("check", 160, 220, 1);
  bootmode();
}

void loop() {
  M5.BtnB.read();
  if (M5.BtnB.wasPressed()) {
    normalmode();
  }

  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }
  if (Serial.available()) {
    Serial2.write(Serial.read());
  }
}
