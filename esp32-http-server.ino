#include <Wire.h>
#include <LiquidCrystal.h>

#define MPU_ADDR 0x68
#define BUZZER_PIN 5

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(14, 27, 26, 25, 33, 32);

int16_t AccX, AccY, AccZ;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);   // I2C for MPU6050

  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("System Starting");
  delay(1500);
  lcd.clear();

  // Wake up MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  lcd.print("MPU Ready");
  delay(1000);
  lcd.clear();
}

void loop() {

  // Read accelerometer data
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  AccX = Wire.read() << 8 | Wire.read();
  AccY = Wire.read() << 8 | Wire.read();
  AccZ = Wire.read() << 8 | Wire.read();

  Serial.print("X: "); Serial.print(AccX);
  Serial.print(" Y: "); Serial.print(AccY);
  Serial.print(" Z: "); Serial.println(AccZ);

  // Display values
  lcd.setCursor(0, 0);
  lcd.print("X:");
  lcd.print(AccX);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Y:");
  lcd.print(AccY);
  lcd.print("   ");

  // Fall detection condition
  if (abs(AccX) > 15000 || abs(AccY) > 15000) {

    tone(BUZZER_PIN, 1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FALL DETECTED!");
    lcd.setCursor(0, 1);
    lcd.print("Alerting...");
    
    delay(2000);  // show alert for 2 seconds
  }
  else {
    noTone(BUZZER_PIN);
  }

  delay(500);
}
