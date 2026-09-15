#include <ESP8266WiFi.h>

const char* ssid = "vivo";
const char* password = "12345678";
const int ledPin = 2; // GPIO2 atau LED bawaan NodeMCU
const unsigned long connectTimeout = 20000;

bool connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < connectTimeout) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Gagal terhubung ke WiFi.");
    return false;
  }

  Serial.println("WiFi berhasil terhubung!");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("RSSI       : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.mode(WIFI_STA);
  digitalWrite(ledPin, connectToWiFi() ? HIGH : LOW);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi terputus. Mencoba menghubungkan kembali...");
    digitalWrite(ledPin, LOW);
    WiFi.disconnect();
    digitalWrite(ledPin, connectToWiFi() ? HIGH : LOW);
  } else {
    Serial.print("Status: Terhubung | RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  }

  delay(5000);
}