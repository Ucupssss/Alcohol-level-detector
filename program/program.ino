#include <Wire.h>
#include <LiquidCrystal_I2C.h>

float sensor_volt;
float RS_gas; 
float R0;
float ratio;
float ppm;
float percent;
int R2 = 2000;
const int buzzerPin = 9;  // Pin digital untuk buzzer
const float detectionThreshold = 0.01; // Threshold for detection
unsigned long startTime = 0;  // Variabel untuk menyimpan waktu mulai

// Inisialisasi LCD dengan alamat I2C 0x27, 16 karakter lebar dan 2 baris
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() 
{
    Serial.begin(9600);
    pinMode(buzzerPin, OUTPUT);  // Mengatur pin buzzer sebagai output
    digitalWrite(buzzerPin, LOW);  // Memastikan buzzer mati saat awal

    lcd.init();  // Use init() instead of begin()

    // Turn on the backlight
    lcd.backlight();

    // Print a message to the LCD.
    lcd.setCursor(0, 0); // Set the cursor to column 0, line 0
    lcd.print("Pendeteksi");
    lcd.setCursor(0, 1); // Set the cursor to column 0, line 1
    lcd.print("Kadar Alkohol");
}

void loop() {
    int sensorValue = analogRead(A0);
    sensor_volt = (float)sensorValue / 1024 * 5.0;
    RS_gas = ((5.0 * R2) / sensor_volt) - R2; 

    //-Ganti nilai R0 dengan nilai R0 dalam pengujian Anda-/
    R0 = 16000;
    ratio = RS_gas / R0;  // rasio = RS/R0
    double x = 0.4 * ratio;   
    ppm = pow(x, -1.431);  // Konsentrasi dalam ppm

    percent = ppm / 10000;  // Konversi ppm ke persen

    // Set ppm dan percent ke 0 jika tidak ada alkohol terdeteksi
    if (percent < detectionThreshold) {
        ppm = 0;
        percent = 0;
    }

    Serial.print("Alkohol Dalam (ppm) = ");
    Serial.print(ppm);  // Tampilkan dalam ppm
    Serial.print(" ppm");

    Serial.print("Persentasi Alkohol = ");
    Serial.print(percent);  // Tampilkan dalam persen
    Serial.print(" %\n\n");

    // Tampilkan hasil pada LCD
    lcd.clear();
    lcd.setCursor(0, 0);  // Baris pertama
    lcd.print("Alkohol:");
    lcd.setCursor(9, 0);  // Menempatkan nilai ppm di posisi ke-9
    lcd.print(ppm);
    lcd.print("ppm");

    // Tampilkan persentasi alkohol pada LCD
    lcd.setCursor(0, 1);  // Baris kedua
    lcd.print("Kadar:");
    lcd.setCursor(9, 1);  // Menempatkan nilai persen di posisi ke-9
    lcd.print(percent * 100);
    lcd.print(" %");

    // Jika terdeteksi adanya alkohol, buzzer akan berbunyi
    if (percent > detectionThreshold) {  
        // Anda bisa mengatur ambang batas sesuai kebutuhan
        tone(buzzerPin, 1000, 500);  // Menghasilkan nada 1000Hz selama 500ms
        lcd.setCursor(13, 1);  // Pindahkan kursor ke posisi setelah persen
        lcd.print("!");  // Tampilkan peringatan
    } else {
        noTone(buzzerPin);  // Mematikan nada pada buzzer
    }

    delay(1000);  // Menunggu 1 detik sebelum pengukuran berikutnya
}