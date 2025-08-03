#include "awsconnection.h"
#define SWITCH_COUNT 8
int switchPins[SWITCH_COUNT] = {5,17,16,15,13,2,19,18};

volatile bool switchStates[SWITCH_COUNT] = {false};
bool exec = false;

void IRAM_ATTR handleSwitchInterrupt(int index) {
    switchStates[index] = !switchStates[index]; // Toggle the state
    Serial.print("Switch ");
    Serial.print(index + 1);
    Serial.print(" state changed to: ");
    Serial.println(switchStates[index]);

    exec = true;
}
volatile bool switchTriggered[SWITCH_COUNT] = {false};

void IRAM_ATTR handleSwitchInterrupt0() { switchTriggered[0] = true; }
void IRAM_ATTR handleSwitchInterrupt1() { switchTriggered[1] = true; }
void IRAM_ATTR handleSwitchInterrupt2() { switchTriggered[2] = true; }
void IRAM_ATTR handleSwitchInterrupt3() { switchTriggered[3] = true; }
void IRAM_ATTR handleSwitchInterrupt4() { switchTriggered[4] = true; }
void IRAM_ATTR handleSwitchInterrupt5() { switchTriggered[5] = true; }
void IRAM_ATTR handleSwitchInterrupt6() { switchTriggered[6] = true; }
void IRAM_ATTR handleSwitchInterrupt7() { switchTriggered[7] = true; }


void configureSwitches() {
    for (int i = 0; i < SWITCH_COUNT; i++) {
        pinMode(switchPins[i], INPUT_PULLUP);
    }
}
void blink_twice()
{
  for (int i =0; i<5; i++){
  digitalWrite(l1, HIGH);
  digitalWrite(l2, HIGH);
  digitalWrite(l3, HIGH);
  digitalWrite(l4, HIGH);
  digitalWrite(l5, HIGH);
  digitalWrite(l6, HIGH);
  delay(1000);
  digitalWrite(l1, LOW);
  digitalWrite(l2, LOW);
  digitalWrite(l3, LOW);
  digitalWrite(l4, LOW);
  digitalWrite(l5, LOW);
  digitalWrite(l6, LOW);
  delay(1000);
}}
void setup()
{
  Serial.begin(115200);
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(l3, OUTPUT);
  pinMode(l4, OUTPUT);
  pinMode(l5, OUTPUT);
  pinMode(l6, OUTPUT);

  //blink_twice();
  
  Serial.println("WELCOME   ");
  // Serial.begin(115200);
  Serial.println("Slave ESP32 ready.");

  WiFi.mode(WIFI_STA);
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  preference.begin("wifi_config", false);
  bool isConfigured = preference.getBool("configured", false);
  String ssid = preference.getString("SSID");
  String pass = preference.getString("PASSWORD");
  preference.end();

  if (isConfigured && !ssid.isEmpty() && !pass.isEmpty()) {
    Serial.println("Device already configured. Connecting to saved WiFi...");
    connectToWiFi(ssid.c_str(), pass.c_str());
  } else {
    Serial.println(" Waiting for credentials via ESP-NOW...");
    initEspNow();
  }
  //Wifi_connect();
  configureSwitches();

  attachInterrupt(digitalPinToInterrupt(switchPins[0]), handleSwitchInterrupt0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(switchPins[1]), handleSwitchInterrupt1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(switchPins[2]), handleSwitchInterrupt2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(switchPins[3]), handleSwitchInterrupt3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(switchPins[4]), handleSwitchInterrupt4, CHANGE);
  attachInterrupt(digitalPinToInterrupt(switchPins[5]), handleSwitchInterrupt5, CHANGE)  ;
  attachInterrupt(digitalPinToInterrupt(switchPins[6]), handleSwitchInterrupt6, CHANGE)   ;
  attachInterrupt(digitalPinToInterrupt(switchPins[7]), handleSwitchInterrupt7, CHANGE)          ;



  AWSconnect();


  
  // publishMessage();
}
int val=0;
int wificount = 0;
bool wifidisconnected = false;
bool wificonnected = true;
void loop()
{
   // Handle reconnection if necessary
  if (WiFi.status() != WL_CONNECTED && millis() - lastCheck > 10000) {
    lastCheck = millis();
    reconnectAttempts++;

    Serial.printf("\n Reconnect Attempt %d/%d", reconnectAttempts, maxReconnects);

    preference.begin("wifi_config", false);
    String ssid = preference.getString("SSID");
    String pass = preference.getString("PASSWORD");
    preference.end();

    if (!ssid.isEmpty() && !pass.isEmpty()) {
      connectToWiFi(ssid.c_str(), pass.c_str());
    }

    if (reconnectAttempts >= maxReconnects) {
      Serial.println("\n⚠ Max retries reached. Resetting...");
      preference.begin("wifi_config", false);
      preference.clear();
      preference.end();
      ESP.restart();
    }
  }

  delay(1000);

  for (int i = 0; i < SWITCH_COUNT; i++) {
        if (switchTriggered[i]) {
            switchTriggered[i] = false; // Clear the flag
            int state = !digitalRead(switchPins[i]);
            Serial.print("Switch ");
            Serial.print(i + 1);
            Serial.print(" triggered. State: ");
            Serial.println(state);

            // Map switch to corresponding light
            switch (i) {
                case 0: 
                  // HandleDevice(l1, state); 
                  publishMessage("l1", state);
                  break;
                case 1: 
                  // HandleDevice(l2, state);                  
                  publishMessage("l2", state);                  
                  break;
                case 2: 
                  // HandleDevice(l3, state);                 
                  publishMessage("l3", state);
                  break;
                case 3: 
                  // HandleDevice(l4, state);                 
                  publishMessage("l4", state);
                  break;
                case 4: 
                  // HandleDevice(l5, state);                 
                  publishMessage("l5", state);
                  break;
                case 5: 
                  // HandleDevice(l6, state);                 
                  publishMessage("l6", state);
                  break;
                case 6: 
                  // HandleDevice(l7, state);                 
                  publishMessage("l7", state);
                  break;
                case 7: 
                  // HandleDevice(l8, state);                
                  publishMessage("l8", state); 
                  break;
                default: break;
            }
        }
    }

    // delay(10); // Avoid busy-waiting
    delay(1000);
    
  
  if(WiFi.status() == WL_CONNECTED)
  {
    wificonnected = true;
    wifidisconnected = false;
    if(!mqtt_client.connected())
  {
    AWSconnect();
  }
  }
  if(WiFi.status() != WL_CONNECTED)
  {
    wifidisconnected = true;
    wificonnected = false;
    Serial.println("WiFi disconnected, ATTEMPTING RECONNCETION");
    WiFi.begin(preference.getString("SSID"), preference.getString("PASSWORD")); 
    wificount++;
    delay(1000);
  

  }
  
  if(wifidisconnected == true && wificount >= 300)
  {
    Wifi_connect();
    AWSconnect();
    wificount = 0;

  }
  
  
  mqtt_client.loop();
}
