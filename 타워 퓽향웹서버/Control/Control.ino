#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

char ssid[] = "iptime";//ssid 이름
char pass[] = "";//비밀번호
int keyIndex = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);//lcd 초기설정
SoftwareSerial HC12(D3, D4);

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  HC12.begin(9600);
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  status = WiFi.begin(ssid, pass);
  delay(5000);
  server.begin();
  // you're connected now, so print out the status:
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  lcd.begin();//lcd 초기화
  lcd.backlight();//백라이트 키기
  lcd.setCursor(0, 0);
  printWifiStatus();
}
float WindSpeed;
void loop() {
  String input = "";//input 초기화
  while (HC12.available() > 0)//HC12 신호감지시
  {
    input += (char) HC12.read();//string형 input에 char형식으로 한글자씩 저장
    delay(1);
    Serial.println(input);
    WindSpeed = input.toFloat();
  }
  lcd.setCursor(0, 1);
  lcd.printf(" %02dm/s", (int)WindSpeed);
  
  if (WindSpeed < 10) {
    digitalWrite(D5 , LOW);
    digitalWrite(D6 , LOW);
    digitalWrite(D7 , LOW);
    digitalWrite(D8 , HIGH);
  }
  if ((WindSpeed >= 10) && (WindSpeed < 15)) {
    digitalWrite(D6 , LOW);
    digitalWrite(D7 , HIGH);
    digitalWrite(D8 , LOW);
    digitalWrite(D5 , HIGH);
    delay(1000);
    digitalWrite(D5 , LOW);
  }
  if (WindSpeed >= 15) {
    digitalWrite(D5 , HIGH);
    digitalWrite(D6 , HIGH);
    digitalWrite(D7 , LOW);
    digitalWrite(D8 , LOW);
  }

  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      char c = client.read();
      Serial.write(c);
      if (c == '\n' && currentLineIsBlank) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println("Refresh: 1");
        client.println();
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");

        client.println("<div style =\"text-align:center\">");
        client.println("<font size=800>");
        client.println("Current Wind Speed");
        client.println("</font>");
        client.println("<br /><br />");
        client.println("<font size=1000>");
        if (WindSpeed < 10) {
          client.print("<font color=#52E252>");
          client.print(WindSpeed);
          client.print("</font>");
        }
        if ((WindSpeed >= 10) && (WindSpeed < 15)) {
          client.print("<font color=#FFAF00>");
          client.print(WindSpeed);
          client.print("</font>");
        }
        if (WindSpeed >= 15) {
          client.print("<font color=#FF0000>");
          client.print(WindSpeed);
          client.print("</font>");
        }
        client.println(" m/s");
        client.println("</font>");
        client.println("<br />");
        client.println("</div >");


        client.println("</html>");
        break;
      }
      if (c == '\n') {
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
    }
    delay(1);
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  lcd.print(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

