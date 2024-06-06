#include <Arduino.h>
#include "StreamString.h"
#include "ESPAsyncHTTPUpdateServer.h"

#ifdef ESP32
    #ifdef ESPASYNCHTTPUPDATESERVER_LITTLEFS
        #include <LittleFS.h>
    #else
        #include <SPIFFS.h>
    #endif
    #include <Update.h>
#elif defined(ESP8266)
    #include <flash_hal.h>
    #include <FS.h>
#else
  #error "This library only supports boards with an ESP8266 or ESP32 processor."
#endif

#ifdef ESPASYNCHTTPUPDATESERVER_SERIAL0
    #define SerialOutput Serial0
#elif defined(ESPASYNCHTTPUPDATESERVER_SERIAL1)
    #define SerialOutput Serial1
#elif defined(ESPASYNCHTTPUPDATESERVER_SERIAL2)
    #define SerialOutput Serial2
#else
    #define SerialOutput Serial
#endif


static const char serverIndex[] PROGMEM =
    R"(<!DOCTYPE html>
     <html lang='en'>
     <head>
         <meta charset='utf-8'>
         <meta name='viewport' content='width=device-width,initial-scale=1'/>
     </head>
     <body>
     <form method='POST' action='?name=firmware' enctype='multipart/form-data'>
         Firmware:<br>
         <input type='file' accept='.bin,.bin.gz' name='firmware'>
         <input type='submit' value='Update Firmware'>
     </form>
     <form method='POST' action='?name=filesystem' enctype='multipart/form-data'>
         FileSystem:<br>
         <input type='file' accept='.bin,.bin.gz' name='filesystem'>
         <input type='submit' value='Update FileSystem'>
     </form>
     </body>
     </html>)";
static const char successResponse[] PROGMEM =
    "<META http-equiv=\"refresh\" content=\"15;URL=/\">Update Success! Rebooting...";

ESPAsyncHTTPUpdateServer::ESPAsyncHTTPUpdateServer()
{
    _server = NULL;
    _username = emptyString;
    _password = emptyString;
    _authenticated = false;
}

void ESPAsyncHTTPUpdateServer::setup(AsyncWebServer *server, const String &path, const String &username, const String &password)
{
    _server = server;
    _username = username;
    _password = password;

    // handler for the /update form page
    _server->on(path.c_str(), HTTP_GET, [&](AsyncWebServerRequest *request)
                {
            if(_username != emptyString && _password != emptyString)
                if( !request->authenticate(_username.c_str(), _password.c_str()))
                    return request->requestAuthentication();

            request->send_P(200, "text/html", serverIndex); });

    // handler for the /update form page - preflight options
    _server->on(path.c_str(), HTTP_OPTIONS, [&](AsyncWebServerRequest *request)
                {
            AsyncWebServerResponse* response = request->beginResponse(200,F("text/html"), String(F("y")));
            response->addHeader("Access-Control-Allow-Headers", "*");
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response); 
            
            _authenticated = (_username == emptyString || _password == emptyString || request -> authenticate(_username.c_str(), _password.c_str()));
            if (!_authenticated)
            {
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                SerialOutput.printf("Unauthenticated Update\n");
#endif
                return;
            } });

    // handler for the /update form POST (once file upload finishes)
    _server->on(
        path.c_str(), HTTP_POST, [&](AsyncWebServerRequest *request)
        {
        // if requestAuthentication() is false second handler will run, else it wont.
        if (!_authenticated)
            return request->requestAuthentication();

        if (Update.hasError())
        {
            AsyncWebServerResponse *response = request->beginResponse(200, F("text/html"), String(F("Update error: ")) + _updaterError);
            response->addHeader("Access-Control-Allow-Headers", "*");
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->addHeader("Connection", "close");
            request->send(response);
        }
        else
        {
            request->send_P(200, PSTR("text/html"), successResponse);
            delay(1000);
            ESP.restart();
        } },
        [&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
        {
            // handler for the file upload, gets the sketch bytes, and writes
            // them through the Update object

            String inputName = request->getParam("name")->value();

            if (!index)
            {
                _updaterError.clear();
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                SerialOutput.setDebugOutput(true);
#endif
                _authenticated = (_username == emptyString || _password == emptyString || request->authenticate(_username.c_str(), _password.c_str()));
                if (!_authenticated)
                {
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                    SerialOutput.printf("Unauthenticated Update\n");
#endif
                    return;
                }
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                SerialOutput.printf("Update: %s\n", filename.c_str());
#endif
#ifdef ESP8266
                Update.runAsync(true);
#endif
                if (inputName == "filesystem")
                {
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                    SerialOutput.println("updating filesystem");
#endif

#ifdef ESP8266
                    int command = U_FS;
                    size_t fsSize = ((size_t)FS_end - (size_t)FS_start);
                    close_all_fs();
#elif defined(ESP32)
                    int command = U_SPIFFS;
    #ifdef ESPASYNCHTTPUPDATESERVER_LITTLEFS
                    size_t fsSize = LittleFS.totalBytes();
    #else
                    size_t fsSize = SPIFFS.totalBytes();
    #endif
#endif
                    if (!Update.begin(fsSize, command))
                    { // start with max available size
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                        Update.printError(Serial);
#endif
                    }
                }
                else
                {
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                    SerialOutput.println("updating flash");
#endif
                    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
                    if (!Update.begin(maxSketchSpace, U_FLASH))
                    { // start with max available size
                        _setUpdaterError();
                    }
                }
            }

            if (_authenticated && len && !_updaterError.length())
            {
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                SerialOutput.printf(".");
#endif
                if (Update.write(data, len) != len)
                    _setUpdaterError();
            }

            if (_authenticated && final && !_updaterError.length())
            {
                if (Update.end(true))
                { // true to set the size to the current progress
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                    SerialOutput.println("Update Success: \nRebooting...\n");
#endif
                }
                else
                    _setUpdaterError();
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                SerialOutput.setDebugOutput(false);
#endif
            }
            else
                return;
        });
}

void ESPAsyncHTTPUpdateServer::_setUpdaterError()
{
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
    Update.printError(Serial);
#endif
    StreamString str;
    Update.printError(str);
    _updaterError = str.c_str();
}
