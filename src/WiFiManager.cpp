#include "WiFiManager.h"

classWiFiManager WiFiManager;

#define HOST "192.168.1.5"
#define PORT 1968
#define URI "/interface"

void classWiFiManager::sendCommand(int channel, const char* command) {
  char body [64];

  sprintf (body, "operation=433&channel=%d&command=%s", channel, command);
  Serial.print ("Sending: ");
  Serial.println (body);
  client.begin(HOST, PORT, URI);
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");
  client.addHeader("Authorization", "Basic d2FyaW86bWFuc2lvbjE=");
  client.POST(body);
  Serial.print ("Received: ");
  Serial.println(client.getString());
}