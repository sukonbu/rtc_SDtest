#ifndef INCLUDED_GLOBAL
#define INCLUDED_GLOBAL

#ifdef GLOBAL_VALUE_DEFINE
  #define GLOBAL
  #define GLOBAL_VAL(v) = (v)
#else
  #define GLOBAL extern
  #define GLOBAL_VAL(v)
#endif

#include <Wire.h>
#include <RTClib.h>
#include <rgb_lcd.h>
#include <SPI.h>
#include <SD.h>

#include "LCD.h"

// define setting value
#define SENSING_DELAY 1000
#define SETTING_WAIT 3000
#define BUTTON_SETTING_PIN 6
#define BUTTON_SELECT_PIN 8
#define SD_CS_PIN 4


// define about logfile
#define FILE_NAME "test"
#define FILE_TYPE "csv"

GLOBAL RTC_DS1307 rtc;
GLOBAL rgb_lcd lcd;
GLOBAL DateTime now;
GLOBAL String dic;
GLOBAL String filename;

#endif
