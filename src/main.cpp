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

#define TFT_LIGHT 16

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

Screen *pIdleScreen = createIdleScreen();
Screen *pAlarmSetScreen = createAlarmSetScreen();
Screen *pRemoteScreen = createRemoteScreen();

TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60}; //British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};  //Standard Time
Timezone UK(BST, GMT);

TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120}; //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};   //Central European Standard Time
Timezone CE(CEST, CET);

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");  

  Serial1.begin(38400);
  Serial.println("Serial1 initialised");

  pinMode(TFT_LIGHT, OUTPUT);
  digitalWrite(TFT_LIGHT, HIGH);

  if (touch.begin())
  {
    Serial.println("Touch initialised");
  }
  else
  {
    Serial.println("Touch initialisation failed");
  }

  if (SPIFFS.begin())
  {
    Serial.println("SPIFFS initialised");
  }
  else
  {
    Serial.println("SPIFFS failed");
  }

  /*timeClient.begin();
  Serial.print("Getting time ... ");
  timeClient.forceUpdate();
  Serial.println(timeClient.getFormattedTime());
  RTC.set(timeClient.getEpochTime());*/

  /*Dir dir = SPIFFS.openDir("/data");
  while (dir.next()) {
    Serial.print(dir.fileName());
    File f = dir.openFile("r");
    Serial.println(f.name());
  }*/


  //calibrate(touch);
  loadCalibration();

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
    }
  } else {
    // The buffer is empty, the user is no longer touching the screen, fire UP if not already up
    if (touched_previously) {
      EventManager.queueEvent(new TouchEvent(last_x, last_y, false));
      touched_previously = false;
    }
  }
}

void loop()
{
  /*int n = analogRead(A0);
    Serial.println(n);*/

  processTouch();

  /*while (Serial.available())
  {
    int n = Serial.read();
    switch (n)
    {
    case '1':
      analogWrite(TFT_LIGHT, 900);
      break;
    case '0':
      analogWrite(TFT_LIGHT, 100);
      break;
    default:
      Serial1.write(n);
      break;
    }
    Serial.write(n);
  }*/

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
