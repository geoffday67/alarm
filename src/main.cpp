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

#define TFT_LIGHT 16

extern void bmpDraw(String filename, int16_t x, int16_t y);
extern void wifi_init(void);
extern void wifi_remote(const int channel, const char *command);
extern Screen* createIdleScreen (Output *pout);
extern Screen* createAlarmSetScreen (Output *pout);
extern Screen* createRemoteScreen (Output *pout);

classEventManager EventManager;

Adafruit_STMPE610 touch = Adafruit_STMPE610();
bool touched = false;
int last_x, last_y;
time_t lastTime;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

Output output;
Screen *pIdleScreen = createIdleScreen(&output);
Screen *pAlarmSetScreen = createAlarmSetScreen(&output);
Screen *pRemoteScreen = createRemoteScreen(&output);

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
  }
  bmpDraw ("/boilme.bmp", 0, 40);*/

  pIdleScreen->activate();
}

void loop()
{
  /*int n = analogRead(A0);
    Serial.println(n);*/

  uint16_t raw_x, raw_y;
  uint8_t raw_z;

  // Check for a touch event (down or up), fire event
  if (touch.touched()) {
    while (!touch.bufferEmpty()) {
      touch.readData(&raw_x, &raw_y, &raw_z);

      if (!touched) {
        last_x = raw_y / 12.8;
        last_y = (4096 - raw_x) / 17;
        EventManager.fireEvent(EVENT_TOUCH, new TouchEvent(last_x, last_y, true));
        touched = true;
      }
    }
  } else {
    if (touched) {
      EventManager.fireEvent(EVENT_TOUCH, new TouchEvent(last_x, last_y, false));
      touched = false;
    }
  }

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
    EventManager.fireEvent(EVENT_TIME, &local);
    lastTime = utc;
  }

  delay(10);
}
