// WIFI Library untuk dapat mengatur dan terkoneksi ke WIFI
#include <ESP8266WiFi.h>


// Nama (ssid) dan Password wifi yang akan dikoneksikan NodeMCU
const char ssid[]     = "*";
const char password[] = "*";

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
  
}
