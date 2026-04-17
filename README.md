# ESP32 Security Hub with Blynk & Biometrics

This project is a multi-layered security system using an ESP32. It integrates a Touch Sensor as a master toggle, Biometric (Fingerprint) and RFID card scanning for access, and a Blynk IoT interface for remote control.

## Component Pinout
| Component | Pin Function | ESP32 Pin |
| :--- | :--- | :--- |
| **Relay 1** | Control | GPIO 25 |
| **Relay 2** | Control | GPIO 26 |
| **Touch Sensor** | Signal | GPIO 32 |
| **OLED (I2C)** | SDA / SCL | GPIO 21 / 22 |
| **Fingerprint** | RX / TX | GPIO 16 / 17 (Serial2) |
| **RFID (SPI)** | SS / RST | GPIO 5 / 4 |
| | SCK / MOSI / MISO | GPIO 18 / 23 / 19 |

## Libraries Required
- Adafruit SSD1306 & Adafruit GFX (OLED)
- Adafruit Fingerprint Sensor Library
- MFRC522 (RFID)
- Blynk (for the Blynk version)

## Usage
1. **Simple Version:** The system waits for a Fingerprint match or an RFID card. Once authorized, both relays turn ON for 5 seconds.
2. **Blynk Version:** Adds the ability to toggle Relay 1 and Relay 2 individually via the Blynk App (Virtual Pins V1 and V2) and provides status updates.
#define BLYNK_TEMPLATE_ID "TMPL6Rscc2EfI"
#define BLYNK_TEMPLATE_NAME "Bike Automation"
#define BLYNK_AUTH_TOKEN "HQVv8n4wc7lOrsHh3mV9hhkBCji59CEU"
