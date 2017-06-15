#include <Wire.h>
#include <RTClib.h>
#include <rgb_lcd.h>
#include <SPI.h>
#include <SD.h>

// define setting value
#define SENSING_DELAY 1000
#define SETTING_WAIT 3000
#define BUTTON_PIN 6
#define SD_CS_PIN 4

// define about logfile
#define FILE_NAME "test"
#define FILE_TYPE "csv"

RTC_DS1307 rtc;
rgb_lcd lcd;
DateTime now;
String filename = String(FILE_NAME) + String(".") +  String(FILE_TYPE);

// function prototype
void debugSerialPrint();
void initLCD();
void updateLCD();
void logcsv(String);
void initSD();
void settingLoop();
void modeCheck();

void setup () {
  //init RTC
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
  // init LCD
  initLCD();
  delay(SETTING_WAIT);
  // check device mode
  modeCheck();
  // init SDcard
  initSD();
  delay(2000);
  lcd.setRGB(255,255,255);
}

void loop () {
  // create dataString(ex. 2017/6/7,12:20:00)
  String dataString = String(now.year());
  dataString+="/";
  dataString+=String(now.month());
  dataString+="/";
  dataString+=String(now.day());
  dataString+=",";
  dataString+=String(now.hour());
  dataString+=":";
  dataString+=String(now.minute());
  dataString+=":";
  dataString+=String(now.second());
  // logging SDcard
  logcsv(dataString);
  // update LCD display
  updateLCD();
  // print dataString (test function)
  debugSerialPrint();
  // delay loop
  delay(SENSING_DELAY);
}

void initLCD(){
  // initialize LCD
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

void initSD(){
  if(!SD.begin(SD_CS_PIN)){
    Serial.println("Card failed, or not present");
    lcd.clear();
    lcd.print("SD_ERROR");
    while(true);
  }
  Serial.println("card initialized.");
}

void logcsv(String dataString){
  File dataFile = SD.open(filename, FILE_WRITE);
  if(dataFile){
    dataFile.println(dataString);
    dataFile.close();
  }
}

void modeCheck(){
  int countl = 0;
  while(countl < SETTING_WAIT){
    if(digitalRead(BUTTON_PIN)){
      Serial.println("setting mode");
      lcd.clear();
      lcd.setRGB(0,100,255);
      lcd.setCursor(0, 1);
      lcd.print("SETTING_MODE");
      settingLoop();
    }
    countl++;
  }
}

void settingLoop(){
  while(true){
    //send serial message to PC
    Serial.println("uv");
    delay(500);
    if(Serial.available() > 0){
      int dt[6]; // Y m d H M S
      for(int i = 0; i < 6;i++){
        dt[i] = Serial.readStringUntil(',').toInt();
      }
      lcd.setCursor(0, 0);
      //lcd.print(buf)A;
      //Serial.println(buf);
      if(dt[1] < 10)lcd.print('0');
      lcd.print(dt[1]);
      lcd.print("/");
      if(dt[2] < 10)lcd.print('0');
      lcd.print(dt[2]);
      lcd.print(" ");
      if(dt[3] < 10)lcd.print('0');
      lcd.print(dt[3]);
      lcd.print(":");
      if(dt[4] < 10)lcd.print('0');
      lcd.print(dt[4]);
      lcd.print(":");
      if(dt[5] < 10)lcd.print('0');
      lcd.print(dt[5]);
      rtc.adjust(DateTime(dt[0],dt[1],dt[2],dt[3],dt[4],dt[5]+3));
      break;
    }
  }
  Serial.println("end");
  while(true){
    updateLCD();
    delay(SENSING_DELAY);
  }
}

void debugSerialPrint(){
  // print dataString(ex. 2017/6/7,12:20:00)
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
}
