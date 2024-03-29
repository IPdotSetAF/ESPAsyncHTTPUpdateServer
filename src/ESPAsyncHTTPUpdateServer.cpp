#include <Arduino.h>
#include <flash_hal.h>
#include <FS.h>
#include "StreamString.h"
#include "ESPAsyncHTTPUpdateServer.h"

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
#ifdef DEBUG
                Serial.printf("Unauthenticated Update\n");
#endif
                return;
            } });

    // handler for the /update form POST (once file upload finishes)
    _server->on(
        path.c_str(), HTTP_POST, [&](AsyncWebServerRequest *request)
        {
            //if requestAuthentication() is false second handler will run, else it wont.
            if(!_authenticated)
                return request->requestAuthentication();

            if (Update.hasError()) {
                AsyncWebServerResponse* response = request->beginResponse(200,F("text/html"), String(F("Update error: ")) + _updaterError);
                response->addHeader("Access-Control-Allow-Headers", "*");
                response->addHeader("Access-Control-Allow-Origin", "*");
                response->addHeader("Connection", "close");
                request->send(response); 
            } 
            else {
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
#ifdef DEBUG
                Serial.setDebugOutput(true);
#endif
                _authenticated = (_username == emptyString || _password == emptyString || request->authenticate(_username.c_str(), _password.c_str()));
                if (!_authenticated)
                {
#ifdef DEBUG
                    Serial.printf("Unauthenticated Update\n");
#endif
                    return;
                }
#ifdef DEBUG
                Serial.printf("Update: %s\n", filename.c_str());
#endif
                Update.runAsync(true);
                if (inputName == "filesystem")
                {
#ifdef DEBUG
                    Serial.println("updating filesystem");
#endif
                    size_t fsSize = ((size_t)FS_end - (size_t)FS_start);
                    close_all_fs();
                    if (!Update.begin(fsSize, U_FS))
                    { // start with max available size
#ifdef DEBUG
                        Update.printError(Serial);
#endif
                    }
                }
                else
                {
#ifdef DEBUG
                    Serial.println("updating flash");
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
#ifdef DEBUG
                Serial.printf(".");
#endif
                if (Update.write(data, len) != len)
                    _setUpdaterError();
            }

            if (_authenticated && final && !_updaterError.length())
            {
                if (Update.end(true))
                { // true to set the size to the current progress
#ifdef DEBUG
                    Serial.println("Update Success: \nRebooting...\n");
#endif
                }
                else
                    _setUpdaterError();
#ifdef DEBUG
                Serial.setDebugOutput(false);
#endif
            }
            else
                return;
        });
}

void ESPAsyncHTTPUpdateServer::_setUpdaterError()
{
#ifdef DEBUG
    Update.printError(Serial);
#endif
    StreamString str;
    Update.printError(str);
    _updaterError = str.c_str();
}
