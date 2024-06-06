# ESP Async HTTP Update Server

This is a copy of [ESP8266HTTPUpdateServer](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPUpdateServer)/[ESP32's HTTPUpdateServer](https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPUpdateServer) library, modified to be compatible with [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer).

It will provide a simple webpage for updating the firmware or filesystem of `ESP8266` or `ESP32` microcontrollers.

## Features
- Supports:
    - ESP8266
    - ESP32
- Can Update:
    - Firmware
    - FileSystem
- Update route customization (default: `/update`)
- Update credentials customization (default: `No credentials`)
    - Username
    - Password
- FileSystem Options:
    - SPIFFS
    - LittleFS

## HowTo

### Install

This Library is available in `Arduino Library Repository` and `PIO` and you can install it from: 
- Arduino IDE Library Manager
- PlatformIO Libraries

### Setup
1. Include the library after `ESPAsyncWebServer.h`
``` C++
#include <ESPAsyncWebServer.h>
#include <ESPAsyncHTTPUpdateServer.h>
```
2. Create an object from `ESPAsyncHTTPUpdateServer`
``` C++
ESPAsyncHTTPUpdateServer _updateServer;
AsyncWebServer _server(80);
```
3. Setup the update server before starting the webServer
``` C++
_updateServer.setup(&_server);
_server.begin();
``` 
#### Custom Route
``` C++
_updateServer.setup(&_server, "/customroute");
```
#### Credentials
``` C++
_updateServer.setup(&_server, "username", "password");
```
or
``` C++
_updateServer.setup(&_server, "/customroute", "username", "password");
```

> [!IMPORTANT]
> ### Selecting FileSystem
> The library's default fileSystem is `SPIFFS` but if you are using `LittleFS` for your FileSystem, make sure to add the `-DESPASYNCHTTPUPDATESERVER_LITTLEFS` Build Flag to your environment.

> [!TIP]
> ### Debugging
> In order to debug the library funtionality, you can add the `-DESPASYNCHTTPUPDATESERVER_DEBUG` Build Flag to your environment.
>
>This will enable the library to print logs to the Serial.
>### Selecting another serial port
>In case you use another serial port, such as Serial0,Serial1 or Serial2, if your controller support those, you can add `-DESPASYNCHTTPUPDATESERVER_SERIAL0` Build Flag to your environment, adjust the final digit to the serial port you want to use  

## TODO:
- ESP8266 LittleFS support

## Contribution
- You can open Issues for any bug report or feature request.
- You are free to contribute to this project by following these steps:
   1. Fork this Repo.
   2. Create a new branch for your feature/bugfix in your forked Repo.
   3. Commit your changes to the new branch you just made.
   4. Create a pull request from your branch into the `master` branch of This Repo([https://github.com/IPdotSetAF/ESPAsyncHTTPUpdateServer](https://github.com/IPdotSetAF/ESPAsyncHTTPUpdateServer)).
