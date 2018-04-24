/*
TODO - SOFTWARE
Settings (none critial)
  - time zone
  - snooze length
  - high/low brightness and thresholds
  - WiFi credentials

TODO - HARDWARE
Make reset(s) available outside
Clean up case
*/

#include <Adafruit_STMPE610.h>
#include "FS.h"
#include <DS1307RTC.h>
#include <Time.h>
#include <Timezone.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "EventManager.h"
#include "Screen.h"
#include "Output.h"
#include "Button.h"
#include "AlarmManager.h"

#define TFT_LIGHT_OUTPUT    16
#define TFT_DARK_THRESHOLD  500
#define TFT_LIGHT_THRESHOLD 700
#define TFT_LIGHT_LOW       0
#define TFT_LIGHT_HIGH      800
#define TFT_LIGHT_TEMP      100
#define LIGHT_TEMP_PERIOD   5000

extern void calibrate(Adafruit_STMPE610& touch);
extern void loadCalibration();
extern void transform(int raw_x, int raw_y, int* pscreen_x, int* pscreen_y);

extern void wifi_init(void);
extern void wifi_remote(const int channel, const char *command);
extern Screen* createIdleScreen ();
extern Screen* createAlarmSetScreen ();
extern Screen* createRemoteScreen ();

Adafruit_STMPE610 touch = Adafruit_STMPE610();
int last_x, last_y;
time_t lastTime;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
int lightTimer;

Screen *pIdleScreen = createIdleScreen();
Screen *pAlarmSetScreen = createAlarmSetScreen();
Screen *pRemoteScreen = createRemoteScreen();

TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60}; //British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};  //Standard Time
Timezone UK(BST, GMT);

TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120}; //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};   //Central European Standard Time
Timezone CE(CEST, CET);

bool isDark = false;

void listFiles(void) {
  Serial.println("SPIFFS files found:");

  fs::Dir dir = SPIFFS.openDir("/"); // Root directory
  String  line = "=====================================";

  Serial.println(line);
  Serial.println("  File name               Size");
  Serial.println(line);

  while (dir.next()) {
    String fileName = dir.fileName();
    Serial.print(fileName);
    int spaces = 25 - fileName.length(); // Tabulate nicely
    if (spaces < 0) spaces = 1;
    while (spaces--) Serial.print(" ");
    fs::File f = dir.openFile("r");
    Serial.print(f.size()); Serial.println(" bytes");
    yield();
  }

  Serial.println(line);
}

void printTime(time_t time) {
  tmElements_t elements;
  breakTime(time, elements);

  char text [32];
  sprintf (text, "%02d-%02d-%04d %02d:%02d:%02d", elements.Day, elements.Month, elements.Year + 1970, elements.Hour, elements.Minute, elements.Second);

  Serial.println(text);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");  

  Serial1.begin(38400);
  Serial.println("Serial1 initialised");

  pinMode(TFT_LIGHT_OUTPUT, OUTPUT);
  analogWrite (TFT_LIGHT_OUTPUT, TFT_LIGHT_HIGH);
  Serial.println("Initial light level set");

  if (touch.begin()) {
    Serial.println("Touch initialised");
  } else {
    Serial.println("Touch initialisation failed");
  }

  if (SPIFFS.begin()) {
    Serial.println("SPIFFS initialised");
  } else {
    Serial.println("SPIFFS failed");
  }
  listFiles();

  Output.begin();
  Serial.println("Output initialised");

  timeClient.begin();
  timeClient.setUpdateInterval(600000L);
  Serial.println("NTP initialised");

  //calibrate(touch);
  loadCalibration();
  Serial.println("Calibration loaded");

  lastTime = RTC.get();
  time_t local = UK.toLocal(lastTime);
  AlarmManager.setCurrentTime(local);
  Serial.print("Local time: ");
  printTime(local);

  // TESTING
  /*const Alarm* pnext_alarm = AlarmManager.getNextAlarm();
  time_t alarm_time = AlarmManager.getAlarmTime(pnext_alarm);
  Serial.print("Alarm: ");
  printTime(alarm_time);
  RTC.set(UK.toUTC(alarm_time - 5));
  lastTime = RTC.get();
  local = UK.toLocal(lastTime);
  AlarmManager.setCurrentTime(local);*/
  // TESTING

  pIdleScreen->activate();
}

void processTouch() {
  static bool touched_previously = false;

  // The last touched position
  static int last_x = -1;
  static int last_y = -1;

  // Update 'last touched' to most recent touch position, if any
  uint16_t raw_x, raw_y;
  uint8_t raw_z;
  int this_x = -1;
  int this_y = -1;
  bool buffer_empty;
  while (!touch.bufferEmpty()) {
    touch.readData(&raw_x, &raw_y, &raw_z);
    transform(raw_x, raw_y, &this_x, &this_y);
  }

  // Record if there were any points in buffer
  buffer_empty = (this_x == -1 && this_y == -1);

  if (!buffer_empty) {
    last_x = this_x;
    last_y = this_y;

    // If the buffer is not empty then a touch event happened, fire DOWN if not already down
    if (!touched_previously) {
      EventManager.queueEvent(new TouchEvent(last_x, last_y, true));
      touched_previously = true;

      if (isDark) {
        analogWrite (TFT_LIGHT_OUTPUT, TFT_LIGHT_TEMP);
        lightTimer = millis();
      }
    }
  } else {
    // The buffer is empty, the user is no longer touching the screen, fire UP if not already up
    if (touched_previously) {
      EventManager.queueEvent(new TouchEvent(last_x, last_y, false));
      touched_previously = false;
    }
  }
}

void processLight() {
  if (lightTimer > 0) {
    if (millis() - lightTimer >= LIGHT_TEMP_PERIOD) {
      if (isDark) {
        analogWrite (TFT_LIGHT_OUTPUT, TFT_LIGHT_LOW);
      }
      lightTimer = 0;
    }
  }

  int level = analogRead(A0);

  if (isDark) {
    if (level > TFT_LIGHT_THRESHOLD) {
      isDark = false;
      lightTimer = 0;
      Serial.println("Light level now LIGHT");
      analogWrite (TFT_LIGHT_OUTPUT, TFT_LIGHT_HIGH);
      EventManager.queueEvent(new LightEvent(false));
    }
  } else {
    if (level < TFT_DARK_THRESHOLD) {
      isDark = true;
      Serial.println("Light level now DARK");
      EventManager.queueEvent(new LightEvent(true));
      analogWrite (TFT_LIGHT_OUTPUT, TFT_LIGHT_LOW);
    }
  }
}

void loop()
{
  processLight();
  processTouch();

  /*while (Serial.available())
  {
    int n = Serial.read();
    switch (n)
    {
    case '1':
      analogWrite(TFT_LIGHT_OUTPUT, TFT_LIGHT_HIGH);
      break;
    case '0':
      analogWrite(TFT_LIGHT_OUTPUT, TFT_LIGHT_LOW);
      break;
    default:
      Serial1.write(n);
      break;
    }
    Serial.write(n);
  }*/

  if (timeClient.asyncUpdate()) {
    Serial.print ("Time updated: ");
    Serial.println(timeClient.getFormattedTime());
    RTC.set(timeClient.getEpochTime());
  }
  
  // Fire time event once a second
  time_t utc = RTC.get();
  if (utc != lastTime) {

    time_t local = UK.toLocal(utc);
    AlarmManager.setCurrentTime(local);
    EventManager.queueEvent(new TimeEvent(local));
    lastTime = utc;
  }

  EventManager.processEvents();

  delay(100);
}
