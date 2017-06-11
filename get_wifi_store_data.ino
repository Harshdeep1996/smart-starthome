#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <String.h>

#define MAX_PLUG_COUNT 10

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

    WiFiClient get_client;
    get_client = create_client();
    if (!get_client) {
      return;
    }
    get_request(get_client, email);

    unsigned long timeout = millis();
    while (get_client.available() == 0) {
      if (millis() - timeout > 10000) {
        Serial.println(">>> Client Timeout !");
        get_client.stop();
        return;
      }
    }

    // Read all the lines of the reply from server until carriage
    String get_data;
    while(get_client.available()){
      get_data = get_client.readStringUntil('\r');
      Serial.println(get_data);
    }

    // Keep sufficient memory, this is what was hindering the distance before.
    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& plug_data = jsonBuffer.parseObject(get_data);
    Serial.println(plug_data.containsKey("plugs"));

    int chip_id = ESP.getChipId();
    Serial.println("This is the chip id");
    Serial.println(chip_id);
    Serial.println("ChipId has been printed above");
    bool id_present = false;
    if (plug_data.containsKey("plugs"))
    {
      Serial.println("Found object2 !");
      
      JsonArray& plugs =  plug_data["plugs"];
      int arraySize = plug_data["plugs"].size();

      for (int i = 0; i< arraySize; i++){
        int sensorValue=plug_data["plugs"][i]["plug_id"];
        Serial.println(sensorValue);
      }
    }
    else
    {
      Serial.println("Found nothing");
    }

    Serial.println();
    Serial.println("Closing get request");
//    
//    // We now create a URI for the request
//    String url = "/smartplug";
//    Serial.print("Requesting URL: ");
//    Serial.println(url);
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
}

WiFiClient create_client() {
  // Use WiFiClient class to create TCP connections
  const char* host = "harshcs1996.cloudant.com";
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return client;
  }
  return client;
}

void get_request(WiFiClient client, WiFiManagerParameter* email) {
  client.print("GET /smartplug/");
  client.print("harshcs.1996@gmail.com");
//  client.print(email->getValue());
  client.println(" HTTP/1.1");
  client.println("Host: harshcs1996.cloudant.com");
  client.println("Authorization: Basic aGFyc2hjczE5OTY6SGFyc2gxOTk2IQ==");
  client.println("");
}

void loop() {
  // put your main code here, to run repeatedly:

}