#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Kaveesh's S10"
#define WIFI_PASSWORD "01234567"
// Insert Firebase project API Key
#define API_KEY "API KEY"
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "DATABASE URL/"

#define TRIG_PIN 12 // Replace with your TRIG pin number
#define ECHO_PIN 14 // Replace with your ECHO pin number
#define TRIG_PIN2 32 // Replace with your TRIG pin number
#define ECHO_PIN2 35 // Replace with your ECHO pin number

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

bool signupOK = false;
bool wp_status = false;
bool wp_status2 = false;
bool wp_status3 = false;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  long duration1, distance1;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration1 = pulseIn(ECHO_PIN, HIGH);
  distance1 = (duration1 * 0.0343) / 2;

  // Send ultrasonic sensor 1 data to Firebase Realtime Database
  Firebase.RTDB.setInt(&fbdo, "ULTRASONIC_DISTANCE", distance1);

  Serial.println("Distance 1: " + String(distance1));

  long duration2, distance2;
  digitalWrite(TRIG_PIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN2, LOW);
  duration2 = pulseIn(ECHO_PIN2, HIGH);
  distance2 = (duration2 * 0.0343) / 2;

  // Send ultrasonic sensor 2 data to Firebase Realtime Database
  Firebase.RTDB.setInt(&fbdo, "ULTRASONIC_DISTANCE2", distance2);

  Serial.println("Distance 2: " + String(distance2));

  

    if (Firebase.RTDB.getBool(&fbdo, "WP/digital")) {
      if (fbdo.dataType() == "boolean") {
        wp_status = fbdo.boolData();
        digitalWrite(2, wp_status);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getBool(&fbdo, "WP2/digital")) {
      if (fbdo.dataType() == "boolean") {
        wp_status2 = fbdo.boolData();
        digitalWrite(4, wp_status2);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.getBool(&fbdo, "WP3/digital")) {
      if (fbdo.dataType() == "boolean") {
        wp_status3 = fbdo.boolData();
        digitalWrite(5, wp_status3);
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
  
}
