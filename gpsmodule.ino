#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Wi-Fi Configuration
const char* ssid = "xxxx";
const char* password = "xxxx";

// Initialize Bot Token and Chat ID
#define BOTtoken "xxxx-xxxx"  // Bot Token from BotFather
#define CHAT_ID "xxxx" // Your Telegram Id from Userinfobot

// Initialize Telegram Bot
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// GPS Configuration
TinyGPSPlus gps;
SoftwareSerial GPSSerial(D5, D6);  // TX, RX

// Variables for Telegram messages
int botRequestDelay = 1000;
unsigned long lastTimeBotRan = 0;
float currentLat = 0.0;
float currentLng = 0.0;
bool gpsAvailable = false;

// Function to read GPS data
void readGPSData() {
  while (GPSSerial.available() > 0) {
    if (gps.encode(GPSSerial.read())) {
      if (gps.location.isValid()) {
        currentLat = gps.location.lat();
        currentLng = gps.location.lng();
        gpsAvailable = true;
      } else {
        gpsAvailable = false;
      }
    }
  }
}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages: " + String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    String text = bot.messages[i].text;
    Serial.println("Message from user: " + text);
    String from_name = bot.messages[i].from_name;

    // Command "/start"
    if (text == "/start") {
      String control = "Welcome, " + from_name + ".\n";
      control += "Use the commands below to monitor GPS location:\n\n";
      control += "/Location - Get the current location\n";
      bot.sendMessage(chat_id, control, "");
    }

    // Command "/Location"
    if (text == "/Location") {
      if (gpsAvailable) {
        // Format URL for Google Maps with pin
        String location = "Location: https://www.google.com/maps/search/?api=1&query=";
        location += String(currentLat, 6);
        location += ",";
        location += String(currentLng, 6);
        bot.sendMessage(chat_id, location, "");
      } else {
        bot.sendMessage(chat_id, "GPS not available or data is invalid.", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  GPSSerial.begin(9600);  // Initialize GPS Serial at 9600 baud

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setInsecure();  // ESP8266 does not support SSL certificates, use setInsecure

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send message to Telegram when ESP8266 is active
  bot.sendMessage(CHAT_ID, "Tracking bot is now active!", "");
}

void loop() {
  // Continuously read GPS data
  readGPSData();

  // Check for messages from bot at regular intervals
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("Receiving message from bot");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
