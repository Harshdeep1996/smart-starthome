#include "index.h"
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "harsh-iot-project";
const char *password = "smartplugproject";

ESP8266WebServer server(80);

/* Default IP is 192.168.4.1 - This is the IP address accessed by the device  */
void handleRoot() {
  String s = main_html;
  server.send(200, "text/html", s);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.print("This is ssid: ");
  Serial.println(ssid);

  // Getting the IP - this is redundant
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  //server.on("/page1", firstPage)

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
