#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definisi pin dan serial untuk PZEM
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#define PZEM_SERIAL Serial2

PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);

// Inisialisasi LCD I2C (alamat I2C bisa disesuaikan jika perlu, biasanya 0x27 atau 0x3F)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Variabel untuk menyimpan data dari sensor
float voltage, current, power, energy, frequency, pf, reactivePower;

void setup() {
  // Inisialisasi komunikasi Serial untuk debugging
  Serial.begin(115200);
  delay(1000);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();

  // Tampilkan pesan awal di LCD
  lcd.setCursor(0, 0);
  lcd.print("PZEM Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("Inisialisasi...");
  delay(2000); // Tunggu 2 detik sebelum mulai membaca data

  // Bersihkan layar
  lcd.clear();
}

void loop() {
  // Membaca data dari PZEM
  voltage = pzem.voltage();
  current = pzem.current();
  power = pzem.power();
  energy = pzem.energy();
  frequency = pzem.frequency();
  pf = pzem.pf();

  // Hitung daya reaktif (Reactive Power = Power * tan(acos(PF)))
  if (!isnan(pf) && pf > 0.0 && pf <= 1.0) {
    reactivePower = power * tan(acos(pf));
  } else {
    reactivePower = NAN;
  }

  // Tampilkan data di Serial Monitor
  Serial.println("=== PZEM Monitoring ===");
  if (!isnan(voltage)) {
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
  } else {
    Serial.println("Error reading voltage!");
  }

  if (!isnan(current)) {
    Serial.print("Current: "); Serial.print(current); Serial.println(" A");
  } else {
    Serial.println("Error reading current!");
  }

  if (!isnan(power)) {
    Serial.print("Power: "); Serial.print(power); Serial.println(" W");
  } else {
    Serial.println("Error reading power!");
  }

  if (!isnan(energy)) {
    Serial.print("Energy: "); Serial.print(energy); Serial.println(" kWh");
  } else {
    Serial.println("Error reading energy!");
  }

  if (!isnan(frequency)) {
    Serial.print("Frequency: "); Serial.print(frequency); Serial.println(" Hz");
  } else {
    Serial.println("Error reading frequency!");
  }

  if (!isnan(pf)) {
    Serial.print("Power Factor: "); Serial.println(pf);
  } else {
    Serial.println("Error reading power factor!");
  }

  if (!isnan(reactivePower)) {
    Serial.print("Reactive Power: "); Serial.print(reactivePower); Serial.println(" kVAR");
  } else {
    Serial.println("Error calculating reactive power!");
  }
  Serial.println("========================");

  // Tampilkan data di LCD
  lcd.setCursor(0, 0); // Baris 1
  lcd.print("I:"); lcd.print(current, 2); lcd.print("A");
  lcd.setCursor(13, 0); // Baris 1
  lcd.print("V:"); lcd.print(voltage, 0); 

  lcd.setCursor(0, 1); // Baris 2
  lcd.print("P:"); lcd.print(power, 1); lcd.print("W ");
  lcd.setCursor(13, 1); lcd.print("PF:"); lcd.print(pf, 2); 

  lcd.setCursor(0, 2); // Baris 3
  lcd.print("VAR:"); lcd.print(reactivePower, 1);
  lcd.setCursor(13, 2); lcd.print("Hz:");   lcd.print(frequency, 1);

  lcd.setCursor(0, 3); // Baris 4
  lcd.print("E: "); lcd.print(energy, 2); lcd.print("kWh");

  // Tunggu sebelum pembaruan berikutnya
  delay(1000);
}
