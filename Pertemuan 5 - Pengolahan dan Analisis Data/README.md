# Pertemuan 5 - Pengolahan dan Analisis Data

## Penjelasan Code

### Percobaan 5A - Moving Average dan Statistik pada Perangkat

Program Percobaan 5A dapat dilihat di [code/Percobaan5A.ino](code/Percobaan5A.ino). Program menghasilkan data suhu simulasi yang berfluktuasi, menambahkan noise kecil, dan membuat lonjakan suhu setiap 25 pembacaan. Data tersebut dipakai untuk menguji pengolahan di sisi perangkat tanpa bergantung pada sensor.

Setiap dua detik, program memasukkan suhu baru ke dalam buffer berukuran lima. Fungsi `hitungMovingAverage()` menghitung rata-rata sampel yang tersedia di awal program, kemudian rata-rata lima sampel terakhir setelah buffer penuh. Program juga menyimpan suhu minimum dan maksimum sejak board dinyalakan dan menampilkan nilai mentah, hasil filter, minimum, serta maksimum pada Serial Monitor.

### Percobaan 5B - Akuisisi, Analisis, dan Visualisasi

Program pengirim MQTT tersedia di [code/Percobaan5B.ino](code/Percobaan5B.ino). Sketch ini menggunakan ESP8266, menghasilkan suhu simulasi setiap tiga detik, dan mempublikasikannya sebagai JSON pada topic `unsoed/tk245004/kelompok2425/data`. Sketch juga menerima perintah `ON` atau `OFF` pada topic perintah untuk mengendalikan LED.

Program analisis Python tersedia di [code/Percobaan5B.py](code/Percobaan5B.py). Python mengumpulkan 30 sampel dari topic data, menyimpan waktu dan suhu ke `data_suhu.csv`, menghitung statistik deskriptif, dan mendeteksi anomali menggunakan z-score. Program kemudian menerapkan moving average dengan jendela lima data, menghitung ulang z-score pada hasil filter, membandingkan jumlah anomali sebelum dan sesudah filter, serta menyimpan grafik sebagai `grafik_suhu.png`.

> **Catatan kesesuaian perangkat:** Modul praktikum mengarahkan penggunaan ESP32 dan sensor DHT22 pada GPIO 4. Berkas Percobaan 5A dan sketch Percobaan 5B yang disertakan di sini memakai suhu simulasi; sketch MQTT juga menggunakan library dan board ESP8266. Jadi, kode ini mendemonstrasikan algoritma dan alur MQTT, tetapi bukan pembacaan langsung DHT22/ESP32. Untuk praktikum dengan perangkat fisik, ganti `bacaSuhuMock()` dengan pembacaan `dht.readTemperature()`, gunakan library DHT, dan sesuaikan library WiFi serta konfigurasi pin dengan board yang dipakai.

## Penjelasan Setiap Fungsi

- `setup()` pada Percobaan 5A memulai Serial Monitor dan menginisialisasi seed untuk bilangan acak.
- `bacaSuhuMock()` menghasilkan suhu dasar yang berubah perlahan, menambahkan noise, dan menyisipkan lonjakan untuk simulasi anomali.
- `hitungMovingAverage(nilaiBaru)` menyimpan nilai baru ke buffer melingkar, memperbarui jumlah sampel yang tersedia, lalu mengembalikan nilai rata-ratanya.
- `loop()` pada Percobaan 5A membaca suhu simulasi, mengabaikan nilai tidak valid, memperbarui hasil filter dan statistik minimum/maksimum, lalu menampilkan hasil setiap dua detik.
- `setup()` pada sketch Percobaan 5B mengatur Serial Monitor dan LED, menghubungkan WiFi, serta menyiapkan server dan callback MQTT.
- `callback()` mengubah payload MQTT menjadi teks, membaca JSON, lalu menyalakan atau mematikan LED sesuai nilai `perintah`.
- `hubungkanWiFi()` menghubungkan ESP8266 ke jaringan WiFi.
- `hubungkanMQTT()` mencoba koneksi ke broker dan melakukan subscribe pada topic perintah setelah koneksi berhasil.
- `loop()` menjaga koneksi WiFi dan MQTT, memanggil `client.loop()` agar pesan masuk diproses, lalu menerbitkan data suhu sesuai interval.
- `on_connect()` pada Python melakukan subscribe ke topic data setelah client terhubung.
- `on_message()` memvalidasi payload JSON, mengambil nilai suhu, mencatat waktu penerimaan, dan menghentikan client setelah jumlah sampel terpenuhi.
- `rolling(window=5, min_periods=1).mean()` menghitung moving average lima sampel. `min_periods=1` membuat nilai awal tetap bisa dihitung sebelum lima sampel terkumpul.
- `to_csv()` menyimpan data mentah, hasil moving average, z-score, dan penanda anomali ke CSV.
- Bagian visualisasi menggambar suhu mentah, hasil moving average, dan penanda anomali; grafik disimpan ke file sebelum ditampilkan.

## Penjelasan Percabangan atau Conditional

Pada Percobaan 5A, `if (isnan(suhu))` memastikan pembacaan yang tidak valid tidak masuk ke perhitungan. Kondisi `if (jumlah < N)` membatasi jumlah sampel sampai buffer penuh. Kondisi pada pembaruan `suhuMin` dan `suhuMax` memastikan nilai minimum dan maksimum hanya berubah ketika ditemukan nilai yang lebih kecil atau lebih besar.

Pada sketch Percobaan 5B, `if (WiFi.status() != WL_CONNECTED)` dan `if (!client.connected())` memicu koneksi ulang ketika koneksi terputus. Di callback, hasil deserialisasi JSON diperiksa sebelum nilai perintah digunakan; perintah yang tidak dikenal tidak mengubah LED.

Pada program Python, blok `try-except` mengabaikan pesan JSON yang rusak, field `suhu` yang tidak tersedia, atau nilai suhu yang tidak dapat dikonversi menjadi angka. Pemeriksaan `std == 0` mencegah pembagian dengan nol ketika seluruh data sama. Kondisi `abs(zscore) > 2` menandai sampel yang jaraknya lebih dari dua simpangan baku dari rata-rata.

## Library atau Dependencies yang Diperlukan

- Arduino IDE dan board package yang sesuai dengan board yang digunakan.
- Untuk sketch Percobaan 5B yang tersedia: board ESP8266, `ESP8266WiFi.h`, `PubSubClient.h`, `ArduinoJson.h`, dan `math.h`.
- Untuk Percobaan 5A, `math.h` dan fungsi acak bawaan Arduino digunakan untuk membuat data simulasi.
- Python 3 dengan pustaka berikut:

  ```bash
  pip install paho-mqtt pandas matplotlib
  ```

- Koneksi internet dan broker MQTT publik `broker.hivemq.com` pada port `1883`.
- Pada sketch MQTT, ganti `NAMA_WIFI_ANDA` dan `PASSWORD_WIFI_ANDA` dengan kredensial WiFi sendiri sebelum mengunggahnya. Jangan menyimpan kredensial pribadi ke repository.
- Pastikan nilai `TOPIC` pada Python sama dengan `topicData` pada sketch. Untuk menerima perintah LED, publish JSON seperti `{"perintah":"ON"}` atau `{"perintah":"OFF"}` ke topic perintah.

## Jawaban Pertanyaan Praktikum yang Berkaitan dengan Code

### Percobaan 5A

#### 1. Gambarkan diagram alur (flowchart) proses perhitungan moving average pada fungsi `hitungMovingAverage()`!

![Flowchart perhitungan moving average](images/flowchart-percobaan-5a-moving-average.png)

Buffer diisi satu per satu pada awal program, sehingga rata-rata hanya menggunakan sampel yang sudah tersedia. Setelah buffer penuh, setiap nilai baru menggantikan nilai paling lama.

#### 2. Jelaskan fungsi dari operasi `(indeks + 1) % N` pada program tersebut!

Operasi `(indeks + 1) % N` membuat buffer bekerja sebagai *circular buffer* (buffer melingkar). Setiap pembacaan baru, indeks bertambah satu. Karena `N = 5`, operasi modulo membuat indeks bergerak pada `0, 1, 2, 3, 4`, lalu kembali ke `0` setelah mencapai ujung array. Dengan demikian, indeks tidak pernah melewati batas array.

Setelah buffer penuh, data terlama otomatis tertimpa oleh data terbaru sehingga buffer selalu berisi `N` pembacaan terakhir. Program tidak perlu menggeser seluruh isi array setiap kali ada data baru, sehingga penggunaan waktu proses pada ESP8266 lebih efisien.

#### 3. Apa dampaknya terhadap data hasil filter apabila nilai `N` diperbesar menjadi 20 atau diperkecil menjadi 2?

- **`N = 20`:** Hasil filter menjadi lebih halus karena derau dirata-ratakan dari lebih banyak sampel. Namun, respons terhadap perubahan suhu menjadi lebih lambat. Dengan jeda dua detik antar pembacaan, jendela 20 sampel mencakup sekitar 40 detik data. Ukuran buffer yang digunakan juga lebih besar.
- **`N = 2`:** Filter lebih cepat mengikuti perubahan suhu, tetapi efek penghalusannya lebih kecil. Hasilnya lebih mirip data mentah sehingga lonjakan dan derau masih terbawa.

Pemilihan `N` merupakan kompromi antara kehalusan data dan kecepatan respons. `N = 5` pada percobaan ini menjadi nilai tengah di antara keduanya.

#### 4. Modifikasi program agar ESP32 juga menghitung simpangan baku dari data pada buffer. Jelaskan setiap baris kode yang ditambahkan.

Berikut program lengkap Percobaan 5A dengan perhitungan simpangan baku pada buffer:

```cpp
#include <math.h>

// Mock function pengganti sensor DHT22
float bacaSuhuMock() {
    static unsigned long jumlahBaca = 0;
    jumlahBaca++;

    // Suhu dasar berubah secara bertahap
    float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);

    // Noise kecil untuk mensimulasikan fluktuasi sensor
    float noise = random(-5, 6) * 0.1;
    float suhu = suhuDasar + noise;

    // Lonjakan suhu setiap 25 pembacaan untuk simulasi anomali
    if (jumlahBaca % 25 == 0) {
        suhu += 8.0;
    }

    return suhu;
}

const int N = 5;
float buffer[N];
int indeks = 0;
int jumlah = 0;

float suhuMin = 1000.0;
float suhuMax = -1000.0;

float hitungMovingAverage(float nilaiBaru) {
    buffer[indeks] = nilaiBaru;
    indeks = (indeks + 1) % N;
    if (jumlah < N) {
        jumlah++;
    }

    float total = 0;
    for (int i = 0; i < jumlah; i++) {
        total += buffer[i];
    }

    return total / jumlah;
}

// TAMBAHAN: Menghitung simpangan baku data yang tersimpan di buffer
float hitungSimpanganBaku() {
    float total = 0.0;
    for (int i = 0; i < jumlah; i++) {
        total += buffer[i];
    }

    float rataRata = total / jumlah;
    float jumlahKuadratSelisih = 0.0;

    for (int i = 0; i < jumlah; i++) {
        float selisih = buffer[i] - rataRata;
        jumlahKuadratSelisih += selisih * selisih;
    }

    float varians = jumlahKuadratSelisih / jumlah;
    return sqrt(varians);
}

void setup() {
    Serial.begin(115200);
    randomSeed(micros());
}

void loop() {
    float suhu = bacaSuhuMock();

    if (isnan(suhu)) {
        Serial.println("Pembacaan gagal, data diabaikan");
    } else {
        float suhuFilter = hitungMovingAverage(suhu);

        // TAMBAHAN: Menghitung simpangan baku setelah buffer diperbarui
        float simpanganBaku = hitungSimpanganBaku();

        if (suhu < suhuMin) suhuMin = suhu;
        if (suhu > suhuMax) suhuMax = suhu;

        Serial.print("Mentah: ");
        Serial.print(suhu);
        Serial.print(" | Filter: ");
        Serial.print(suhuFilter);
        Serial.print(" | Simpangan Baku: ");
        Serial.print(simpanganBaku);
        Serial.print(" | Min: ");
        Serial.print(suhuMin);
        Serial.print(" | Max: ");
        Serial.println(suhuMax);
    }

    delay(2000);
}
```

Penjelasan bagian yang ditambahkan:

1. `hitungSimpanganBaku()` menjumlahkan nilai di buffer, lalu membaginya dengan `jumlah` untuk memperoleh rata-rata sampel yang tersedia.
2. Perulangan kedua menghitung selisih setiap nilai dari rata-rata dan menjumlahkan kuadrat selisihnya.
3. Pembagian jumlah kuadrat selisih dengan `jumlah` menghasilkan varians populasi. `sqrt()` mengakarkannya untuk memperoleh simpangan baku.
4. Pemanggilan `hitungSimpanganBaku()` dilakukan setelah moving average memperbarui buffer, sehingga perhitungan menggunakan isi buffer terbaru.
5. Dua pemanggilan `Serial.print()` menampilkan label dan nilai simpangan baku di Serial Monitor.

### Percobaan 5B

#### 1. Jelaskan fungsi blok `try-except` pada fungsi `on_message()` dan apa yang terjadi apabila blok tersebut dihilangkan!

Blok `try-except` menangani kesalahan saat pesan MQTT diolah. Di dalam blok `try`, program menggunakan `json.loads()` untuk mengubah payload menjadi dictionary, mengambil nilai `payload["suhu"]`, lalu mengubahnya menjadi `float`. Blok `except` menangkap `ValueError` jika payload bukan JSON yang valid atau nilai suhu tidak dapat diubah menjadi angka, serta `KeyError` jika key `"suhu"` tidak ada. Jika terjadi kesalahan, program mencetak `"Data tidak valid, diabaikan"` lalu keluar dari fungsi dengan `return`, sehingga data yang rusak tidak dimasukkan ke dalam list.

Jika blok tersebut dihilangkan, kesalahan pada satu pesan dapat menimbulkan exception yang tidak tertangani di callback. Exception tersebut dapat menghentikan `client.loop_forever()` dan program berhenti dengan traceback. Akibatnya, pengumpulan data terputus dan tahap penyimpanan CSV, analisis, serta visualisasi tidak dijalankan. Hal ini dapat terjadi karena broker publik `broker.hivemq.com` dapat menerima pesan dari siapa saja yang menggunakan topic yang sama.

#### 2. Jelaskan arti nilai z-score dan mengapa batas `|z| > 2` digunakan sebagai penentu anomali!

Z-score menyatakan seberapa jauh suatu nilai menyimpang dari rata-rata dalam satuan simpangan baku, dengan rumus `z = (x - rata-rata) / simpangan baku`. Nilai `z = 0` berarti data sama dengan rata-rata, nilai z positif berarti berada di atas rata-rata, dan nilai z negatif berarti berada di bawah rata-rata. Semakin besar nilai absolut z, semakin jauh data tersebut dari kondisi umum.

Pada data yang berdistribusi mendekati normal, sekitar 95% data berada dalam rentang rata-rata ± 2 simpangan baku. Data dengan `|z| > 2` berada di luar rentang tersebut, sehingga tergolong jarang terjadi dan patut dicurigai sebagai anomali. Batas ini merupakan kompromi: batas yang terlalu kecil, misalnya `|z| > 1`, dapat menandai terlalu banyak data normal sebagai anomali, sedangkan batas yang terlalu besar, misalnya `|z| > 3`, dapat melewatkan anomali, terutama pada jumlah sampel yang sedikit seperti 30 data.

#### 3. Apa kelemahan metode z-score apabila data memiliki tren naik atau turun yang tajam?

Z-score pada program ini dihitung dari rata-rata dan simpangan baku seluruh data, sehingga mengasumsikan data berada di sekitar satu rata-rata yang tetap. Jika data memiliki tren naik atau turun yang tajam, beberapa masalah dapat terjadi:

- Data di awal dan akhir tren dapat ditandai sebagai anomali meskipun merupakan bagian dari perubahan yang wajar (*false positive*).
- Simpangan baku dapat membesar akibat tren, sehingga lonjakan yang sebenarnya anomali tidak lagi terlihat menyimpang (*false negative*).
- Nilai ekstrem dapat mengubah rata-rata dan simpangan baku, sehingga anomali besar ikut menggeser batas deteksinya sendiri.

Untuk mengatasinya, z-score dapat dihitung pada jendela data yang bergerak (*rolling window*), atau tren dapat dihilangkan terlebih dahulu sebelum anomali dideteksi.

#### 4. Terapkan moving average (jendela 5 data) pada data sebelum menghitung z-score, bandingkan jumlah anomali yang terdeteksi, dan jelaskan kode tambahannya.

Berikut program Python lengkap untuk mengumpulkan data MQTT, menganalisis data mentah dan hasil moving average, lalu membandingkan deteksi anomalinya:

```python
import json
import time

import matplotlib.pyplot as plt
import pandas as pd
import paho.mqtt.client as mqtt

BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "unsoed/tk245004/kelompok2425/data"
JUMLAH_SAMPEL = 30

data = []


def on_connect(client, userdata, flags, reason_code, properties):
    print("Terhubung ke broker, kode:", reason_code)
    client.subscribe(TOPIC)


def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode())
        suhu = float(payload["suhu"])
    except (TypeError, ValueError, KeyError):
        print("Data tidak valid, diabaikan")
        return

    data.append({
        "waktu": time.strftime("%Y-%m-%d %H:%M:%S"),
        "suhu": suhu,
    })

    print(f"[{len(data)}/{JUMLAH_SAMPEL}] suhu = {suhu}")

    if len(data) >= JUMLAH_SAMPEL:
        client.disconnect()


# Tahap 1: Akuisisi data dari MQTT
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT)
client.loop_forever()

# Tahap 2: Membuat DataFrame dari data hasil akuisisi
df = pd.DataFrame(data)

# TAMBAHAN: Memastikan data tersedia sebelum melakukan analisis
if df.empty:
    raise SystemExit("Tidak ada data suhu yang diterima.")

# Tahap 3: Menghitung moving average dengan jendela 5 data
df["suhu_ma5"] = df["suhu"].rolling(window=5, min_periods=1).mean()

# Tahap 4: Menghitung z-score data mentah
rata2 = df["suhu"].mean()
std = df["suhu"].std()

if std == 0:
    df["zscore"] = 0.0
else:
    df["zscore"] = (df["suhu"] - rata2) / std

df["anomali"] = df["zscore"].abs() > 2

# Tahap 5: Menghitung z-score setelah moving average MA5
rata2_ma5 = df["suhu_ma5"].mean()
std_ma5 = df["suhu_ma5"].std()

if std_ma5 == 0:
    df["zscore_ma5"] = 0.0
else:
    df["zscore_ma5"] = (df["suhu_ma5"] - rata2_ma5) / std_ma5

df["anomali_ma5"] = df["zscore_ma5"].abs() > 2

# Tahap 6: Menyimpan hasil analisis ke CSV
df.to_csv("data_suhu.csv", index=False)
print("Data dan hasil analisis tersimpan di data_suhu.csv")

print(f"Rata-rata suhu mentah : {rata2:.2f}")
print(f"Std deviasi mentah    : {std:.2f}")
print(f"Minimum suhu mentah   : {df['suhu'].min():.2f}")
print(f"Maksimum suhu mentah  : {df['suhu'].max():.2f}")

jumlah_anomali_mentah = int(df["anomali"].sum())
jumlah_anomali_ma5 = int(df["anomali_ma5"].sum())
print(f"Anomali data mentah   : {jumlah_anomali_mentah}")
print(f"Anomali setelah MA5   : {jumlah_anomali_ma5}")

# Tahap 7: Visualisasi hasil
plt.figure(figsize=(10, 4))
plt.plot(df.index, df["suhu"], marker="o", label="Suhu mentah")
plt.plot(df.index, df["suhu_ma5"], linestyle="--", label="Moving average (5)")

anom = df[df["anomali"]]
plt.scatter(
    anom.index,
    anom["suhu"],
    color="red",
    s=100,
    label="Anomali data mentah",
    zorder=3,
)

# TAMBAHAN: Menampilkan anomali pada data hasil MA5
anom_ma5 = df[df["anomali_ma5"]]
plt.scatter(
    anom_ma5.index,
    anom_ma5["suhu_ma5"],
    color="purple",
    marker="x",
    s=80,
    label="Anomali setelah MA5",
    zorder=3,
)

plt.axhline(rata2, color="gray", linestyle="--", label="Rata-rata suhu mentah")
plt.xlabel("Urutan data")
plt.ylabel("Suhu (°C)")
plt.title("Perbandingan Suhu Mentah, MA5, dan Deteksi Anomali")
plt.legend()
plt.tight_layout()
plt.savefig("grafik_suhu.png")
plt.show()
```

Penjelasan bagian yang ditambahkan:

1. **Validasi data:** `df.empty` memastikan ada data yang diterima sebelum analisis dijalankan.
2. **Moving average:** `rolling(window=5, min_periods=1).mean()` menghitung rata-rata bergerak lima sampel. `min_periods=1` memungkinkan perhitungan sejak sampel pertama.
3. **Z-score MA5:** rata-rata dan simpangan baku dihitung dari kolom `suhu_ma5`, kemudian digunakan untuk membuat kolom `zscore_ma5`. Pemeriksaan `std_ma5 == 0` mencegah pembagian dengan nol.
4. **Penanda dan perbandingan anomali:** `anomali_ma5` menandai hasil dengan batas `|z| > 2`. Kedua kolom anomali dijumlahkan dan ditampilkan agar hasil data mentah dan MA5 dapat dibandingkan.
5. **Visualisasi MA5:** titik silang ungu menandai anomali pada hasil moving average, sehingga dapat dibandingkan dengan anomali data mentah pada grafik.

## Skematik atau Diagram Rangkaian

![Rangkaian board ESP8266 pada breadboard](images/rangkaian-percobaan-5a.png)

Foto menunjukkan board NodeMCU ESP8266 yang terpasang pada breadboard dan terhubung ke komputer melalui USB. Sensor DHT22 tidak tampak pada foto, dan program yang didokumentasikan masih menggunakan suhu simulasi.

## Foto Proses Praktikum

### Percobaan 5A - Moving Average dan Statistik

![Program Percobaan 5A dan hasil pada Serial Monitor](images/dokumentasi-percobaan-5a-serial-monitor.png)

Serial Monitor memperlihatkan nilai suhu mentah, hasil moving average, serta nilai minimum dan maksimum.

### Percobaan 5B - Pengiriman Data MQTT

![Data suhu diterima pada MQTT Explorer](images/dokumentasi-percobaan-5b-mqtt-explorer.png)

MQTT Explorer menampilkan pesan suhu pada topic data.

### Percobaan 5B - Analisis Data dengan Python

![Program Python Percobaan 5B](images/dokumentasi-percobaan-5b-python.png)

![Terminal menjalankan program pengumpulan data MQTT](images/dokumentasi-percobaan-5b-terminal.png)

> Screenshot sketch MQTT (`dokumentasi-percobaan-5b-sketch-perlu-redaksi.png`) tidak ditampilkan karena kredensial WiFi terlihat pada gambar. Samarkan kredensial dan ganti password WiFi jika masih digunakan sebelum gambar dibagikan atau repository dipublikasikan.
