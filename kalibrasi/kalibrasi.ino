#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int sensorPin = A0; 
float R0 = 10; 

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600); 
  lcd.init();         
  lcd.backlight();    
  lcd.print("Kalibrasi...");
  delay(20000); 
  R0 = calibrateSensor();
  lcd.clear();
  lcd.print("R0:");
  lcd.print(R0);
  delay(2000);
}

void loop() {
  float sensorValue = analogRead(sensorPin); 
  float ppm = calculatePPM(sensorValue); 
  float percentage = calculatePercentage(ppm); 
  Serial.print("Konsentrasi gas alkohol: ");
  Serial.print(ppm);
  Serial.print(" PPM, ");
  Serial.print("Persentase: ");
  Serial.print(percentage);
  Serial.println(" %");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PPM: ");
  lcd.print(ppm);
  lcd.setCursor(0, 1);
  lcd.print("Persen: ");
  lcd.print(percentage);
  lcd.print(" %");

  delay(1000); 
}

float calibrateSensor() {
  int samples = 50; 
  float sum = 0;

  for (int i = 0; i < samples; i++) {
    sum += analogRead(sensorPin);
    delay(50); 
  }

  float average = sum / samples;
  float R0 = ((1023.0 / average) - 1.0) * 10.0; 
  return R0;
}

float calculatePPM(float sensorValue) {
  float RS_air = ((1023.0 / sensorValue) - 1.0) * 10.0; 
  float ratio = RS_air / R0; 

  
  if (ratio > 1) {
    return 0; 
  }

  float ppm = pow(10, ((log10(ratio) - 0.45) / -0.35)); 
  return ppm;
}

float calculatePercentage(float ppm) {
  float maxPPM = 1000.0; // Nilai PPM maksimum (sesuaikan sesuai kebutuhan)
  float percentage = (ppm / maxPPM) * 100.0;
  return percentage;
}
