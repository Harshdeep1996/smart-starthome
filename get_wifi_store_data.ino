#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <String.h>

void setup() {
    Serial.begin(115200);

     WiFiManager wifiManager;
    // This is to add a new parameter for recognizing the device name
    WiFiManagerParameter *plug_name = new WiFiManagerParameter("plug_name", "Plug Name (Mandatory section)", NULL, 100);
    WiFiManagerParameter *email= new WiFiManagerParameter("email", "Email (Mandatory section)", NULL, 100);
    // Clears up the EEPROM settings for wifi credentials
    // wifiManager.resetSettings();
    wifiManager.addParameter(plug_name);
    wifiManager.addParameter(email);
    // Creates an Access Point
    wifiManager.autoConnect("Connect_Smart_Plug");
    
    Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
    Serial.printf("Name of plug: %s\n", plug_name->getValue());
    Serial.printf("Email of user: %s\n", email->getValue());
    Serial.println("Connected to some WiFi!");

    // Use WiFiClient class to create TCP connections
    Serial.println("First before setting up client");
    const char* host = "harshcs1996.cloudant.com";
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    Serial.println("Before sending up the request");
    client.print("GET /smartplug/");
    client.println("ha@gmail.com HTTP/1.1");
    client.println("Host: harshcs1996.cloudant.com");
    client.println("Authorization: Basic aGFyc2hjczE5OTY6SGFyc2gxOTk2IQ==");
    client.println("");

    Serial.println("Ending the request");
    
//    // We now create a URI for the request
//    String url = "/smartplug";
//    Serial.print("Requesting URL: ");
//    Serial.println(url);
    Serial.println("Lets see what we have recieved.");
//    StaticJsonBuffer<200> jsonBuffer;
//    JsonObject& data = jsonBuffer.createObject();
//    data["_id"] = email->getValue();
//    data["name"] = plug_name->getValue();
//    JsonArray& plugs = data.createNestedArray("plugs");
//    JsonObject &plug = jsonBuffer.createObject();
//    plug["plug_id"] = ESP.getChipId();
//    plug["status"] = "off";
//    plugs.add(plug);
//
//    String sysdata;
//    data.printTo(sysdata);
//    int content_length = sysdata.length();
//    client.println("POST /smartplug HTTP/1.1");
//    client.println("Host: harshcs1996.cloudant.com");
//    client.println("Authorization: Basic aGFyc2hjczE5OTY6SGFyc2gxOTk2IQ==");
//    client.print("Accept: *");
//    client.print("/");
//    client.println("*");
//    client.print("Content-Length:");
//    client.println(content_length);
//    client.println("Content-Type: application/json");
//    client.println("");
//    client.println(sysdata);
//
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 10000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    
    // Read all the lines of the reply from server and print them to Serial
    String data;
    while(client.available()){
      data = client.readStringUntil('\r');
      Serial.println(data);
    }

    // Keep sufficient memory, this is what was hindering the distance before.
    StaticJsonBuffer<1024> jsonBuffer;

    JsonObject& plug_data = jsonBuffer.parseObject(data);

    Serial.println(plug_data.containsKey("plugs"));

    if (plug_data.containsKey("plugs")) {
      Serial.println("Found object2 !");
    } else {
      Serial.println("Found nothing");
    }
//
//    Serial.println(user_there);
//    if(!user_there) {
//      // Then make new doc, make use of POST client
//      Serial.println("A new doc needs to be made.");
//    } else {
//      // Then check if the plug with the same id is there, if yes. then do nothing
//      // otherwise append, it to the plugs list as a new plug with status off.
//      Serial.println("These strings are the same. No new entry to be made.");
//    }
    
    Serial.println();
    Serial.println("closing connection");
}

void loop() {
  // put your main code here, to run repeatedly:

}