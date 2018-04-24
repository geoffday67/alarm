#ifndef __WIFI_MANAGER__
#define __WIFI_MANAGER__

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

class classWiFiManager
{
    private:
        HTTPClient client;
    
    public:
        void sendCommand(int channel, const char* command);
};

extern classWiFiManager WiFiManager;

#endif