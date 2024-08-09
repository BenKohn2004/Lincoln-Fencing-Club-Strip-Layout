#include <ESP8266WiFi.h>
#include <espnow.h>

// Define strip names and LED pins
const char* STRIP_1_NAME = "Strip 2";  // Red Light
const char* STRIP_2_NAME = "Strip 1";  // Green Light
#define LED_PIN_STRIP1 D7
#define LED_PIN_STRIP2 D2

// Enable or disable verbose output
bool verbose = false;

// Structure to hold the incoming message
struct struct_message {
  uint8_t msgType;
  uint8_t macAddr[6];
  unsigned int Right_Score;
  unsigned int Left_Score;
  unsigned int Seconds_Remaining;
  unsigned int Minutes_Remaining;
  bool Green_Light;
  bool Red_Light;
  bool White_Green_Light;
  bool White_Red_Light;
  bool Yellow_Green_Light;
  bool Yellow_Red_Light;
  bool Yellow_Card_Green;
  bool Yellow_Card_Red;
  bool Red_Card_Green;
  bool Red_Card_Red;
  bool Priority_Left;
  bool Priority_Right;
  char customMessage[32];
};

struct_message incomingMessage;

void setup() {
  Serial.begin(9600);

  // Initialize the LED pins
  pinMode(LED_PIN_STRIP1, OUTPUT);
  digitalWrite(LED_PIN_STRIP1, LOW);  // Start with the LED off
  pinMode(LED_PIN_STRIP2, OUTPUT);
  digitalWrite(LED_PIN_STRIP2, LOW);  // Start with the LED off

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    if (verbose) Serial.println("ESP-NOW initialization failed");
    return;
  }

  // Register the receive callback function
  esp_now_register_recv_cb(OnDataRecv);

  if (verbose) Serial.println("ESP-NOW Initialized. Waiting for messages...");
}

void loop() {
  // Nothing needed in loop
}

void OnDataRecv(uint8_t* mac, uint8_t* incomingData, uint8_t len) {
  // Copy incoming data into our structure
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

  // Check if the custom message (transmitter name) matches Strip 1
  if (String(incomingMessage.customMessage) == STRIP_1_NAME) {
    if (verbose) {
      // Print the entire message
      Serial.print("Message from ");
      Serial.println(STRIP_1_NAME);
      Serial.print("Right Score: ");
      Serial.println(incomingMessage.Right_Score);
      Serial.print("Left Score: ");
      Serial.println(incomingMessage.Left_Score);
      Serial.print("Time Remaining: ");
      Serial.print(incomingMessage.Minutes_Remaining);
      Serial.print(":");
      Serial.println(incomingMessage.Seconds_Remaining);
      Serial.print("Green Light: ");
      Serial.println(incomingMessage.Green_Light ? "ON" : "OFF");
      Serial.print("Red Light: ");
      Serial.println(incomingMessage.Red_Light ? "ON" : "OFF");
    }

    // Control the LED for Strip 1 based on the Red Light status
    if (incomingMessage.Red_Light) {
      if (verbose) {
        Serial.print("Activating ");
        Serial.print(STRIP_1_NAME);
        Serial.println(" Light");
      }
      digitalWrite(LED_PIN_STRIP1, LOW);  // Turn on the LED
    } else {
      digitalWrite(LED_PIN_STRIP1, HIGH);  // Turn off the LED
    }

    if (verbose) Serial.println();
  }

  // Check if the custom message (transmitter name) matches Strip 2
  if (String(incomingMessage.customMessage) == STRIP_2_NAME) {
    if (verbose) {
      // Print the entire message
      Serial.print("Message from ");
      Serial.println(STRIP_2_NAME);
      Serial.print("Right Score: ");
      Serial.println(incomingMessage.Right_Score);
      Serial.print("Left Score: ");
      Serial.println(incomingMessage.Left_Score);
      Serial.print("Time Remaining: ");
      Serial.print(incomingMessage.Minutes_Remaining);
      Serial.print(":");
      Serial.println(incomingMessage.Seconds_Remaining);
      Serial.print("Green Light: ");
      Serial.println(incomingMessage.Green_Light ? "ON" : "OFF");
      Serial.print("Red Light: ");
      Serial.println(incomingMessage.Red_Light ? "ON" : "OFF");
    }

    // Control the LED for Strip 2 based on the Green Light status
    if (incomingMessage.Green_Light) {
      if (verbose) Serial.println("Activating Strip 2 Light");
      digitalWrite(LED_PIN_STRIP2, LOW);  // Turn on the LED
    } else {
      digitalWrite(LED_PIN_STRIP2, HIGH);  // Turn off the LED
    }

    if (verbose) Serial.println();
  }
}
