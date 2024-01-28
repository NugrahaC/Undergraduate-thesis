#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "model.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

Eloquent::ML::Port::DecisionTree classifier;

float features[4];
int flexPin1 = A0; // Pin analog untuk sensor fleksibel 1
int flexPin2 = A1; // Pin analog untuk sensor fleksibel 2
float prevFlex1 = 0;
float prevFlex2 = 0;
unsigned long prevMillis = 0;
unsigned long interval = 60000; // 1 menit
int totalBreaths = 0;

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print("Classifying...");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= interval) {
    prevMillis = currentMillis;

    float flex1 = readFlex(flexPin1);
    float flex2 = readFlex(flexPin2);

    float respRate1 = calculateRespirationRate(flex1, prevFlex1);
    float respRate2 = calculateRespirationRate(flex2, prevFlex2);

    prevFlex1 = flex1;
    prevFlex2 = flex2;

    features[0] = calculateNoBreathing(respRate1, respRate2);
    features[1] = respRate1;
    features[2] = respRate2;
    features[3] = calculateRespirationState(respRate1, respRate2);

    String predictionClass = classifier.predictLabel(features);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Class: ");
    lcd.print(predictionClass);

    totalBreaths += static_cast<int>(features[3]); // Menambah jumlah pernapasan

    lcd.setCursor(0, 1);
    lcd.print("Total: ");
    lcd.print(totalBreaths);
    
    Serial.println(predictionClass);
  }
}

float readFlex(int pin) {
  int flexValue = analogRead(pin);

  float flex = map(flexValue, 0, 1023, 0, 100);

  return flex;
}

float calculateRespirationRate(float currentFlex, float previousFlex) {
  // Menghitung perubahan fleksibilitas relatif dari waktu ke waktu
  float flexChange = abs(currentFlex - previousFlex);

  // Mengubah perubahan fleksibilitas menjadi estimasi jumlah pernapasan per menit
  float respirationRate = flexChange * 60.0 / interval;

  return respirationRate;
}

float calculateRespirationState(float respRate1, float respRate2) {
  // Menghitung perbedaan jumlah pernapasan dari kedua sensor
  float respRateDiff = abs(respRate1 - respRate2);

  // Mengklasifikasikan keadaan pernapasan berdasarkan perbedaan jumlah pernapasan
  if (respRateDiff <= 5) {
    return 1; // Pernapasan normal
  } else if (respRate1 > respRate2) {
    return 2; // Tachypnea
  } else {
    return 3; // Bradypnea
  }
}

float calculateNoBreathing(float respRate1, float respRate2) {
  // Menentukan jika tidak ada pernapasan dari kedua sensor
  if (respRate1 == 0 && respRate2 == 0) {
    return 0; // Tidak bernapas
  } else {
    return 1; // Bernapas
  }
}