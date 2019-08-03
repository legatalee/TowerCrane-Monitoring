#include <Wire.h>
#include <MechaQMC5883.h>
#include<LiquidCrystal_I2C.h>
#include<math.h>

MechaQMC5883 qmc;
LiquidCrystal_I2C lcd(0x27, 16, 2);//lcd 초기설정

int north = 0;

int range1, range2, range3, range4, range5, range6;
int showrange1, showrange2, showrange3, showrange4, showrange5, showrange6;

void setup() {
  Serial.begin(9600);

  pinMode(3, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  Wire.begin();
  lcd.begin();
  qmc.init();
  lcd.setCursor(0, 0);
}

void loop() {

  int x, y, z;
  int azimuth;
  //float azimuth; //is supporting float too
  qmc.read(&x, &y, &z, &azimuth);
  //  azimuth = qmc.azimuth(&y, &x); //you can get custom azimuth
  azimuth = (360 - north + azimuth) % 360;

  range1 = analogRead(A2);
  range2 = analogRead(A3);
  range3 = analogRead(A6);
  range4 = analogRead(A7);
  range5 = analogRead(A0);
  range6 = analogRead(A1);

  range1 = map(range1, 0, 1023, 360, -359);
  range2 = map(range2, 0, 1023, -359 , 360);
  range3 = map(range3, 0, 1023, 360, -359);
  range4 = map(range4, 0, 1023, -359 , 360);
  range5 = map(range5, 0, 1023, 360, -359);
  range6 = map(range6, 0, 1023, -359 , 360);
  showrange1 = range1;
  showrange2 = range2;
  showrange3 = range3;
  showrange4 = range4;
  showrange5 = range5;
  showrange6 = range6;

  if (range1 > 0) {
    range1 = 360 - range1;
  }
  if (range1 < 0) {
    range1 = abs(range1);
  }
  if (range2 < 0) {
    range2 += 360;
  }
  if (range3 > 0) {
    range3 = 360 - range3;
  }
  if (range3 < 0) {
    range3 = abs(range3);
  }
  if (range4 < 0) {
    range4 += 360;
  }
  if (range5 > 0) {
    range5 = 360 - range5;
  }
  if (range5 < 0) {
    range5 = abs(range5);
  }
  if (range6 < 0) {
    range6 += 360;
  }
  range1 = range1 / 10 * 10;
  range2 = range2 / 10 * 10;
  range3 = range3 / 10 * 10;
  range4 = range4 / 10 * 10;
  range5 = range5 / 10 * 10;
  range6 = range6 / 10 * 10;

  lcd.setCursor(0, 0);
  lcd.print(range1);
  lcd.print("     ");
  lcd.setCursor(4, 0);
  lcd.print(range2);
  lcd.print("     ");
  lcd.setCursor(8, 0);
  lcd.print(range3);
  lcd.print("     ");
  lcd.setCursor(12, 0);
  lcd.print(range4);
  lcd.print("     ");

  lcd.setCursor(0, 1);
  lcd.print(range5);
  lcd.print("     ");
  lcd.setCursor(4, 1);
  lcd.print(range6);
  lcd.print("     ");


  Serial.print("range1: ");
  Serial.print(range1);
  Serial.print(", range2: ");
  Serial.print(range2);
  Serial.print("range3: ");
  Serial.print(range3);
  Serial.print(", range4: ");
  Serial.print(range4);
  Serial.print("range5: ");
  Serial.print(range5);
  Serial.print(", range6: ");
  Serial.println(range6);

  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  if (digitalRead(6) == HIGH) {
    if (range1 > range2) {
      if (((range1 <= azimuth) && (azimuth <= 359)) || (0 <= azimuth) && (azimuth <= range2)) {
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        delay(650);
        digitalWrite(4, LOW);
        delay(350);
      } else {
      }
    }
    if (range1 < range2) {
      if ((range1 <= azimuth) && (range2 >= azimuth)) {
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        delay(650);
        digitalWrite(4, LOW);
        delay(350);
      } else {
      }
    }
  }
  if (digitalRead(7) == HIGH) {
    if (range3 > range4) {
      if (((range3 <= azimuth) && (azimuth <= 359)) || (0 <= azimuth) && (azimuth <= range4)) {
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        delay(650);
        digitalWrite(4, LOW);
        delay(350);
      } else {
      }
    }
    if (range3 < range4) {
      if ((range3 <= azimuth) && (range4 >= azimuth)) {
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        delay(650);
        digitalWrite(4, LOW);
        delay(350);
      } else {
      }
    }
  }
  if (digitalRead(8) == HIGH) {
    if (range5 > range6) {
      if (((range5 <= azimuth) && (azimuth <= 359)) || (0 <= azimuth) && (azimuth <= range6)) {
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        delay(650);
        digitalWrite(4, LOW);
        delay(350);
      } else {
      }
    }
    if (range5 <= range6) {
      if ((range5 <= azimuth) && (range6 >= azimuth)) {
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        delay(650);
        digitalWrite(4, LOW);
        delay(350);
      } else {
      }
    }
  }
  lcd.setCursor(11, 1);
  lcd.print("n:");
  lcd.print(azimuth);
  lcd.print("     ");

  delay(100);
}

