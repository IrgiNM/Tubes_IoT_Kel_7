#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Konfigurasi WiFi
const char* ssid = "LORD";          // Ganti dengan SSID WiFi
const char* password = "12345678"; // Ganti dengan password WiFi

// Konfigurasi MQTT
const char* mqtt_server = "mqtt.my.id"; // Ganti dengan broker MQTT
const int mqtt_port = 1883;                   // Port MQTT (1883 untuk non-SSL)
const char* mqtt_topic = "wokwi/tubes/iot/klp7";   // Topik untuk mengirim data

WiFiClient espClient;
PubSubClient client(espClient);

// sensor mendeteksi gelas datang
#define TRIGGD 15
#define ECHOGD 2

#define SERVO_PIN 4
Servo myservo;  // Membuat objek servo

void setup() {
  Serial.begin(115200); // Memulai komunikasi serial
  myservo.attach(SERVO_PIN); // Menghubungkan servo ke pin

  // Sambungkan ke WiFi
  setup_wifi();

  // Sambungkan ke broker MQTT
  client.setServer(mqtt_server, mqtt_port);

  // Tunggu koneksi ke broker
  reconnect();

  pinMode(TRIGGD, OUTPUT); // Mengatur trigPin sebagai OUTPUT
  pinMode(ECHOGD, INPUT);  // Mengatur echoPin sebagai INPUT
}

long readUltrasonicDistance(int trigPin, int echoPin) {
  // Mengirimkan sinyal trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Membaca waktu perjalanan sinyal echo
  long duration = pulseIn(echoPin, HIGH);

  // Menghitung jarak dalam cm
  long distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  // Membaca jarak dari sensor GD
  long distanceGD = readUltrasonicDistance(TRIGGD, ECHOGD);
  delay(50); // Menunggu sejenak sebelum membaca sensor berikutnya


  // Output jarak dari setiap sensor ke Serial Monitor
  Serial.print("Distance GD: ");
  Serial.print(distanceGD);
  Serial.println(" cm");

  myservo.write(0);

  // Mengontrol servo berdasarkan jarak sensor GD
  if (distanceGD <= 10) {
    myservo.write(180); // Menggerakkan servo ke 180 derajat jika jarak kurang dari 5 cm
  } else if(distanceGD > 10) {
    myservo.write(0); // Menggerakkan servo ke 0 derajat jika jarak lebih dari 5 cm
  }

  // Pastikan tetap terhubung ke broker MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Gabungkan data jarak menjadi satu pesan
  String message = String(distanceGD);
  client.publish(mqtt_topic, message.c_str()); // Kirim pesan

  Serial.println("Pesan dikirim: " + message);

  delay(500); // Menunggu selama 1 detik sebelum mengulangi loop
}

// Fungsi untuk menyambungkan ke WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Menghubungkan ke WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password, 6);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi terhubung!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

// Fungsi untuk menyambungkan ke broker MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke broker MQTT...");
    if (client.connect("ESP32Client")) { // Ganti "ESP32Client" jika diperlukan
      Serial.println("Berhasil terhubung!");
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" Coba lagi dalam 5 detik...");
      delay(5000);
    }
  }
}
