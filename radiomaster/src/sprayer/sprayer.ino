#include <CrsfSerial.h>
#include <WiFi.h>
#include <MQTT.h>
#include <RoboClaw.h>

// Initialize Hotspot values
RoboClaw roboclaw(&Serial2,10000);
#define address 0x80

const char ssid[] = "Bale-Bot";
const char pass[] = "$tructur3s-Computer";
const int relay1 = 26;
const int relay2 = 25;
const int relay3 = 13;
const int relay4 = 12;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("")) {
    Serial.print(".");
    delay(1000);
  }

  client.subscribe("Sprayer");

  Serial.println("\nconnected!");
}

void messageReceived(String &topic, String &payload)
{
  //Serial.println("incoming: " + topic + " - " + payload);
  int num = payload.toInt();
  if(num & 1)
  {
    digitalWrite(relay1, HIGH);
    Serial.println("1");
  }
  else
  {
    digitalWrite(relay1, LOW);
    Serial.println("not 1");
  }
  if(num & 2)
  {
    digitalWrite(relay2, HIGH);
    Serial.println("2");
  }
  else
  {
    digitalWrite(relay2, LOW);
    Serial.println("not 2");
  }
  if(num & 4)
  {
    digitalWrite(relay3, HIGH);
    Serial.println("3");
  }
  else
  {
    digitalWrite(relay3, LOW);
    Serial.println("not 3");
  }
  if(num & 8)
  {
    digitalWrite(relay4, HIGH);
    Serial.println("4");
  }
  else
  {
    digitalWrite(relay4, LOW);
    Serial.println("not 4");
  }
  //Serial.print("Changing speed to: ");
  //Serial.println(speed);
}

void setup()
{
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  WiFi.begin(ssid, pass);

  //POTENTIAL PROBLEMS IF IP CHANGES
  Serial.begin(115200);
  client.begin("10.42.0.1", net);
  client.onMessage(messageReceived);
  connect();
  roboclaw.begin(38400);
}

void loop()
{
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
}