#include "global.h"

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
