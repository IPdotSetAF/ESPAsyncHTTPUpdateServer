#ifndef __ASYNC_HTTP_UPDATE_SERVER_H
#define __ASYNC_HTTP_UPDATE_SERVER_H

#include <ESPAsyncWebServer.h>

enum UpdateType
{
    FIRMWARE,
    FILE_SYSTEM
};

enum UpdateResult
{
    UPDATE_OK,
    UPDATE_ABORT,
    UPDATE_ERROR,
};

typedef void (*ESPAsyncHTTPUpdateServer_event)(const UpdateType type, int &result);

class ESPAsyncHTTPUpdateServer
{
public:
    ESPAsyncHTTPUpdateServer_event onUpdateBegin = NULL;
    ESPAsyncHTTPUpdateServer_event onUpdateEnd = NULL;

    ESPAsyncHTTPUpdateServer();

    void setup(AsyncWebServer *server)
    {
        setup(server, emptyString, emptyString);
    }

    void setup(AsyncWebServer *server, const String &path)
    {
        setup(server, path, emptyString, emptyString);
    }

    void setup(AsyncWebServer *server, const String &username, const String &password)
    {
        setup(server, "/update", username, password);
    }

    void setup(AsyncWebServer *server, const String &path, const String &username, const String &password);

    void updateCredentials(const String &username, const String &password)
    {
        _username = username;
        _password = password;
    }

protected:
    void _setUpdaterError();

private:
    AsyncWebServer *_server;
    String _username;
    String _password;
    bool _authenticated;
    String _updaterError;
    UpdateType _updateType;
    int _updateResult;
};

#endif
