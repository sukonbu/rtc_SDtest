#include <Wire.h>
#include <RTClib.h>
#include <rgb_lcd.h>
#include <SPI.h>
#include <SD.h>

#define SENSING_DELAY 1000
#define SETTING_WAIT 3000
#define BUTTON_PIN 6

#define FILE_TYPE "csv"
#define FILE_NAME "test"


RTC_DS1307 rtc;
rgb_lcd lcd;
DateTime now;
String filename = String(FILE_NAME) + String(".") +  String(FILE_TYPE);
void initLCD();
void updateLCD();
void settingLoop();
void modeCheck();

void setup () {
  while (!Serial);
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  initLCD();

  modeCheck();

  delay(2000);
  lcd.setRGB(255,255,255);
}

void loop () {
    Serial.print(now.year());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.print(now.day());
    Serial.print(',');
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.println();
    updateLCD();
    delay(SENSING_DELAY);
}

void logcsv(String dataString){
  File dataFile = SD.open(filename, FILE_WRITE);
  if(dataFile){
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
}

void initLCD(){
  lcd.begin(16,2);
  lcd.setRGB(0,255,0);
  lcd.print("UV Sensing Ready");
  lcd.clear();
  now = rtc.now();
  if(now.month() < 10)lcd.print('0');
  lcd.print(now.month());
  lcd.setCursor(2, 0);
  lcd.print('/');
  if(now.day() < 10)lcd.print('0');
  lcd.print(now.day());
  lcd.setCursor(5, 0);
  //lcd.print(',');
  lcd.print(' ');
  if(now.hour() < 10)lcd.print('0');
  lcd.print(now.hour());
  lcd.setCursor(8, 0);
  lcd.print(':');
  if(now.minute() < 10)lcd.print('0');
  lcd.print(now.minute());
  lcd.setCursor(11, 0);
  lcd.print(':');
  if(now.second() < 10)lcd.print('0');
  lcd.print(now.second());
}

void updateLCD(){
  now = rtc.now();
  lcd.setCursor(0, 0);
  if(now.month() < 10)lcd.print('0');
  lcd.print(now.month());
  lcd.setCursor(3, 0);
  if(now.day() < 10)lcd.print('0');
  lcd.print(now.day());
  lcd.setCursor(6, 0);
  if(now.hour() < 10)lcd.print('0');
  lcd.print(now.hour());
  lcd.setCursor(9, 0);
  if(now.minute() < 10)lcd.print('0');
  lcd.print(now.minute());
  lcd.setCursor(12, 0);
  if(now.second() < 10)lcd.print('0');
  lcd.print(now.second());
}

void modeCheck(){
  int countl = 0;
  while(countl < SETTING_WAIT){
    if(digitalRead(BUTTON_PIN)){
      Serial.println("setting mode");
      settingLoop();
    }
    countl++;
  }
}

void settingLoop(){
  while(true){
    //send serial message to PC
  }
}
