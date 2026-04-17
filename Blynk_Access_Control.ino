#define BLYNK_TEMPLATE_ID "TMPL_XXXX"
#define BLYNK_DEVICE_NAME "SecurityHub"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Auth & WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASS";

// Pins
const int TOUCH_PIN = 32;
const int RELAY1 = 25;
const int RELAY2 = 26;
#define SS_PIN  5
#define RST_PIN 4

// Instances
Adafruit_SSD1306 display(128, 64, &Wire, -1);
MFRC522 rfid(SS_PIN, RST_PIN);
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Blynk Virtual Pins for Remote Toggle
BLYNK_WRITE(V1) { digitalWrite(RELAY1, param.asInt()); }
BLYNK_WRITE(V2) { digitalWrite(RELAY2, param.asInt()); }

void setup() {
  pinMode(TOUCH_PIN, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  SPI.begin();
  rfid.PCD_Init();
  finger.begin(57600);
  
  Blynk.begin(auth, ssid, pass);
  updateDisplay("Blynk Online", "Ready");
}

void loop() {
  Blynk.run();

  if (digitalRead(TOUCH_PIN) == HIGH) {
    bool access = false;

    if (getFingerprintID() != -1) { access = true; }
    if (!access && rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
       access = true;
       rfid.PICC_HaltA();
    }

    if (access) {
      Blynk.logEvent("access_granted", "Security triggered!");
      digitalWrite(RELAY1, HIGH);
      digitalWrite(RELAY2, HIGH);
      updateDisplay("ACCESS GRANTED", "Relays ON");
      delay(5000);
      digitalWrite(RELAY1, LOW);
      digitalWrite(RELAY2, LOW);
      updateDisplay("System Locked", "Waiting...");
    }
  }
}

void updateDisplay(String l1, String l2) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.setTextSize(1);
  display.println(l1);
  display.setCursor(0,30);
  display.setTextSize(1);
  display.println(l2);
  display.display();
}

int getFingerprintID() {
  if (finger.getImage() != FINGERPRINT_OK) return -1;
  if (finger.image2Tz() != FINGERPRINT_OK) return -1;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) return -1;
  return finger.fingerID;
}
