#include <CrsfSerial.h>
#include <WiFi.h>
#include <MQTT.h>
#include <RoboClaw.h>

// Initialize Hotspot values
RoboClaw roboclaw(&Serial2,10000);
#define address 0x80

const char ssid[] = "Bale-Bot";
const char pass[] = "$tructur3s-Computer";

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

  client.subscribe("Motors/FL");
  client.subscribe("Motors/FR");

  Serial.println("\nconnected!");
}

void messageReceived(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);
  int speed = payload.toInt();
  motor_speed(speed);
  Serial.print("Changing speed to: ");
  Serial.println(speed);
}


void motor_speed(int speed) {
  if (speed > 0)
    roboclaw.ForwardM1(address,speed);
  else
    roboclaw.BackwardM1(address,speed);
  //delay(2000);
}


void setup()
{
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