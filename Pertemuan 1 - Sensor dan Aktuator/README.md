# Pertemuan 1 - Sensor dan Aktuator
## Penjelasan Code

Ada dua percobaan pada praktikum ini. Percobaan pertama membaca suhu dan kelembaban dari DHT11. Percobaan kedua membaca suhu dari DHT22 dan memakai hasilnya untuk mengendalikan relay atau LED.

### Percobaan 1

Kode ada di [code/Percobaan1H1.ino](code/Percobaan1H1.ino). Percobaan ini menggunakan NodeMCU ESP8266 dan DHT11. Pin data sensor dipasang pada `D4` atau GPIO2. Setiap perulangan program membaca suhu dan kelembaban, lalu menampilkannya di Serial Monitor.

Kalau sensor gagal dibaca, program menampilkan pesan kesalahan. Kalau pembacaan berhasil, nilai suhu dan kelembaban ditampilkan. Pembacaan dilakukan setiap 500 ms.

### Percobaan 2

Kode ada di [code/Percobaan2H1.ino](code/Percobaan2H1.ino). Percobaan ini menggunakan ESP32, DHT22 pada GPIO4, dan relay atau LED pada GPIO26. Suhu yang terbaca dibandingkan dengan batas `30.0 C`.

Jika suhu lebih dari 30.0 C, relay atau LED dinyalakan. Jika suhu 30.0 C atau lebih rendah, relay atau LED dimatikan.

## Penjelasan Setiap Fungsi

- `setup()` dijalankan satu kali ketika board mulai menyala. Fungsi ini menyiapkan Serial Monitor, sensor DHT, dan pin relay.
- `loop()` dijalankan terus-menerus untuk membaca sensor dan menjalankan program.
- `dht.begin()` memulai sensor DHT.
- `dht.readTemperature()` membaca suhu.
- `dht.readHumidity()` membaca kelembaban.
- `isnan()` mengecek apakah hasil pembacaan sensor tidak valid.
- `pinMode()` mengatur fungsi sebuah pin, misalnya sebagai `OUTPUT`.
- `digitalWrite()` memberi keluaran `HIGH` atau `LOW` pada pin relay.
- `Serial.print()` dan `Serial.println()` menampilkan informasi di Serial Monitor.
- `delay()` memberi jeda sebelum proses berikutnya.

## Penjelasan Percabangan atau Conditional

Pada Percobaan 1, kondisi `isnan(kelembaban) || isnan(suhu)` mengecek hasil pembacaan sensor. Jika salah satu hasil tidak valid, program menampilkan pesan kegagalan. Jika keduanya valid, nilai suhu dan kelembaban ditampilkan.

Pada Percobaan 2, program mengecek apakah pembacaan suhu gagal. Jika berhasil, suhu dibandingkan dengan `suhuThreshold` menggunakan kondisi `suhu > suhuThreshold`. Relay menyala jika suhu lebih dari 30.0 C dan mati jika suhu kurang dari atau sama dengan 30.0 C.

## Library atau Dependencies yang Diperlukan

- Arduino IDE atau PlatformIO.
- Library `DHT sensor library` dari Adafruit.
- NodeMCU ESP8266 untuk Percobaan 1.
- ESP32 untuk Percobaan 2.
- Sensor DHT11 untuk Percobaan 1.
- Sensor DHT22 dan relay atau LED untuk Percobaan 2.

Library DHT dipanggil dengan perintah `#include <DHT.h>`.

## Jawaban Pertanyaan Praktikum yang Berkaitan dengan Code

**Mengapa hasil sensor dicek dengan `isnan()`?**

Karena pembacaan sensor tidak selalu berhasil. Pengecekan ini membantu program membedakan data yang valid dan data yang gagal dibaca.

**Kapan relay atau LED menyala?**

Relay atau LED menyala ketika suhu terbaca valid dan nilainya lebih dari 30.0 C.

**Apa yang terjadi jika suhu tepat 30.0 C?**

Relay atau LED tetap mati karena program menggunakan operator `>`.

**Apa perbedaan kedua percobaan?**

Percobaan 1 hanya membaca dan menampilkan suhu serta kelembaban. Percobaan 2 menggunakan suhu untuk mengatur aktuator.

## Penjelasan Singkat Detail Percobaan

Pada Percobaan 1, DHT11 dipasang ke NodeMCU. Setelah program di-upload, suhu dan kelembaban dapat dilihat melalui Serial Monitor dengan baud rate `115200`.

Pada Percobaan 2, DHT22 dipasang ke ESP32 dan relay atau LED dipasang sebagai aktuator. Nilai suhu digunakan untuk menentukan apakah aktuator menyala atau mati.

## Skematik atau Diagram Rangkaian

### Percobaan 1: NodeMCU dan DHT11

```text
NodeMCU ESP8266       DHT11
3V3 ------------------ VCC
GND ------------------ GND
D4 / GPIO2 ----------- DATA
```

### Percobaan 2: ESP32, DHT22, dan Relay atau LED

```text
ESP32                 DHT22
3V3 ------------------ VCC
GND ------------------ GND
GPIO4 --------------- DATA

ESP32                 Relay / LED
GND ------------------ GND
GPIO26 -------------- IN / Anode melalui resistor
```

Diagram ini mengikuti pin yang digunakan di dalam program. Sambungan daya dan modul relay perlu disesuaikan dengan komponen yang dipakai saat praktikum.

## Foto Proses Praktikum atau Perangkaian

### Percobaan 1

![Rangkaian NodeMCU dan DHT11](images/foto-percobaan-1-rangkaian.png)

![Hasil pembacaan sensor pada Serial Monitor](images/foto-percobaan-1-monitor.png)

### Percobaan 2

![Rangkaian ESP atau NodeMCU, sensor DHT, dan relay](images/foto-percobaan-2-rangkaian.png)

## GIF atau Video Demonstrasi

GIF atau video demonstrasi belum tersedia. Tautan video dapat ditambahkan pada bagian ini setelah proses pengujian direkam.