// WIFI Library untuk dapat mengatur dan terkoneksi ke WIFI
#include <ESP8266WiFi.h>

// Library untuk sensor kita
#include "DHT.h"

// MQTT Library untuk terkoneksi dengan Broker MQTT
// dan melakukan aksi publish atau subcribe
#include <PubSubClient.h>

// Nama (ssid) dan Password wifi yang akan dikoneksikan NodeMCU
const char ssid[]     = "*";
const char password[] = "*";

// mendefenisikan pin yang terhubung dengan DataPin DHT11
// yaitu pin D4
#define DHTPIN D4

// Mendefenisikan jenis sensor DHT kita
#define DHTTYPE DHT11

// Deklarasi class dengan memasukan tipe dan datapin
DHT dht(DHTPIN, DHTTYPE);

// Deklarasi topic
const char topic_suhu[] = "nama_kalian/suhu_mcu";
const char topic_kelempaban[] = "nama_kalian/kelembapan_mcu";

// ada topic rssi (signal wifi) bagi yang belum terdapat sensor
const char topic_rssi[] = "nama_kalian/rssi_mcu";

// informasi Host dan Port MQTT broker yang akan digunakan
const char* broker = "broker.hivemq.com";
const uint16_t port = 1883;

// Membuat class client untuk pubsub
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  //// Tahap Mengkoneksikan ke WIFI
  
  // Code disini akan berjalan sekali
  // Baudrate di set 115200
  Serial.begin(115200);

  // Kita mulai mencoba mengkoneksi ke koneksi WIFI
  // untuk mengkoneksikan ke WIFI gunakan object WiFi dengan
  // fungsi begin yang menerima parameter ssid dan password wifi
  WiFi.begin(ssid, password);

  // print keterangan akan mencoba mengkoneksi ke ssid sebelumnya
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  // Selama status wifi tidak bernilai WL_CONNECTED akan di-print
  // titik setiap 500 ms atau 0.5 detik
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Setelah perulangan while di atas selesai, maka kita sudah
  // terkoneksi. mari kita print local IP dan MAC Address NodeMCU
  // gunakan fungsi localIP() dan macAddress() dari WiFi
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Siapkan client MQTT
  client.setServer(broker, port);

  // looping Selama kita belum terkoneksi dengan broker
  while (!client.connected()) {
    // buat client ID yang unik dimana ini perlu untuk terkoneksi dengan
    // broker public kita, untuk membuatnya unik kita bisa tambahkan nama
    // dan MAC Address NodeMCU kita
    String client_id = "NodeMCU-Nama_kalian-";
    client_id += String(WiFi.macAddress());
    Serial.print("Connecting to the public mqtt broker with client id : ");
    Serial.println(client_id.c_str());
    
    //jika kita terkoneksi, maka kita keluar dari looping while
    if (client.connect(client_id.c_str())){
        Serial.println("Connected to " + String(broker));
      }

    // jika tidak kita akan looping terus sampai terkoneksi
    else{
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
  }
}

void loop() {
  // Code disini akan berjalan terus

  // Tahap Mendapatkan Nilai suhu dan kelembapan
  // Membuat variable untuk memuat nilai kelembapan, mendapatkan nilai
  // tersebut mengunakan fungsi readHumidity() pada class dht
  float h = dht.readHumidity();

  // hal yang serupa untuk mendapatkan nilai suhu
  float t = dht.readTemperature();

  // periksa jika nilai tersebut gagal untuk dibaca, jika gagal akan
  // keluar dari fungsi loop, dan mencoba membacanya lagi
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // menambahkan nilai rssi wifi
  long rssi = WiFi.RSSI();

  // print nilai dari sensor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" | Temperature: ");
  Serial.print(t);
  Serial.print(" | RSSI: ");
  Serial.println(rssi);

  // melakukan publish ke topic yang sudah dibuat dengan
  // fungsi publish dari class client yang sudah di buat
  // sebelumnya, publish menerima dua parameter yaitu topic
  // dan nilai/data yang akan di publish dalam bentuk array of
  // cgaracters
  client.publish(topic_suhu, String(t).c_str());
  client.publish(topic_kelempaban , String(h).c_str());
  client.publish(topic_rssi, String(rssi).c_str());

  // kasi delay 5 detik untuk sensor mendapatkan nilainya
  delay(5000);
}
