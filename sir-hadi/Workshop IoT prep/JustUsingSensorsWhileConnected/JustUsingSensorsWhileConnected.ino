// WIFI Library untuk dapat mengatur dan terkoneksi ke WIFI
#include <ESP8266WiFi.h>
// Library untuk sensor kita
#include "DHT.h"

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
}

void loop() {
  // Code disini akan berjalan terus

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

  // print nilai dari sensor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" | Temperature: ");
  Serial.print(t);
  Serial.println();

  // kasi delay 5 detik untuk sensor mendapatkan nilainya
  delay(5000);
}
