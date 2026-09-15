#include <ESP8266WiFi.h>

const char* apSsid = "ESP8266-PraktikumIoT";
const char* apPassword = "12345678"; // WPA2 membutuhkan minimal 8 karakter

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);

  if (!WiFi.softAP(apSsid, apPassword)) {
    Serial.println("Gagal mengaktifkan Access Point.");
    return;
  }

  Serial.println("Access Point aktif!");
  Serial.print("SSID       : ");
  Serial.println(apSsid);
  Serial.print("IP Address : ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(WiFi.softAPgetStationNum());
  delay(5000);
}