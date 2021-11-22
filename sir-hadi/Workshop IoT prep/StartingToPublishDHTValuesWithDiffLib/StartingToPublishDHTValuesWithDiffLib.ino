/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

// WIFI Library untuk dapat mengatur dan terkoneksi ke WIFI
#include <ESP8266WiFi.h>

// MQTT Library untuk terkoneksi dengan Broker MQTT dan melakukan
// aksi publish dan subcribe
#include <PubSubClient.h>

#include <dht.h>

#define DHTPIN D4
#define DHTTYPE DHT11 
dht DHT;


// Nama (ssid) dan Password wifi yang akan dikoneksikan NodeMCU
const char* ssid     = "*";
const char* password = "*";

// informasi Host dan Port MQTT broker yang akan digunakan
const char* broker = "broker.hivemq.com";
const uint16_t port = 1883;

const char* topic_suhu = "suhu_mcu";
const char* topic_kelempaban = "kelembapan_mcu";
const char* topic_random = "random_mcu";

WiFiClient espClient;
PubSubClient client(espClient);


void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Dimulai dari mengoneksikan ke WIFI
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.begin(ssid, password);

  //if wifi not connected LED is on
  
  digitalWrite(LED_BUILTIN, LOW);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Turning Off LED cause Wifi is connected
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Siapkan client MQTT
  client.setServer(broker, port);
  
  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str())){
        Serial.println("Connected to " + String(broker));
      }
    else{
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
  }
}

void loop() {
  
  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHTPIN);
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.print("OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    case DHTLIB_ERROR_CONNECT:
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        Serial.print("Ack High error,\t");
        break;
    default: 
    Serial.print("Unknown error,\t"); 
    break;
  }
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);

  client.publish(topic_suhu, String(DHT.temperature).c_str());
  client.publish(topic_kelempaban , String(DHT.humidity).c_str());
  client.publish(topic_random, String(random(10,100)).c_str());
  delay(1000);
  client.loop();
}
