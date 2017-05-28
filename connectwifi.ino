#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiClient.h>

void setup() {
    Serial.begin(115200);

    WiFiManager wifiManager;
    // This is to add a new parameter for recognizing the device name
    WiFiManagerParameter *plug_name = new WiFiManagerParameter("plug_name", "Name for plug (Mandatory section)", NULL, 100);
    WiFiManagerParameter *user_name = new WiFiManagerParameter("user_name", "Name of user (Mandatory section)", NULL, 100);
    // Clears up the EEPROM settings for wifi credentials
    wifiManager.resetSettings();
    wifiManager.addParameter(plug_name);
    wifiManager.addParameter(user_name);
    // Creates an Access Point
    wifiManager.autoConnect("Connect_Smart_Plug");

    Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
    Serial.printf("Name of plug: %s\n", plug_name->getValue());
    Serial.printf("Name of plug: %s\n", user_name->getValue());
    Serial.println("Connected to some WiFi!");
}

void loop() {
    // put your main code here, to run repeatedly:
}
