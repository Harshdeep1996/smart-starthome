#include <String.h>
#include <ArduinoJson.h>

#include <DNSServer.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


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
    }

    // Keep sufficient memory, this is what was hindering the distance before.
    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& plug_data = jsonBuffer.parseObject(get_data);

    int chip_id = ESP.getChipId();
    bool id_present = false;
    bool plugs_present = false;
    if (plug_data.containsKey("plugs"))
    {
      plugs_present = true;
      JsonArray& plugs = plug_data["plugs"];
      int arraySize = plug_data["plugs"].size();
      Serial.println("Plugs is present");

      for (int i = 0; i< arraySize; i++) {
        int sensorValue=plug_data["plugs"][i]["plug_id"];
        if(chip_id == sensorValue) {
          Serial.println("Chip is present, no need to add it.");
          id_present = true;
        }
      }
    }
    else
    {
      Serial.println("Plugs dict does not exist..");
    }

    if(!plugs_present)
    {
      // We now create a URI for the request
      String url = "/smartplug";
      Serial.print("Requesting URL: ");
      Serial.println(url);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& user_data = jsonBuffer.createObject();
      user_data["_id"] = email->getValue();
      user_data["name"] = plug_name->getValue();
      JsonArray& add_plugs = user_data.createNestedArray("plugs");
      JsonObject &plug = jsonBuffer.createObject();
      plug["plug_id"] = ESP.getChipId();
      plug["status"] = "off";
      add_plugs.add(plug);

      String sysdata;
      user_data.printTo(sysdata);
      int content_length = sysdata.length();
      get_client.println("POST /smartplug HTTP/1.1");
      get_client.println("Host: harshcs1996.cloudant.com");
      get_client.println("Authorization: Basic aGFyc2hjczE5OTY6SGFyc2gxOTk2IQ==");
      get_client.print("Accept: *");
      get_client.print("/");
      get_client.println("*");
      get_client.print("Content-Length:");
      get_client.println(content_length);
      get_client.println("Content-Type: application/json");
      get_client.println("");
      get_client.println(sysdata);
    }

    if(plugs_present && !id_present)
    {
      JsonArray& plugs = plug_data["plugs"];
      JsonObject &new_plug = jsonBuffer.createObject();
      new_plug["plug_id"] = ESP.getChipId();
      new_plug["status"] = "off";
      plugs.add(new_plug);
      String sysdata;
      plug_data.printTo(sysdata);
      int content_length = sysdata.length();

      get_client.print("PUT /smartplug/");
      get_client.print("harshcs.1996@gmail.com");
      //client.print(email->getValue());
      get_client.println(" HTTP/1.1");
      get_client.println("Host: harshcs1996.cloudant.com");
      get_client.println("Authorization: Basic aGFyc2hjczE5OTY6SGFyc2gxOTk2IQ==");
      get_client.print("Accept: *");
      get_client.print("/");
      get_client.println("*");
      get_client.print("Content-Length:");
      get_client.println(content_length);
      get_client.println("Content-Type: application/json");
      get_client.println("");
      get_client.println(sysdata);
    }

    Serial.println();
    Serial.println("Closing get request");
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
  WiFiClient get_client;
    get_client = create_client();
    if (!get_client) {
      return;
    }
  client.print(String("GET ") + path + " HTTP/1.1\r\n" +               "Host: " + host + "\r\n" +                "Connection: keep-alive\r\n\r\n");
  delay(500); // wait for server to respond
  // read response  
  String section="header";
  while(client.available()){
    String line = client.readStringUntil('\r');
    // Serial.print(line);    // we’ll parse the HTML body here
    if (section=="header") { // headers..
      Serial.print(".");
      if (line=="\n") { // skips the empty space at the beginning
        section="json";
      }
   }
   else if (section=="json") {  // print the good stuff
    section="ignore";
    String result = line.substring(1);      // Parse JSON
    int size = result.length() + 1;
    char json[size];
    result.toCharArray(json, size);
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& json_parsed = jsonBuffer.parseObject(json);
    if (!json_parsed.success())
    {
      Serial.println("parseObject() failed");
      return;
    }
    // Make the decision to turn off or on the LED
    if (strcmp(json_parsed["light"], "on") == 0) {
      digitalWrite(pin, HIGH);
       Serial.println("LED ON");
    }
    else {
      digitalWrite(pin, LOW);
      Serial.println("led off");
    }
  }
}
Serial.print("closing connection. ");
}