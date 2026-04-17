#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pins
const int TOUCH_PIN = 32;
const int RELAY1 = 25;
const int RELAY2 = 26;
#define SS_PIN  5
#define RST_PIN 4
#define FP_RX 16
#define FP_TX 17

// Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Sensor Instances
MFRC522 rfid(SS_PIN, RST_PIN);
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY1, LOW); 
  digitalWrite(RELAY2, LOW);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Initializing...");
  display.display();

  SPI.begin();
  rfid.PCD_Init();
  finger.begin(57600);
}

void loop() {
  // Check Touch Sensor - Act as a "Wake" or "Power" check
  if (digitalRead(TOUCH_PIN) == HIGH) {
    updateDisplay("Status: ACTIVE", "Waiting for Scan");

    bool access = false;

    // Check Fingerprint
    if (getFingerprintID() != -1) {
      access = true;
      updateDisplay("ACCESS GRANTED", "Finger Match");
    }

    // Check RFID
    if (!access && rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      access = true;
      updateDisplay("ACCESS GRANTED", "Card Match");
      rfid.PICC_HaltA();
    }

    if (access) {
      digitalWrite(RELAY1, HIGH);
      digitalWrite(RELAY2, HIGH);
      delay(5000);
      digitalWrite(RELAY1, LOW);
      digitalWrite(RELAY2, LOW);
      updateDisplay("System Locked", "Waiting...");
    }
  } else {
    updateDisplay("System SLEEP", "Touch to Wake");
  }
}

void updateDisplay(String line1, String line2) {
  display.clearDisplay();
  display.setCursor(0,10);
  display.setTextSize(1);
  display.println(line1);
  display.setCursor(0,30);
  display.setTextSize(2);
  display.println(line2);
  display.display();
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;
  return finger.fingerID;
}
