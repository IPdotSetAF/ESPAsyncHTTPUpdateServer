# ESP Async HTTP Update Server

This is a copy of [ESP8266HTTPUpdateServer](https://github.com/esp8266/Arduino/tree/de1029ffe0f85b40465ecb64218ef7ab3643ffa7/libraries/ESP8266HTTPUpdateServer) library, modified to be compatible with [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer).

It will provide a simple webpage for updating the firmware or filesystem of `ESP8266` or `ESP32` microcontrollers.

- The website is available on `/update` by default unless you change it.
- You can also set credentials (username and password) for the update page.
