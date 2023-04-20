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


int hist[8];

/***
 * This callback is called whenever new channel values are available.
 * Use crsf.getChannel(x) to get us channel values (1-16).
 ***/
// void packetChannels1()
// {
//   for(int i = 1; i <= 8; i++)
//   {

//     // Serial.print("CH");
//     // Serial.print(i);
//     // Serial.print(":");
//     int data = crsf.getChannel(i);
//     // Serial.print(data);
//     // Serial.print(" | ");
//     if(crsf.getChannel(6) == 1500 && data != hist[i - 1])
//     {
//       client.publish("RC/CH" + String(i), String(data));
//     }
//     else if(crsf.getChannel(6) == 989)
//     {
//       client.publish("RC/CH" + String(i), String(data));
//     }
//     hist[i - 1] = data;
//   }
//   //Serial.println();
// }

void packetChannels()
{
  bool publish = false;
  String msg = "";
  for(int i = 1; i < 8; i++)
  {

    // Serial.print("CH");
    // Serial.print(i);
    // Serial.print(":");
    int data = crsf.getChannel(i);
    // Serial.print(data);
    // Serial.print(" | ");
    if(crsf.getChannel(6) == 1500 && data != hist[i - 1])
    {
      publish = true;
    }
    else if(crsf.getChannel(6) == 989)
    {
      publish = true;
    }
    msg += String(data) + ",";
    hist[i - 1] = data;
  }
  int data = crsf.getChannel(8);
  if(crsf.getChannel(6) == 1500 && data != hist[8 - 1])
    {
      publish = true;
    }
  else if(crsf.getChannel(6) == 989)
  {
    publish = true;
  }
  msg += String(data);
  hist[8 - 1] = data;

  //Serial.println();
  if(publish)
  {
    client.publish("RC/CH", msg);
  }
}

void setup()
{
  WiFi.begin(ssid, pass);

  //POTENTIAL PROBLEMS IF IP CHANGES
  client.begin("10.42.0.1", net);
    //Serial.begin(115200);

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