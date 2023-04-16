#include <CrsfSerial.h>
#include <WiFi.h>
#include <MQTT.h>

// Initialize Hotspot values
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

  Serial.println("\nconnected!");
}


// Pass any HardwareSerial port
// "Arduino" users (atmega328) can not use CRSF_BAUDRATE, as the atmega does not support it
// and should pass 250000, but then also must flash the receiver with RCVR_UART_BAUD=250000
// Also note the atmega only has one Serial, so logging to Serial must be removed
CrsfSerial crsf(Serial1, CRSF_BAUDRATE);

/***
 * This callback is called whenever new channel values are available.
 * Use crsf.getChannel(x) to get us channel values (1-16).
 ***/
void packetChannels()
{
  for(int i = 1; i <= 8; i++)
  {

    Serial.print("CH");
    Serial.print(i);
    Serial.print(":");
    int data = crsf.getChannel(i);
    Serial.print(data);
    Serial.print(" | ");
    client.publish("RC/CH" + String(i), String(data));
  }
  Serial.println();
}

void setup()
{
  WiFi.begin(ssid, pass);
  client.begin("10.42.0.1", net);
    Serial.begin(115200);

    // If something other than changing the baud of the UART needs to be done, do it here
    // Serial1.end(); Serial1.begin(500000, SERIAL_8N1, 16, 17);

    // Attach the channels callback
    crsf.onPacketChannels = &packetChannels;
}

void loop()
{
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
    // Must call CrsfSerial.loop() in loop() to process data
  crsf.loop();
}