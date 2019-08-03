#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <stdio.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);//lcd 초기설정
SoftwareSerial HC12(9, 6);

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  lcd.begin();//lcd 초기화
  lcd.backlight();//백라이트 키기
  lcd.setCursor(0, 0);
  lcd.print(" Current Speed");
}

void loop() {
  int Value = analogRead(A2);
  float WindSpeed = Value * (5.0 / 1023.0) * 6;
  if (WindSpeed < 0.15) {
    WindSpeed = 0;
  }
  HC12.print(WindSpeed);
  if (WindSpeed < 10) {
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(3, LOW);
  }
  if (WindSpeed >= 10 && WindSpeed < 15) {
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(3, HIGH);
    delay(1000);
    digitalWrite(3, LOW);
  }
  if (WindSpeed >= 15) {
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(3, HIGH);
  }
  lcd.setCursor(0, 1);
  Serial.println(WindSpeed);
  lcd.printf("     %02dm/s", (int)WindSpeed);
  delay(300);
}
