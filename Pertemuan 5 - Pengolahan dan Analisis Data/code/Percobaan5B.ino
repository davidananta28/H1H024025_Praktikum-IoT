
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <math.h>

// Ganti dengan WiFi yang digunakan ESP8266
const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

// Konfigurasi MQTT
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

const char* topicData = "unsoed/tk245004/kelompok2425/data";
const char* topicPerintah = "unsoed/tk245004/kelompok2425/perintah";

const int ledPin = D5;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long waktuTerakhirPublish = 0;
const unsigned long intervalPublish = 3000;

// Mock function pengganti sensor DHT
float bacaSuhuMock() {
  static unsigned long jumlahBaca = 0;
  jumlahBaca++;

  // Suhu dasar berubah secara bertahap
  float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);

  // Noise kecil
  float noise = random(-5, 6) * 0.1;

  float suhu = suhuDasar + noise;

  // Lonjakan suhu setiap 25 pembacaan
  if (jumlahBaca % 25 == 0) {
    suhu += 8.0;
  }

  return suhu;
}

// Menerima perintah LED melalui MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String pesan;

  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  JsonDocument doc;

  if (deserializeJson(doc, pesan)) {
    Serial.println("Format perintah tidak valid");
    return;
  }

  const char* perintah = doc["perintah"] | "";

  if (String(perintah) == "ON") {
    digitalWrite(ledPin, HIGH);
  } else if (String(perintah) == "OFF") {
    digitalWrite(ledPin, LOW);
  } else {
    Serial.println("Perintah tidak dikenal");
    return;
  }

  Serial.print("Perintah diterima: ");
  Serial.println(perintah);
}

// Menghubungkan ke WiFi
void hubungkanWiFi() {
  Serial.print("Menghubungkan ke WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi berhasil terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Menghubungkan ke MQTT
void hubungkanMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");

    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("berhasil!");

      client.subscribe(topicPerintah);
      Serial.println("Subscribe topic perintah berhasil");
    } else {
      Serial.print("Gagal, kode: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  randomSeed(analogRead(A0));

  hubungkanWiFi();

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    hubungkanWiFi();
  }

  if (!client.connected()) {
    hubungkanMQTT();
  }

  client.loop();

  // Mengirim data setiap 3 detik
  unsigned long sekarang = millis();

  if (sekarang - waktuTerakhirPublish >= intervalPublish) {
    waktuTerakhirPublish = sekarang;

    float suhu = bacaSuhuMock();

    JsonDocument doc;
    doc["suhu"] = suhu;

    char buffer[128];
    serializeJson(doc, buffer);

    bool terkirim = client.publish(topicData, buffer);

    Serial.print("Suhu simulasi: ");
    Serial.print(suhu, 2);
    Serial.print(" °C | Data: ");
    Serial.print(buffer);
    Serial.print(" | MQTT: ");
    Serial.println(terkirim ? "Terkirim" : "Gagal");
  }
}
