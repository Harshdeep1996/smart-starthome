#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

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
    Serial.printf("Email of user: %s\n", user_name->getValue());
    Serial.println("Connected to some WiFi!");

    // Use WiFiClient class to create TCP connections
    const char* host = "harshcs1996.cloudant.com";
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
    
    // We now create a URI for the request
    String url = "/smartplug";
    Serial.print("Requesting URL: ");
    Serial.println(url);

    char data[1024];
    int content_length;
    content_length = sprintf(data, "{\"_id\": \"%s\", \"name\": \"%s\", \"status\": \"off\"}", user_name->getValue(), plug_name->getValue());
    client.println("POST /smartplug HTTP/1.1");
    client.println("Host: harshcs1996.cloudant.com");
    client.println("Authorization: Basic aGFyc2hjczE5OTY6SGFyc2gxOTk2IQ==");
    client.print("Accept: *");
    client.print("/");
    client.println("*");
    client.print("Content-Length:");
    client.println(content_length);
    client.println("Content-Type: application/json");
    client.println("");
    client.println(data);

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 10000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    
    Serial.println();
    Serial.println("closing connection");
}

void loop() {
  // put your main code here, to run repeatedly:

}