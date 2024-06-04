#ifndef __ASYNC_HTTP_UPDATE_SERVER_H
#define __ASYNC_HTTP_UPDATE_SERVER_H

#include <ESPAsyncWebServer.h>

class ESPAsyncHTTPUpdateServer
{
public:
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
};

#endif
