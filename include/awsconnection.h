
#ifndef AWSCONNECTION_h
#define AWSCONNECTION_h

#include <Arduino.h>
#include <WiFi.h>
#include "credentials.h"
#include <ArduinoJson.h>
#include <MQTTClient.h>
#include <WiFiClientSecure.h>
#include <cmath>
 #include <esp_now.h>
#include <Preferences.h>
#include "dimmable_light.h"
#include <unity.h>

bool wifiConnected = false;
unsigned long lastCheck = 0;
int reconnectAttempts = 0;
const int maxReconnects = 30;

// Struct for credentials
typedef struct struct_message {
  char ssid[32];
  char wifi_password[32];
} struct_message;

struct_message incomingCreds;

// MAC address of the master ESP32 - REPLACE WITH YOUR MASTER'S MAC ADDRESS
uint8_t masterMac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

// Forward declarations
void initEspNow();
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

Preferences preference;


#define PUB_TOPIC "$aws/things/Bennalkar_SmartHome/shadow/name/secondfloor/update"
#define DELTA_TOPIC "$aws/things/Bennalkar_SmartHome/shadow/name/secondfloor/update/delta"
#define ROOMNAME "living_room_1"

#define l1 14
#define l2 27
#define l3 26
#define l4 25
#define l5 33
#define l6 32

// DimmableLight l1(14); //W
// DimmableLight l2(27); //W
// DimmableLight l3(26); //W   //NOT WORKING-14,16
// DimmableLight l4(25); //W

// DimmableLight l5(33); //W
// DimmableLight l6(32); //W
// DimmableLight l7(22); //W
// DimmableLight l8(23); //W
const int syncPin = 4;

void reportMessage(const char *reportedPayload);
// void IRAM_ATTR handle_software_interrupt(){
WiFiClientSecure wifi_client = WiFiClientSecure();
MQTTClient mqtt_client = MQTTClient(2048);


void HandleDevice(int_least16_t &light, int state)//, int strength)
{
    
    
  if (state == 1)
  {
    // light.setBrightness(255);
    digitalWrite(light, HIGH);
    Serial.print("SETTING Light STATE : " );
    Serial.println("l :"+String(state));   
    
  }
  else if(state ==0)
  {
    // light.setBrightness(0);
      digitalWrite(light, LOW);
    Serial.print("SETTING Light STATE : " );
    Serial.println("l :"+String(state));
  }
//   if(strength >= 0)
//   {
    
//     strength = map(strength,1,5,95,255);
//     light.setBrightness(strength);
    
//     Serial.println("SETTING Light STRENGTH : " +String(strength));
//   }
//   else if(state == 0)
//   {
//     light.setBrightness(state);
//     Serial.println("SETTING Light STATE : " +String(state));
//   }
}

void IncomingMessageHandler(String &topic, String &payload) {
    Serial.print(F("incoming: "));
    Serial.println(topic);
    Serial.println(payload);

    StaticJsonDocument<2048> doc1;
    DeserializationError error = deserializeJson(doc1, payload);
    if (error) {
        Serial.println("ERROR DESERIALIZING:");
        return;
    }

    for (JsonPair kv : doc1["state"].as<JsonObject>()) {
        String room = kv.key().c_str();
        JsonObject roomState = kv.value().as<JsonObject>();

        Serial.println("Room: " + String(room));

        if (room == ROOMNAME) {
            for (JsonPair device : roomState) {
                
                String deviceName = device.key().c_str();
                int deviceState = device.value().as<int>();

                if (deviceName.startsWith("l")) {
                    int lightNumber = deviceName.substring(1).toInt();
                    // int state = deviceState.toInt();
                    
                    // int state = deviceState["state"].isNull() ? -1 : deviceState["state"].as<int>();
                    // int strength = deviceState["strength"].isNull() ? -1 : deviceState["strength"].as<int>();

                    // Serial.println("LIGHT NO: " + String(lightNumber) + "  DEVICE STATE : " + String(state) + "  STRENGTH : " + String(strength));

                    switch (lightNumber) {
                        case 1:
                            // if (state != -1 || strength != -1) {
                                // myData.light1_state = state;
                                // myData.light1_strength = map(strength, 1, 5, 1, 255);
                                // HandleDevice(l1, deviceState);//, state, strength);
                                digitalWrite(l1, deviceState);
                            // }
                            break;
                        case 2:
                            // if (state != -1 || strength != -1) {
                                // myData.light2_state = state;
                                // myData.light2_strength = map(strength, 1, 5, 1, 255);
                                // HandleDevice(l2,deviceState);//, state, strength);
                                digitalWrite(l2, deviceState);
                            // }
                            break;
                        case 3:
                            // if (state != -1 || strength != -1) {
                                // myData.light3_state = state;
                                // myData.light3_strength = map(strength, 1, 5, 1, 255);
                                // HandleDevice(l3,deviceState);//, state, strength);
                                digitalWrite(l3, deviceState);
                            // }
                            break;
                        case 4:
                            // if (state != -1 || strength != -1) {
                                // myData.light4_state = state;
                                // myData.light4_strength = map(strength, 1, 5, 1, 255);
                                // HandleDevice(l4,deviceState);//, state, strength);
                                digitalWrite(l4, deviceState);
                            // }
                            break;
                        case 5:
                            // if (state != -1 || strength != -1) {
                                // myData.light5_state = state;
                                // myData.light5_strength = map(strength, 1, 5, 1, 255);
                                // HandleDevice(l5,deviceState);//, state, strength);
                                digitalWrite(l5, deviceState);
                            // }
                            break;
                        case 6:
                            // if (state != -1 || strength != -1) {
                                // myData.light6_state = state;
                                // myData.light6_strength = map(strength, 1, 5, 1, 255);
                                // HandleDevice(l6,deviceState);//, state, strength);
                                digitalWrite(l6, deviceState);
                            // }
                            break;
                        case 7:
                            // if (state != -1 || strength != -1) {
                                // myData.light7_state = state;
                                // myData.light7_strength = map(strength, 1, 5, 1, 255);
                                // HandleDevice(l7,deviceState);//, state, strength);
                            // }
                            break;
                        case 8:
                            // if (state != -1 || strength != -1) {
                                // myData.light8_state = state;
                                // myData.light8_strength = map(strength, 1, 5, 1, 255);
                                // HandleDevice(l8,deviceState);//, state, strength);
                            // }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    doc1.clear();
    reportMessage(payload.c_str());
}

int count;

void Wifi_connect()
{
  preference.begin("wifi_creds",false);

  bool isCredStored = preference.getBool("isCredStored");
  // while (isCredStored == true && sentdata==false)
  // {
  //   // ESP-NOW WORK
    
  //     WiFi.mode(WIFI_STA);
  //     Serial.println("====================CHECKING STORED SETTINGS=========================");
  //     delay(2000);
  //     Serial.println(preference.getString("SSID"));
  //     Serial.println(preference.getString("PASSWORD"));
  //     // Serial.println("WIFI CHANNEL == " +String(WiFi.channel()));
      

      

  // }  
//=================================================================================================================================================================================
  delay(500);
  if(isCredStored == false){
  Serial.println("Connecting to WIFI");
 

  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   Serial.print(".");
  //   delay(50);
  // }

  // WiFi.mode(WIFI_STA);
  if(WiFi.status() == WL_CONNECTED && isCredStored == false)
  {
    Serial.println(F("WiFi Connected"));
    Serial.println(WiFi.SSID());
    Serial.println(WiFi.psk());
    preference.putString("SSID", WiFi.SSID());
    preference.putString("PASSWORD", WiFi.psk());
    preference.putBool("isCredStored", true);
    Serial.println("\n\n RESTARTING !!!!\n\n");
    // ESP.restart();
  }
  }
    

  delay(2000);
  

        // Initialize slave status
        // for (int i = 0; i < slaveCount; i++) {
        //     memcpy(slaves[i].mac, slaveMACs[i], 6);
        //     slaves[i].ackReceived = false;
        //     delay(100);
        // }

        // sendCredentialsToAllSlaves();
    //  else {
    //     Serial.println("ESP-NOW initialization failed.");
    // }
  DimmableLight::setSyncPin(syncPin );
  DimmableLight::begin();
  // Serial.println(F(WiFi.localIP()));
  

   
}
void connectToWiFi(const char* ssid, const char* password) {
  esp_now_deinit(); // Disable ESP-NOW before Wi-Fi connection
  
  WiFi.mode(WIFI_STA);
  Serial.printf("\n Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 20) {
    delay(500);
    Serial.print("...");
    tries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n WiFi Connected! IP: " + WiFi.localIP().toString());
    wifiConnected = true;
    reconnectAttempts = 0;

    // Save configured flag
    preference.begin("wifi_config", false);
    preference.putBool("configured", true);
    preference.end();
       pinMode(2, OUTPUT);  // Set D2 pin as output
    digitalWrite(2, HIGH);  // Set D2 pin HIGH
    Serial.println("Pin D2 is now HIGH.");
  } else {
    Serial.println("\n Connection Failed");
    wifiConnected = false;
    initEspNow(); // Reinit ESP-NOW on failure
  }
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  preference.begin("wifi_config", false);
  bool isConfigured = preference.getBool("configured", false);
  preference.end();

  // If already configured, ignore new credentials
  if (isConfigured) {
    Serial.println(" Credentials already configured. Ignoring new data.");
    return;
  }

  // Copy incoming credentials
  memcpy(&incomingCreds, incomingData, sizeof(incomingCreds));

  Serial.println("\n=== Received Credentials from Master ===");
  Serial.printf("SSID: %s\n", incomingCreds.ssid);
  Serial.printf("PASS: %s\n", incomingCreds.wifi_password);

  // Store credentials in preferences
  preference.begin("wifi_config", false);
  preference.putString("SSID", incomingCreds.ssid);
  preference.putString("PASSWORD", incomingCreds.wifi_password);
  preference.end();

  // Try to connect to Wi-Fi with received credentials
  connectToWiFi(incomingCreds.ssid, incomingCreds.wifi_password);
}

void initEspNow() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println(" ESP-NOW Init Failed");
    return;
  }

  Serial.println(" ESP-NOW initialized");

  esp_now_peer_info_t masterPeer = {};
  memcpy(masterPeer.peer_addr, masterMac, 6);
  masterPeer.channel = 0;
  masterPeer.encrypt = false;

  if (!esp_now_is_peer_exist(masterMac)) {
    if (esp_now_add_peer(&masterPeer) == ESP_OK) {
      Serial.println(" Master peer added successfully!");
    } else {
      Serial.println(" Failed to add master as peer");
    }
  }

  esp_now_register_recv_cb(OnDataRecv);
}
void AWSconnect()
{

  wifi_client.setCACert(ROOT_CA);
  wifi_client.setCertificate(CERTFICATE_KEY);
  wifi_client.setPrivateKey(PRIVATE_KEY);
  mqtt_client.begin(endpoint, 8883, wifi_client);
  mqtt_client.onMessage(IncomingMessageHandler);
  Serial.println(F("connecting to AWS"));
  mqtt_client.connect(THINGNAME);
  
  if(!mqtt_client.connect(THINGNAME));
  {
    Serial.print("*");
    delay(50);
  }
  Serial.print("connected To AWS");

//   if (!mqtt_client.connected())
//   {
//     Serial.println(F("Connection Timeout"));
//   }
  mqtt_client.subscribe(DELTA_TOPIC);
}

void publishMessage(String light, int state)
{
  /*
   * we have created Json object and stores the data recived from slave board
   * {"state" : {"reported":{"room1:{"light1: "ON","light2":"OFF"}","rooom2":{"light21":"OFF","light22":"OFF"}}}}   // json format
   * and then we have serialize the json object before sending it to aws
   */
  StaticJsonDocument<2048> doc;
  JsonObject stateobj = doc.createNestedObject("state");
  JsonObject desiredobj = stateobj.createNestedObject("desired");
  JsonObject roomobj = desiredobj.createNestedObject(ROOMNAME);
  
  roomobj[light] = state;
  
    Serial.println("\n\n===========");
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);             // serializing doc json object into jsonbuffer
  mqtt_client.publish(PUB_TOPIC, jsonBuffer); // updating device shadow (Default topic defined above )
  Serial.println(F(jsonBuffer));
//   mqtt_client.publish(PUB_TOPIC, jsonBuffer); // updating defined topic
  Serial.println(F("Sent a message"));
  doc.clear();
  Serial.println("==========\n\n");
}

void reportMessage(const char *reportedPayload)
{
  /**

   * we have created Json object and stores the data recived from slave board
   * {"state" : {"reported":{"room1:{"light1: "ON","light2":"OFF"}","rooom2":{"light21":"OFF","light22":"OFF"}}}}   // json format
   * and then we have serialize the json object before sending it to aws
   */
  StaticJsonDocument<2048> incomingDoc;
  DeserializationError err = deserializeJson(incomingDoc, reportedPayload);
  if (err)
  {
    Serial.print(F("\nDeserialize failed in reported !!:  "));
    Serial.println(F(err.c_str()));
    return;
  }
  DynamicJsonDocument reportedDoc(2048);

  JsonObject state = reportedDoc.createNestedObject("state");
  // JsonObject reported = state.createNestedObject("reported");
  state["reported"] = incomingDoc["state"];

  char shadowPayload[512];
  serializeJson(reportedDoc, shadowPayload);
  // Serial.println(shadowPayload);
  mqtt_client.publish(PUB_TOPIC, shadowPayload);

  incomingDoc.clear();
  reportedDoc.clear();
  Serial.println(F("\n\nREPORtED SENT\n\n"));
}

#endif