#define GLOBAL_VALUE_DEFINE
#include "global.h"

// function prototype
void debugSerialPrint();
void logcsv(String);
void initSD();
void settingLoop();
String createDataString();
String createFilenameString();
void createFileDic();
void modeCheck();

void setup () {
  while (!Serial);
  //init RTC
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
  now = rtc.now();
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
  // logging SDcard
  logcsv(createDataString());
  // update LCD display
  updateLCD();
  // print dataString (test function)
  debugSerialPrint();
  // delay loop
  delay(SENSING_DELAY);
}

void initSD(){
  if(!SD.begin(SD_CS_PIN)){
    Serial.println("Card failed, or not present");
    lcd.clear();
    lcd.print("SD_ERROR");
    while(true);
  }
  Serial.println("card initialized.");
  createFileDic();
  filename = dic + createFilenameString() + String(".") +  String(FILE_TYPE);
  Serial.println(filename);
}

void logcsv(String dataString){
  File dataFile = SD.open(filename, FILE_WRITE);
  if(dataFile){
    dataFile.println(dataString);
    dataFile.close();
  }else{
    lcd.clear();
    lcd.setRGB(255, 0, 0);
    lcd.setCursor(0, 0);
    lcd.print("SD EJECTED!!");
    while(true);
  }
}

void modeCheck(){
  int countl = 0;
  while(countl < SETTING_WAIT){
    if(digitalRead(BUTTON_SETTING_PIN)){
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

String createDataString(){
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
  return dataString;
}

String createFilenameString(){
  String str = "UV";
  // now = rtc.now();
  // str += now.year();
  // if(now.month() < 10)str+="0";
  // str += now.month();
  // if(now.day() < 10)str+="0";
  // str += now.day();
  //str += "_";
  if(now.hour() < 10)str+="0";
  str += now.hour();
  if(now.minute() < 10)str += "0";
  str += now.minute();
  if(now.second() < 10)str += "0";
  str += now.second();
  return str;
}

void createFileDic(){
  String dicStr = String(now.year()) + String(now.month()) + String(now.day());
  if(!SD.exists(dicStr))SD.mkdir(dicStr);
  dic = dicStr + "/";
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
