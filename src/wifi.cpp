#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define SSID "Wario"
#define PASSWORD "mansion1"

#define HOST "192.168.1.5"
#define PORT 1968
#define URI "/interface"

HTTPClient client;

void wifi_init(void)
{
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print(" connected, IP ");
  Serial.println(WiFi.localIP());
}

void wifi_remote (const int channel, const char *command) {
  char body [64];

  sprintf (body, "operation=433&channel=%d&command=%s", channel, command);
  Serial.print ("Sending: ");
  Serial.println (body);
  client.begin(HOST, PORT, URI);
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");
  client.POST(body);
  Serial.print ("Received: ");
  Serial.println(client.getString());
}
