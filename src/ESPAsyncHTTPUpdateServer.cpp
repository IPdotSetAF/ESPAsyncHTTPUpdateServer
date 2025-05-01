#include <Arduino.h>
#include "StreamString.h"
#include "ESPAsyncHTTPUpdateServer.h"
#include <Ticker.h>

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

#ifndef ESPASYNCHTTPUPDATESERVER_MODE
    #define ESPASYNCHTTPUPDATESERVER_MODE 0
#endif

//generated code start
#if ESPASYNCHTTPUPDATESERVER_MODE == 0
    #ifdef ESPASYNCHTTPUPDATESERVER_PRETTY
        static const uint8_t serverIndex[] PROGMEM = { 31,139,8,0,100,145,225,103,2,255,157,146,205,110,212,48,16,199,95,197,236,170,82,139,214,221,108,182,133,214,249,64,92,184,22,169,229,132,16,114,236,73,50,224,216,145,237,236,71,87,251,6,28,56,113,230,217,250,36,56,217,13,106,145,64,168,135,140,20,219,243,255,205,127,102,210,23,210,8,191,109,161,246,141,202,211,62,18,197,117,149,129,206,211,6,60,39,162,230,214,129,207,58,95,210,171,241,204,104,15,218,103,147,53,74,95,103,18,86,40,128,14,63,51,212,232,145,43,234,4,87,144,45,38,68,243,6,178,21,194,186,53,214,231,169,243,91,5,249,203,93,25,52,104,201,27,84,91,214,24,109,92,203,5,36,133,177,18,44,181,92,98,231,216,34,106,55,251,58,222,53,220,86,168,169,55,45,139,246,245,98,39,140,50,150,77,1,56,8,158,12,74,14,239,129,93,245,239,11,35,183,187,130,139,175,149,53,157,150,108,26,199,113,82,241,150,45,195,109,114,144,98,81,82,3,86,181,15,136,104,85,239,75,99,155,81,245,85,177,188,188,40,147,71,10,10,53,112,75,171,190,170,96,251,52,126,29,73,168,102,71,62,137,102,211,235,139,162,128,107,18,196,78,206,146,150,75,137,186,26,120,123,212,109,231,119,7,91,44,110,55,196,25,133,146,252,166,152,13,117,53,151,102,205,34,210,219,61,132,254,225,52,138,162,229,88,230,242,114,212,98,172,68,5,212,129,2,225,141,165,69,231,189,209,179,225,238,99,63,201,204,117,69,131,254,211,147,30,28,113,71,139,101,89,62,242,127,146,252,107,22,161,227,231,98,39,209,181,138,111,89,169,96,147,244,129,74,180,161,0,52,154,5,209,174,209,201,151,206,121,44,183,244,184,28,76,132,0,54,225,10,43,77,209,67,227,142,71,251,116,126,88,130,180,31,21,17,138,59,151,137,176,124,139,252,244,225,219,247,135,31,63,63,135,239,140,220,220,189,77,231,225,48,237,167,67,248,0,203,38,111,134,125,42,209,54,107,110,97,50,166,19,208,195,30,103,77,167,60,182,220,250,121,159,70,37,15,251,26,150,182,54,50,123,127,115,123,23,48,113,254,238,152,29,228,227,60,149,184,202,211,161,127,100,80,232,251,27,112,2,90,159,157,23,168,103,125,56,175,238,201,19,242,147,148,67,203,201,138,171,14,178,201,135,54,80,129,140,148,73,158,206,7,198,80,209,95,236,40,112,91,23,154,244,92,67,10,110,135,252,231,89,26,233,255,99,106,36,253,97,235,23,4,247,158,198,74,4,0,0 };
    #else
        static const uint8_t serverIndex[] PROGMEM = { 31,139,8,0,100,145,225,103,2,255,149,142,203,106,196,48,12,69,127,197,245,122,50,165,187,82,44,119,215,237,20,166,253,0,197,81,38,2,191,112,148,132,233,215,55,49,77,97,22,133,206,70,160,199,213,57,230,161,75,78,174,153,6,9,222,154,173,42,143,241,2,20,173,9,36,168,220,128,101,36,129,73,250,230,121,159,165,40,20,5,244,194,157,12,208,209,204,142,154,218,28,56,178,48,250,102,116,232,9,158,180,138,24,8,102,166,37,167,34,214,244,169,4,133,78,56,69,208,175,117,217,115,9,11,22,210,138,98,149,129,48,121,225,140,69,30,183,243,166,195,21,186,146,135,212,193,251,233,252,97,223,126,18,47,166,45,214,112,204,147,168,26,236,217,211,250,221,81,22,56,182,28,15,91,57,94,190,212,13,232,38,50,78,109,96,81,51,250,137,64,127,230,21,70,106,7,104,107,170,194,31,222,158,198,235,40,20,238,49,247,116,174,153,187,221,119,216,127,236,119,200,175,255,55,187,114,189,153,233,1,0,0 };
    #endif
#elif ESPASYNCHTTPUPDATESERVER_MODE == 1
    #ifdef ESPASYNCHTTPUPDATESERVER_PRETTY
        static const uint8_t serverIndex[] PROGMEM = { 31,139,8,0,100,145,225,103,2,255,125,146,75,110,219,48,16,134,175,194,218,8,144,20,102,44,203,73,155,80,143,162,155,110,83,160,233,170,40,10,138,28,73,211,82,36,65,82,126,196,240,13,186,232,170,235,158,45,39,41,37,219,69,188,233,66,3,136,228,252,223,60,254,252,149,52,34,108,45,180,161,83,101,62,68,162,184,110,10,208,101,222,65,224,68,180,220,121,8,69,31,106,122,119,58,51,58,128,14,197,100,141,50,180,133,132,21,10,160,227,207,12,53,6,228,138,122,193,21,20,139,9,209,188,131,98,133,176,182,198,133,50,247,97,171,160,124,189,171,163,6,173,121,135,106,203,58,163,141,183,92,64,86,25,39,193,81,199,37,246,158,45,18,187,217,183,233,174,227,174,65,77,131,177,44,217,183,139,157,48,202,56,54,5,224,32,120,54,42,121,124,2,118,55,188,175,140,220,238,42,46,126,52,206,244,90,178,105,154,166,89,195,45,91,198,219,236,32,197,146,172,5,108,218,16,17,201,170,221,215,198,117,39,213,55,213,242,246,166,206,94,40,40,212,192,29,109,134,170,98,219,151,233,219,68,66,51,59,242,73,50,155,222,223,84,21,220,147,40,118,113,149,89,46,37,234,102,228,237,81,219,62,236,14,109,177,212,110,136,55,10,37,249,71,49,27,234,91,46,205,154,37,100,104,247,16,134,135,211,36,73,150,167,50,151,183,39,45,198,106,84,64,61,40,16,193,56,90,245,33,24,61,27,239,190,12,155,44,124,95,117,24,190,158,205,224,136,59,182,88,215,245,139,254,47,178,255,237,34,78,252,90,236,36,122,171,248,150,213,10,54,217,16,168,68,23,11,64,163,89,20,237,59,157,125,239,125,192,122,75,143,230,96,34,6,112,25,87,216,104,138,1,58,127,60,218,231,243,131,9,242,97,85,68,40,238,125,33,162,249,22,229,229,243,207,95,207,191,255,124,139,223,21,121,120,124,159,207,227,97,62,108,135,240,17,86,76,222,141,126,170,209,117,107,238,96,114,74,39,160,71,31,23,93,175,2,90,238,194,124,72,163,146,71,191,70,211,182,70,22,31,31,62,61,70,76,90,126,56,102,71,249,180,204,37,174,202,124,156,31,25,21,134,249,70,156,0,27,138,235,10,245,108,8,215,205,19,57,35,159,165,28,70,78,86,92,245,80,76,62,219,72,5,114,162,76,202,124,62,50,198,138,202,191,77,16,113,93,115,3,0,0 };
    #else
        static const uint8_t serverIndex[] PROGMEM = { 31,139,8,0,100,145,225,103,2,255,85,142,205,78,196,48,12,132,95,37,228,188,93,196,109,133,226,112,227,10,18,203,3,184,137,187,181,148,63,165,78,43,120,122,218,136,61,236,101,36,123,60,254,198,60,249,236,228,167,208,44,49,88,115,168,10,152,110,64,201,154,72,130,202,205,88,23,18,104,50,13,151,251,46,39,161,36,160,55,246,50,131,167,149,29,13,125,56,113,98,97,12,195,226,48,16,188,104,149,48,18,172,76,91,201,85,172,153,114,141,10,157,112,78,160,223,186,57,113,141,27,86,210,138,82,47,3,177,5,225,130,85,158,143,243,193,227,14,221,201,115,246,240,249,241,117,181,239,255,137,87,51,86,107,56,149,38,170,7,39,14,180,127,119,84,4,206,35,167,211,33,231,219,175,122,0,61,68,150,54,70,22,181,98,104,4,250,187,236,48,82,119,128,182,166,87,176,127,120,226,68,109,41,1,0,0 };
    #endif
#elif ESPASYNCHTTPUPDATESERVER_MODE == 2
    #ifdef ESPASYNCHTTPUPDATESERVER_PRETTY
        static const uint8_t serverIndex[] PROGMEM = { 31,139,8,0,100,145,225,103,2,255,125,146,203,110,19,49,20,134,95,197,36,170,212,162,56,153,76,90,104,61,23,196,134,109,43,181,172,16,66,30,251,204,140,193,99,91,182,39,201,52,202,27,176,96,197,154,103,235,147,96,79,18,104,89,176,152,35,141,237,243,127,231,242,231,175,184,102,126,48,208,250,78,150,121,140,72,82,213,20,160,202,188,3,79,17,107,169,117,224,139,222,215,248,250,116,166,149,7,229,139,201,70,112,223,22,28,214,130,1,30,127,102,66,9,47,168,196,142,81,9,197,114,130,20,237,160,88,11,216,24,109,125,153,59,63,72,40,95,239,234,160,129,107,218,9,57,144,78,43,237,12,101,144,85,218,114,176,216,82,46,122,71,150,137,217,238,219,116,215,81,219,8,133,189,54,36,217,183,203,29,211,82,91,50,5,160,192,104,54,42,57,241,8,228,58,190,175,52,31,118,21,101,223,26,171,123,197,201,52,77,211,172,161,134,172,194,109,118,144,34,73,214,130,104,90,31,16,201,186,221,215,218,118,39,213,55,213,234,234,178,206,158,41,72,161,128,90,220,196,170,66,219,231,233,219,132,67,51,59,242,81,50,155,222,92,86,21,220,160,32,118,118,145,25,202,185,80,205,200,219,11,101,122,191,59,180,69,82,179,69,78,75,193,209,31,138,222,98,215,82,174,55,36,65,177,221,67,136,15,167,73,146,172,78,101,174,174,78,90,132,212,66,2,118,32,129,121,109,113,213,123,175,213,108,188,251,20,55,89,184,190,234,132,255,252,98,6,71,220,177,197,186,174,159,245,127,150,253,111,23,97,226,115,182,227,194,25,73,7,82,75,216,102,49,96,46,108,40,64,104,69,130,104,223,169,236,107,239,188,168,7,124,52,7,97,33,128,205,168,20,141,194,194,67,231,142,71,251,124,113,48,65,30,87,133,152,164,206,21,44,152,111,89,158,63,125,255,241,244,243,215,151,240,93,160,219,135,247,249,34,28,230,113,59,136,142,176,98,242,110,244,83,156,129,27,92,80,157,156,4,16,168,209,201,69,215,75,47,12,181,126,17,19,49,167,193,177,193,182,173,230,197,221,237,253,67,0,165,229,135,144,127,63,230,7,68,90,230,92,172,203,124,156,33,26,53,162,126,64,50,48,190,152,87,66,205,98,152,55,143,232,31,250,139,164,195,224,209,154,202,30,138,201,71,19,200,128,254,146,38,101,190,24,57,99,93,229,111,61,86,236,22,123,3,0,0 };
    #else
        static const uint8_t serverIndex[] PROGMEM = { 31,139,8,0,100,145,225,103,2,255,93,142,203,106,3,49,12,69,127,197,245,58,147,210,93,41,150,187,235,54,133,52,31,224,177,53,25,129,95,216,242,132,228,235,235,49,148,210,110,46,72,226,232,92,245,228,146,229,123,198,149,131,215,106,79,225,77,188,2,70,173,2,178,17,118,53,165,34,67,227,101,122,253,217,165,200,24,25,228,141,28,175,224,112,35,139,211,24,14,20,137,201,248,169,90,227,17,94,164,136,38,32,108,132,183,156,10,107,181,164,18,132,177,76,41,130,124,31,199,133,60,214,123,101,12,82,96,28,117,32,52,207,148,77,225,231,29,152,156,233,218,238,94,147,131,207,211,249,75,127,116,230,60,152,55,53,23,173,40,230,198,98,160,251,187,110,176,152,25,142,51,197,195,30,199,235,67,252,147,253,129,106,155,3,177,216,140,111,8,242,146,187,16,197,175,68,106,53,138,232,111,211,36,186,13,49,1,0,0 };
    #endif
#endif
//generated code end

static const char successResponse[] PROGMEM =
    R"(<meta content="15;URL=/"http-equiv=refresh>Update Success! Rebooting...)";

Ticker restartTimer;

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
#ifdef ESP32
            AsyncWebServerResponse* response = request->beginResponse(200, "text/html", serverIndex, sizeof(serverIndex));
#else
            AsyncWebServerResponse* response = request->beginResponse_P(200, "text/html", serverIndex, sizeof(serverIndex));
#endif
            response->addHeader("Content-Encoding", "gzip");
            request->send(response); });

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

        if (_updateResult != UpdateResult::UPDATE_OK)
        {
            AsyncWebServerResponse *response = request->beginResponse(200, F("text/html"), Update.hasError() ? String(F("Update error: ")) + _updaterError : "Update aborted by server.");
            response->addHeader("Access-Control-Allow-Headers", "*");
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->addHeader("Connection", "close");
            request->send(response);
        }
        else
        {
#ifdef ESP32
            request->send(200, PSTR("text/html"), successResponse);
#else
            request->send_P(200, PSTR("text/html"), successResponse);
#endif
            Log("Rebooting...\n");
            restartTimer.once_ms(1000,[]{ ESP.restart(); });
        } },
        [&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
        {
            // handler for the file upload, gets the sketch bytes, and writes
            // them through the Update object

            _updateType = request->getParam("name")->value() == "filesystem"?
                    UpdateType::FILE_SYSTEM :
                    UpdateType::FIRMWARE;

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

                if (onUpdateBegin)
                {
                    _updateResult = UpdateResult::UPDATE_OK;
                    onUpdateBegin(_updateType, _updateResult);
                    if (_updateResult != UpdateResult::UPDATE_OK)
                    {
                        Log("Update aborted by server: %d\n", _updateResult);
                        if(onUpdateEnd)
                            onUpdateEnd(_updateType, _updateResult);
                        return;
                    }
                }

                Log("Update: %s\n", filename.c_str());
#ifdef ESP8266
                Update.runAsync(true);
#endif
                if (_updateType == UpdateType::FILE_SYSTEM)
                {
                    Log("updating filesystem\n");
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
                    Log("updating flash\n");
                    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
                    if (!Update.begin(maxSketchSpace, U_FLASH)) // start with max available size
                        _setUpdaterError();
                }
            }

            if (_authenticated && len && _updateResult == UpdateResult::UPDATE_OK)
            {
                Log(".");
                if (Update.write(data, len) != len)
                    _setUpdaterError();
            }

            if (_authenticated && final && _updateResult == UpdateResult::UPDATE_OK)
            {
                if (Update.end(true))
                { // true to set the size to the current progress
                    Log("Update Success.\n");
                    _updateResult = UpdateResult::UPDATE_OK;
                    if(onUpdateEnd)
                        onUpdateEnd(_updateType, _updateResult);
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
    
    _updateResult = UpdateResult::UPDATE_ERROR;
    if(onUpdateEnd)
        onUpdateEnd(_updateType, _updateResult);
}