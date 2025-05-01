//
// A simple server implementation showing how to:
//  * serve static messages
//  * handle missing pages / 404s
//  * setup an http update server
//

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <ESPAsyncHTTPUpdateServer.h>

//create an object from the UpdateServer
ESPAsyncHTTPUpdateServer updateServer;
AsyncWebServer server(80);

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello, world");
    });

    server.onNotFound(notFound);

    //setup the updateServer with credentials
    updateServer.setup(&server, "admin", "admin");
    //hook to update events if you need to
    updateServer.onUpdateBegin = [](const UpdateType type, int &result)
    {
        //you can force abort the update like this if you need to:
        //result = UpdateResult::UPDATE_ABORT;        
        Serial.println("Update started : " + String(type));
    };
    updateServer.onUpdateEnd = [](const UpdateType type, int &result)
    {
        Serial.println("Update finished : " + String(type) + " result: " + String(result));
    };

    server.begin();
}

void loop() {
}