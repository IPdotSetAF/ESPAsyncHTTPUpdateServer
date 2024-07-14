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

#ifndef ESPASYNCHTTPUPDATESERVER_SerialOutput
    #define ESPASYNCHTTPUPDATESERVER_SerialOutput Serial
#endif

#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
    #define Log(...) ESPASYNCHTTPUPDATESERVER_SerialOutput.printf(__VA_ARGS__)
#else
    #define Log(...) 
#endif

//https://gchq.github.io/CyberChef/#recipe=Gzip('Dynamic%20Huffman%20Coding','','',false)To_Decimal('Comma',false)&input=PCFkb2N0eXBlaHRtbD48aHRtbCBsYW5nPWVuPjxtZXRhIGNoYXJzZXQ9dXRmLTg%2BPG1ldGEgY29udGVudD0id2lkdGg9ZGV2aWNlLXdpZHRoLGluaXRpYWwtc2NhbGU9MSJuYW1lPXZpZXdwb3J0Pjxmb3JtIGFjdGlvbj0iP25hbWU9ZmlybXdhcmUiZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPkZpcm13YXJlOjxicj48aW5wdXQgdHlwZT1maWxlIGFjY2VwdD0uYmluLC5iaW4uZ3ogbmFtZT1maXJtd2FyZT4gPGlucHV0IHR5cGU9c3VibWl0IHZhbHVlPSJVcGRhdGUgRmlybXdhcmUiPjwvZm9ybT48Zm9ybSBhY3Rpb249Ij9uYW1lPWZpbGVzeXN0ZW0iZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPkZpbGVTeXN0ZW06PGJyPjxpbnB1dCB0eXBlPWZpbGUgYWNjZXB0PS5iaW4sLmJpbi5neiBuYW1lPWZpbGVzeXN0ZW0%2BIDxpbnB1dCB0eXBlPXN1Ym1pdCB2YWx1ZT0iVXBkYXRlIEZpbGVTeXN0ZW0iPjwvZm9ybT4&oeol=FF
static const uint8_t serverIndex[] PROGMEM =
    {31,139,8,0,47,63,148,102,0,255,149,142,77,75,196,48,16,64,255,74,204,121,187,226,77,36,19,111,94,21,86,127,192,52,157,110,7,242,69,58,109,89,127,253,182,193,138,30,4,247,50,144,73,94,222,51,119,93,114,114,201,52,72,240,214,108,83,121,140,103,160,104,77,32,65,229,6,44,35,9,76,210,55,143,251,46,69,161,40,160,23,238,100,128,142,102,118,212,212,195,129,35,11,163,111,70,135,158,224,65,71,12,4,51,211,146,83,17,107,250,84,130,66,39,156,34,232,231,122,217,115,9,11,22,210,20,107,10,132,201,11,103,44,114,191,189,110,58,92,149,171,119,72,29,188,189,158,222,237,203,23,240,100,218,98,13,199,60,137,170,96,207,158,214,207,29,101,129,99,203,241,176,141,227,249,83,253,242,88,245,147,25,167,54,176,168,25,253,68,160,63,242,106,35,181,27,180,53,181,225,143,110,79,227,101,20,10,55,148,123,58,85,228,230,246,221,245,175,250,221,242,221,127,5,2,22,24,226,232,1,0,0};
static const char successResponse[] PROGMEM =
    R"(<meta content="15;URL=/"http-equiv=refresh>Update Success! Rebooting...)";

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
            AsyncWebServerResponse* response = request->beginResponse_P(200, "text/html", serverIndex, sizeof(serverIndex));
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);  });

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
                Log("Unauthenticated Update\n");
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
                ESPASYNCHTTPUPDATESERVER_SerialOutput.setDebugOutput(true);
#endif
                _authenticated = (_username == emptyString || _password == emptyString || request->authenticate(_username.c_str(), _password.c_str()));
                if (!_authenticated)
                {
                    Log("Unauthenticated Update\n");
                    return;
                }
                Log("Update: %s\n", filename.c_str());
#ifdef ESP8266
                Update.runAsync(true);
#endif
                if (inputName == "filesystem")
                {
                    Log("updating filesystem");
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
                        Update.printError(ESPASYNCHTTPUPDATESERVER_SerialOutput);
#endif
                    }
                }
                else
                {
                    Log("updating flash");
                    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
                    if (!Update.begin(maxSketchSpace, U_FLASH)) // start with max available size
                        _setUpdaterError();
                }
            }

            if (_authenticated && len && !_updaterError.length())
            {
                Log(".");
                if (Update.write(data, len) != len)
                    _setUpdaterError();
            }

            if (_authenticated && final && !_updaterError.length())
            {
                if (Update.end(true))
                {// true to set the size to the current progress
                    Log("Update Success: \nRebooting...\n");
                }
                else
                    _setUpdaterError();
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
                ESPASYNCHTTPUPDATESERVER_SerialOutput.setDebugOutput(false);
#endif
            }
            else
                return;
        });
}

void ESPAsyncHTTPUpdateServer::_setUpdaterError()
{
#ifdef ESPASYNCHTTPUPDATESERVER_DEBUG
    Update.printError(ESPASYNCHTTPUPDATESERVER_SerialOutput);
#endif
    StreamString str;
    Update.printError(str);
    _updaterError = str.c_str();
}
