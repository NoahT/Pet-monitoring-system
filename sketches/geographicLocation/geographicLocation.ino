#include <SPI.h>
#include <WifiLocation.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#define ARDUINOJSON_USE_DOUBLE 1
#include <ArduinoJson.h>

const char* API_KEY = "GEOLOCATION API KEY";
const char* SS_ID = "network name";
const char* PASSWORD = "password for SSID";
const char HOSTNAME[] = "http(s) server";
const char ROUTE[] = "/arduino";
const int capacity = JSON_OBJECT_SIZE(32);
const int responseCapacity = JSON_OBJECT_SIZE(32);


StaticJsonDocument<capacity> doc;
StaticJsonDocument<responseCapacity> responseDoc;


WifiLocation location(API_KEY);
WiFiClient client;
HTTPClient http;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SS_ID, PASSWORD);
  connectToSSID();
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

/**
 * Data massaging layer. Converts struct for
 * latitude, longitude, and accuracy to a JSON
 * object.
 */
void formatToJSON(location_t locationData) {
  doc["accuracy"] = locationData.accuracy;
  JsonObject location = doc.createNestedObject("location");
  location["lat"] = locationData.lat;
  location["lng"] = locationData.lon;
}

/**
 * Method to deserialize the POST request response.
 * @returns boolean determining whether the response was
 * successfully deserialized.
 */
boolean deserializePayload(const String& payload) {
  
  DeserializationError err = deserializeJson(responseDoc, payload);
  if (err) {
    return false;
  }
  return true;
}

/**
 * Subroutine/minor business logic for connecting to SSID
 */
void connectToSSID() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void loop() {
  formatToJSON(location.getGeoFromWiFi());
  String serializedLocationData;
  serializeJson(doc, serializedLocationData);

  http.begin(client, String(HOSTNAME) + String(ROUTE));
  http.addHeader("Content-Type", "application/json");
  int status = http.POST(serializedLocationData);
  
  if (status == 200) {
    const String& payload = http.getString();
    DeserializationError err = deserializeJson(responseDoc, payload);
    if (err) {
      return;
    } else {
      if (responseDoc["escape"] == true) {
        digitalWrite(LED_BUILTIN, LOW);
      } else {
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
  } else {
  }
}


