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

//https://kangax.github.io/html-minifier/
#ifdef ESPASYNCHTTPUPDATESERVER_PRETTY
//https://gchq.github.io/CyberChef/#recipe=Gzip('Dynamic%20Huffman%20Coding','','',false)To_Decimal('Comma',false)&input=PCFkb2N0eXBlaHRtbD48aHRtbCBsYW5nPWVuPjxtZXRhIGNoYXJzZXQ9dXRmLTg%2BPG1ldGEgY29udGVudD0id2lkdGg9ZGV2aWNlLXdpZHRoLGluaXRpYWwtc2NhbGU9MSJuYW1lPXZpZXdwb3J0PjxzdHlsZT4qe2ZvbnQtZmFtaWx5Om1vbm9zcGFjZTtib3JkZXItcmFkaXVzOjEwcHh9aDJ7bWFyZ2luLXRvcDowfWgxe2NvbG9yOiNlZWFlY2E7Zm9udC1zaXplOjgwcHh9Ym9keXtiYWNrZ3JvdW5kOiMyMjI7Z2FwOjMwcHg7bWFyZ2luOjA7aGVpZ2h0OjEwMHZofWZvcm17Y29sb3I6IzZiMzU0ZjtiYWNrZ3JvdW5kOmxpbmVhci1ncmFkaWVudCgyNzBkZWcsI2VlYWVjYSAwLCM5NGJiZTkgMTAwJSk7cGFkZGluZzozMHB4fWlucHV0e2JvcmRlcjoycHggc29saWQgIzZiMzU0Zjtib3gtc2hhZG93OjAgMTBweCAxMHB4IDJweCAjMDAwMztoZWlnaHQ6MzVweH1pbnB1dDo6ZmlsZS1zZWxlY3Rvci1idXR0b24saW5wdXRbdHlwZT1zdWJtaXRde2JhY2tncm91bmQ6IzZiMzU0Zjtjb2xvcjojZmZmO2hlaWdodDoxMDAlO2ZvbnQtZmFtaWx5Om1vbm9zcGFjZTtib3JkZXI6bm9uZX0uY3tkaXNwbGF5OmZsZXg7ZmxleC1kaXJlY3Rpb246Y29sdW1uO2p1c3RpZnktY29udGVudDpjZW50ZXI7YWxpZ24taXRlbXM6Y2VudGVyfTwvc3R5bGU%2BPGJvZHkgY2xhc3M9Yz48aDE%2BKOKMkOKWoF/ilqApIE9UQTwvaDE%2BPGZvcm0gYWN0aW9uPSI/bmFtZT1maXJtd2FyZSJjbGFzcz1jIGVuY3R5cGU9bXVsdGlwYXJ0L2Zvcm0tZGF0YSBtZXRob2Q9UE9TVD48aDI%2BRmlybXdhcmU8L2gyPjxkaXY%2BPGlucHV0IHR5cGU9ZmlsZSBhY2NlcHQ9LmJpbiwuYmluLmd6IG5hbWU9ZmlybXdhcmU%2BIDxpbnB1dCB0eXBlPXN1Ym1pdCB2YWx1ZT0iVXBkYXRlIEZpcm13YXJlIj48L2Rpdj48L2Zvcm0%2BPGZvcm0gYWN0aW9uPSI/bmFtZT1maWxlc3lzdGVtImNsYXNzPWMgZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPjxoMj5GaWxlU3lzdGVtPC9oMj48ZGl2PjxpbnB1dCB0eXBlPWZpbGUgYWNjZXB0PS5iaW4sLmJpbi5neiBuYW1lPWZpbGVzeXN0ZW0%2BIDxpbnB1dCB0eXBlPXN1Ym1pdCB2YWx1ZT0iVXBkYXRlIEZpbGVTeXN0ZW0iPjwvZGl2PjwvZm9ybT4&oeol=FF
static const uint8_t serverIndex[] PROGMEM = 
    {31,139,8,0,54,195,148,102,0,255,157,146,77,110,219,60,16,134,175,194,218,8,144,20,102,44,203,73,155,80,63,31,186,233,54,5,146,174,138,162,160,200,145,52,45,69,10,36,229,159,24,190,65,23,93,117,253,157,45,39,9,37,203,69,82,32,64,145,133,6,16,201,121,159,121,103,38,125,35,141,240,219,22,106,223,168,60,237,35,81,92,87,25,232,60,109,192,115,34,106,110,29,248,172,243,37,189,58,158,25,237,65,251,108,178,70,233,235,76,194,10,5,208,225,103,134,26,61,114,69,157,224,10,178,197,68,243,6,178,21,194,186,53,214,231,169,243,91,5,249,219,93,25,36,104,201,27,84,91,214,24,109,92,203,5,36,133,177,18,44,181,92,98,231,216,34,106,55,251,58,222,53,220,86,168,169,55,45,139,246,245,98,39,140,50,150,77,1,56,8,158,12,74,14,239,129,93,245,239,11,35,183,187,130,139,31,149,53,157,150,108,26,199,113,82,241,150,45,195,109,114,144,98,81,82,3,86,181,15,136,104,85,239,75,99,155,163,234,187,98,121,121,81,38,79,20,20,106,224,150,86,125,85,193,245,105,252,62,146,80,205,70,62,137,102,211,235,139,162,128,107,18,196,78,206,146,150,75,137,186,26,120,123,212,109,231,119,7,91,44,110,55,196,25,133,146,252,161,152,13,117,53,151,102,205,162,144,30,238,135,208,63,156,70,81,180,60,150,185,188,60,106,49,86,162,2,234,64,129,240,198,210,162,243,222,232,208,245,112,247,165,31,100,230,186,162,65,255,245,89,15,70,220,104,177,44,203,39,254,79,14,29,124,97,22,76,27,13,251,115,177,147,232,90,197,183,172,84,176,73,250,64,37,218,80,3,26,205,130,110,215,232,228,123,231,60,150,91,58,174,7,19,33,128,77,184,194,74,83,244,208,184,241,104,159,206,15,123,144,246,211,34,66,113,231,50,17,214,111,145,159,62,252,252,245,240,251,255,111,225,59,35,55,119,31,210,121,56,76,251,1,17,62,192,178,201,127,195,74,149,104,155,53,183,48,25,179,9,232,97,145,179,166,83,30,91,110,253,188,207,162,146,135,133,13,91,91,27,153,125,186,185,189,11,148,56,255,56,38,7,245,56,79,37,174,242,116,232,32,25,20,250,14,7,154,128,214,103,231,5,234,89,31,206,171,123,242,12,156,147,167,57,135,174,147,21,87,29,100,147,207,109,192,2,57,98,38,121,58,31,32,67,73,47,216,81,224,182,46,52,233,149,134,20,220,14,233,175,179,116,132,255,147,169,35,234,47,91,143,204,254,125,32,76,4,0,0};
#else
//https://gchq.github.io/CyberChef/#recipe=Gzip('Dynamic%20Huffman%20Coding','','',false)To_Decimal('Comma',false)&input=PCFkb2N0eXBlaHRtbD48aHRtbCBsYW5nPWVuPjxtZXRhIGNoYXJzZXQ9dXRmLTg%2BPG1ldGEgY29udGVudD0id2lkdGg9ZGV2aWNlLXdpZHRoLGluaXRpYWwtc2NhbGU9MSJuYW1lPXZpZXdwb3J0Pjxmb3JtIGFjdGlvbj0iP25hbWU9ZmlybXdhcmUiZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPkZpcm13YXJlOjxicj48aW5wdXQgdHlwZT1maWxlIGFjY2VwdD0uYmluLC5iaW4uZ3ogbmFtZT1maXJtd2FyZT4gPGlucHV0IHR5cGU9c3VibWl0IHZhbHVlPSJVcGRhdGUgRmlybXdhcmUiPjwvZm9ybT48Zm9ybSBhY3Rpb249Ij9uYW1lPWZpbGVzeXN0ZW0iZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPkZpbGVTeXN0ZW06PGJyPjxpbnB1dCB0eXBlPWZpbGUgYWNjZXB0PS5iaW4sLmJpbi5neiBuYW1lPWZpbGVzeXN0ZW0%2BIDxpbnB1dCB0eXBlPXN1Ym1pdCB2YWx1ZT0iVXBkYXRlIEZpbGVTeXN0ZW0iPjwvZm9ybT4&oeol=FF
static const uint8_t serverIndex[] PROGMEM =
    {31,139,8,0,47,63,148,102,0,255,149,142,77,75,196,48,16,64,255,74,204,121,187,226,77,36,19,111,94,21,86,127,192,52,157,110,7,242,69,58,109,89,127,253,182,193,138,30,4,247,50,144,73,94,222,51,119,93,114,114,201,52,72,240,214,108,83,121,140,103,160,104,77,32,65,229,6,44,35,9,76,210,55,143,251,46,69,161,40,160,23,238,100,128,142,102,118,212,212,195,129,35,11,163,111,70,135,158,224,65,71,12,4,51,211,146,83,17,107,250,84,130,66,39,156,34,232,231,122,217,115,9,11,22,210,20,107,10,132,201,11,103,44,114,191,189,110,58,92,149,171,119,72,29,188,189,158,222,237,203,23,240,100,218,98,13,199,60,137,170,96,207,158,214,207,29,101,129,99,203,241,176,141,227,249,83,253,242,88,245,147,25,167,54,176,168,25,253,68,160,63,242,106,35,181,27,180,53,181,225,143,110,79,227,101,20,10,55,148,123,58,85,228,230,246,221,245,175,250,221,242,221,127,5,2,22,24,226,232,1,0,0};
#endif

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
