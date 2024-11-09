# ESP8266-GPS-Tracker-Telegram

This tutorial demonstrates how to use the GPS module to send location data (latitude and longitude) to Telegram as a Google Maps link.

### Hardware Setup
1. **Connect the VCC pin** of the GPS module to the **VV** pin of the ESP8266.
2. **Connect the ground (GND) pin** of the GPS module to the **GND** pin of the ESP8266.
3. **Connect the RX pin** of the GPS module to the **RX pin (D6)** of the ESP8266.
4. **Connect the TX pin** of the GPS module to the **TX pin (D5)** of the ESP8266.

### Libraries Required
- `WiFiClient`
- `TinyGPSPlus`
- `SoftwareSerial`
- `UniversalTelegramBot`

### Telegram Setup
1. Search for BotFather on Telegram to create a custom bot and get your bot API token.
2. Use userinfobot on Telegram to get your Telegram ID and copy it.
