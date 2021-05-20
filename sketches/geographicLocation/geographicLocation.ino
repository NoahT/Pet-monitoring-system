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
const char HOSTNAME[] = "ngrok http server";
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
//  Serial.begin(115200);
  // Clear any misc. data previously serialized
//  Serial.println();
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
//  Serial.printf("Latitude: %f", locationData.lat);
  location["lng"] = locationData.lon;
//  Serial.printf("Longitude: %f", locationData.lon);
}

/**
 * Method to deserialize the POST request response.
 * @returns boolean determining whether the response was
 * successfully deserialized.
 */
boolean deserializePayload(const String& payload) {
  
  DeserializationError err = deserializeJson(responseDoc, payload);
  if (err) {
//    Serial.printf("deserializeJson() failed: %s\n", err.f_str());
    return false;
  }
  return true;
}

/**
 * Subroutine/minor business logic for connecting to SSID
 */
void connectToSSID() {
//  Serial.printf("Connecting to %s", SS_ID);
  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
    delay(1000);
  }
//  Serial.println("\nConnected!");
}

void loop() {
//  Serial.println("Getting geographical data..");
  formatToJSON(location.getGeoFromWiFi());
  String serializedLocationData;
  serializeJson(doc, serializedLocationData);

//  Serial.println("Beginning connection to " + (String(HOSTNAME) + String(ROUTE)));

  http.begin(client, String(HOSTNAME) + String(ROUTE));
  http.addHeader("Content-Type", "application/json");
//  Serial.println("Beginning POST..");
  int status = http.POST(serializedLocationData);
  
  if (status == 200) {
    const String& payload = http.getString();
//    Serial.println("Success!");
//    Serial.println(payload);
    DeserializationError err = deserializeJson(responseDoc, payload);
    if (err) {
//      Serial.printf("deserializeJson() failed: %s\n", err.f_str());
      return;
    } else {
      if (responseDoc["escape"] == true) {
//        Serial.println("Blinking.");
        digitalWrite(LED_BUILTIN, LOW);
      } else {
//        Serial.println("Not blinking.");
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
  } else {
//    Serial.printf("Something went wrong.. received response code %d\n", status);
  }
}
