//
//  920MHz LoRa/FSK RF module ES920LR3
//  Firmware updater
//   for M5Stack Core2
//
//  MaiaR Create

#include <Arduino.h>
#include <M5Unified.h>

// @see https://ikkei.akiba.coocan.jp/ikkei_Electronics/M5LR3.html
#define RX_pin 13
#define TX_pin 14

int boot_pin = 22;
int reset_pin = 19;

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
  M5.Display.fillRect(0, 70, 320, 80, BLACK);

  pinMode(boot_pin, OUTPUT);
  digitalWrite(boot_pin, HIGH); // boot mode
  LoRa_Reset();

  M5.Display.setTextSize(1);
  M5.Display.setTextDatum(middle_center);
  M5.Display.drawString("Firmware update", 160, 70);

  Serial.end();
  Serial2.end();
  Serial.begin(115200, SERIAL_8E1);
  Serial2.begin(115200, SERIAL_8E1, RX_pin, TX_pin);
}

void normalmode() {
  String rx;
  M5.Display.fillRect(0, 70, 320, 30, BLACK);

  pinMode(boot_pin, OUTPUT);
  digitalWrite(boot_pin, LOW); // normal mode
  LoRa_Reset();

  M5.Display.setTextSize(1);
  M5.Display.setTextDatum(middle_center);
  M5.Display.drawString("    version check    ", 160, 70);

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
    M5.update();
    Command("v");
    rx = Serial2.readString();
    M5.Display.drawString(rx, 160, 120);

    if (M5.BtnA.wasPressed())
      break;
    delay(10);
  }
  bootmode();
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.setTextSize(1);
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextDatum(middle_center);
  M5.Display.drawString("ES920LR3", 160, 20);
  M5.Display.setTextSize(2);
  M5.Display.drawString("update", 65, 220);
  M5.Display.drawString("check", 160, 220);
  bootmode();
}

void loop() {
  M5.update();
  if (M5.BtnB.wasPressed()) {
    normalmode();
  }

  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }
  if (Serial.available()) {
    Serial2.write(Serial.read());
  }
  delay(1);
}
