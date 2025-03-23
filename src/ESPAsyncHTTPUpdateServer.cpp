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

#ifndef ESPASYNCHTTPUPDATESERVER_MODE
    #define ESPASYNCHTTPUPDATESERVER_MODE 0
#endif

//https://kangax.github.io/html-minifier/
#if ESPASYNCHTTPUPDATESERVER_MODE == 0
    #ifdef ESPASYNCHTTPUPDATESERVER_PRETTY
    //https://gchq.github.io/CyberChef/#recipe=Gzip('Dynamic%20Huffman%20Coding','','',false)To_Decimal('Comma',false)&input=PCFkb2N0eXBlaHRtbD48aHRtbCBsYW5nPWVuPjxtZXRhIGNoYXJzZXQ9dXRmLTg%2BPG1ldGEgY29udGVudD0id2lkdGg9ZGV2aWNlLXdpZHRoLGluaXRpYWwtc2NhbGU9MSJuYW1lPXZpZXdwb3J0PjxzdHlsZT4qe2ZvbnQtZmFtaWx5Om1vbm9zcGFjZTtib3JkZXItcmFkaXVzOjEwcHh9aDJ7bWFyZ2luLXRvcDowfWgxe2NvbG9yOiNlZWFlY2E7Zm9udC1zaXplOjgwcHh9Ym9keXtiYWNrZ3JvdW5kOiMyMjI7Z2FwOjMwcHg7bWFyZ2luOjA7aGVpZ2h0OjEwMHZofWZvcm17Y29sb3I6IzZiMzU0ZjtiYWNrZ3JvdW5kOmxpbmVhci1ncmFkaWVudCgyNzBkZWcsI2VlYWVjYSAwLCM5NGJiZTkgMTAwJSk7cGFkZGluZzozMHB4fWlucHV0e2JvcmRlcjoycHggc29saWQgIzZiMzU0Zjtib3gtc2hhZG93OjAgMTBweCAxMHB4IDJweCAjMDAwMztoZWlnaHQ6MzVweH1pbnB1dDo6ZmlsZS1zZWxlY3Rvci1idXR0b24saW5wdXRbdHlwZT1zdWJtaXRde2JhY2tncm91bmQ6IzZiMzU0Zjtjb2xvcjojZmZmO2hlaWdodDoxMDAlO2ZvbnQtZmFtaWx5Om1vbm9zcGFjZTtib3JkZXI6bm9uZX0uY3tkaXNwbGF5OmZsZXg7ZmxleC1kaXJlY3Rpb246Y29sdW1uO2p1c3RpZnktY29udGVudDpjZW50ZXI7YWxpZ24taXRlbXM6Y2VudGVyfTwvc3R5bGU%2BPGJvZHkgY2xhc3M9Yz48aDE%2BKOKMkOKWoF/ilqApIE9UQTwvaDE%2BPGZvcm0gYWN0aW9uPSI/bmFtZT1maXJtd2FyZSJjbGFzcz1jIGVuY3R5cGU9bXVsdGlwYXJ0L2Zvcm0tZGF0YSBtZXRob2Q9UE9TVD48aDI%2BRmlybXdhcmU8L2gyPjxkaXY%2BPGlucHV0IHR5cGU9ZmlsZSBhY2NlcHQ9LmJpbiwuYmluLmd6IG5hbWU9ZmlybXdhcmU%2BIDxpbnB1dCB0eXBlPXN1Ym1pdCB2YWx1ZT0iVXBkYXRlIEZpcm13YXJlIj48L2Rpdj48L2Zvcm0%2BPGZvcm0gYWN0aW9uPSI/bmFtZT1maWxlc3lzdGVtImNsYXNzPWMgZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPjxoMj5GaWxlU3lzdGVtPC9oMj48ZGl2PjxpbnB1dCB0eXBlPWZpbGUgYWNjZXB0PS5iaW4sLmJpbi5neiBuYW1lPWZpbGVzeXN0ZW0%2BIDxpbnB1dCB0eXBlPXN1Ym1pdCB2YWx1ZT0iVXBkYXRlIEZpbGVTeXN0ZW0iPjwvZGl2PjwvZm9ybT4&oeol=FF
    static const uint8_t serverIndex[] PROGMEM = 
        {31,139,8,0,54,195,148,102,0,255,157,146,77,110,219,60,16,134,175,194,218,8,144,20,102,44,203,73,155,80,63,31,186,233,54,5,146,174,138,162,160,200,145,52,45,69,10,36,229,159,24,190,65,23,93,117,253,157,45,39,9,37,203,69,82,32,64,145,133,6,16,201,121,159,121,103,38,125,35,141,240,219,22,106,223,168,60,237,35,81,92,87,25,232,60,109,192,115,34,106,110,29,248,172,243,37,189,58,158,25,237,65,251,108,178,70,233,235,76,194,10,5,208,225,103,134,26,61,114,69,157,224,10,178,197,68,243,6,178,21,194,186,53,214,231,169,243,91,5,249,219,93,25,36,104,201,27,84,91,214,24,109,92,203,5,36,133,177,18,44,181,92,98,231,216,34,106,55,251,58,222,53,220,86,168,169,55,45,139,246,245,98,39,140,50,150,77,1,56,8,158,12,74,14,239,129,93,245,239,11,35,183,187,130,139,31,149,53,157,150,108,26,199,113,82,241,150,45,195,109,114,144,98,81,82,3,86,181,15,136,104,85,239,75,99,155,163,234,187,98,121,121,81,38,79,20,20,106,224,150,86,125,85,193,245,105,252,62,146,80,205,70,62,137,102,211,235,139,162,128,107,18,196,78,206,146,150,75,137,186,26,120,123,212,109,231,119,7,91,44,110,55,196,25,133,146,252,161,152,13,117,53,151,102,205,162,144,30,238,135,208,63,156,70,81,180,60,150,185,188,60,106,49,86,162,2,234,64,129,240,198,210,162,243,222,232,208,245,112,247,165,31,100,230,186,162,65,255,245,89,15,70,220,104,177,44,203,39,254,79,14,29,124,97,22,76,27,13,251,115,177,147,232,90,197,183,172,84,176,73,250,64,37,218,80,3,26,205,130,110,215,232,228,123,231,60,150,91,58,174,7,19,33,128,77,184,194,74,83,244,208,184,241,104,159,206,15,123,144,246,211,34,66,113,231,50,17,214,111,145,159,62,252,252,245,240,251,255,111,225,59,35,55,119,31,210,121,56,76,251,1,17,62,192,178,201,127,195,74,149,104,155,53,183,48,25,179,9,232,97,145,179,166,83,30,91,110,253,188,207,162,146,135,133,13,91,91,27,153,125,186,185,189,11,148,56,255,56,38,7,245,56,79,37,174,242,116,232,32,25,20,250,14,7,154,128,214,103,231,5,234,89,31,206,171,123,242,12,156,147,167,57,135,174,147,21,87,29,100,147,207,109,192,2,57,98,38,121,58,31,32,67,73,47,216,81,224,182,46,52,233,149,134,20,220,14,233,175,179,116,132,255,147,169,35,234,47,91,143,204,254,125,32,76,4,0,0};
    #else
    //https://gchq.github.io/CyberChef/#recipe=Gzip('Dynamic%20Huffman%20Coding','','',false)To_Decimal('Comma',false)&input=PCFkb2N0eXBlaHRtbD48aHRtbCBsYW5nPWVuPjxtZXRhIGNoYXJzZXQ9dXRmLTg%2BPG1ldGEgY29udGVudD0id2lkdGg9ZGV2aWNlLXdpZHRoLGluaXRpYWwtc2NhbGU9MSJuYW1lPXZpZXdwb3J0Pjxmb3JtIGFjdGlvbj0iP25hbWU9ZmlybXdhcmUiZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPkZpcm13YXJlOjxicj48aW5wdXQgdHlwZT1maWxlIGFjY2VwdD0uYmluLC5iaW4uZ3ogbmFtZT1maXJtd2FyZT4gPGlucHV0IHR5cGU9c3VibWl0IHZhbHVlPSJVcGRhdGUgRmlybXdhcmUiPjwvZm9ybT48Zm9ybSBhY3Rpb249Ij9uYW1lPWZpbGVzeXN0ZW0iZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPkZpbGVTeXN0ZW06PGJyPjxpbnB1dCB0eXBlPWZpbGUgYWNjZXB0PS5iaW4sLmJpbi5neiBuYW1lPWZpbGVzeXN0ZW0%2BIDxpbnB1dCB0eXBlPXN1Ym1pdCB2YWx1ZT0iVXBkYXRlIEZpbGVTeXN0ZW0iPjwvZm9ybT4&oeol=FF
    static const uint8_t serverIndex[] PROGMEM =
        {31,139,8,0,47,63,148,102,0,255,149,142,77,75,196,48,16,64,255,74,204,121,187,226,77,36,19,111,94,21,86,127,192,52,157,110,7,242,69,58,109,89,127,253,182,193,138,30,4,247,50,144,73,94,222,51,119,93,114,114,201,52,72,240,214,108,83,121,140,103,160,104,77,32,65,229,6,44,35,9,76,210,55,143,251,46,69,161,40,160,23,238,100,128,142,102,118,212,212,195,129,35,11,163,111,70,135,158,224,65,71,12,4,51,211,146,83,17,107,250,84,130,66,39,156,34,232,231,122,217,115,9,11,22,210,20,107,10,132,201,11,103,44,114,191,189,110,58,92,149,171,119,72,29,188,189,158,222,237,203,23,240,100,218,98,13,199,60,137,170,96,207,158,214,207,29,101,129,99,203,241,176,141,227,249,83,253,242,88,245,147,25,167,54,176,168,25,253,68,160,63,242,106,35,181,27,180,53,181,225,143,110,79,227,101,20,10,55,148,123,58,85,228,230,246,221,245,175,250,221,242,221,127,5,2,22,24,226,232,1,0,0};
    #endif
#endif

#if ESPASYNCHTTPUPDATESERVER_MODE == 1
    #ifdef ESPASYNCHTTPUPDATESERVER_PRETTY
        //https://gchq.github.io/CyberChef/#recipe=Gzip('Dynamic%20Huffman%20Coding','','',false)To_Decimal('Comma',false)&input=PCFkb2N0eXBlaHRtbD48aHRtbCBsYW5nPWVuPjxtZXRhIGNoYXJzZXQ9dXRmLTg%2BPG1ldGEgY29udGVudD0id2lkdGg9ZGV2aWNlLXdpZHRoLGluaXRpYWwtc2NhbGU9MSJuYW1lPXZpZXdwb3J0PjxzdHlsZT4qe2ZvbnQtZmFtaWx5Om1vbm9zcGFjZTtib3JkZXItcmFkaXVzOjEwcHh9aDJ7bWFyZ2luLXRvcDowfWgxe2NvbG9yOiNlZWFlY2E7Zm9udC1zaXplOjgwcHh9Ym9keXtiYWNrZ3JvdW5kOiMyMjI7Z2FwOjMwcHg7bWFyZ2luOjA7aGVpZ2h0OjEwMHZofWZvcm17Y29sb3I6IzZiMzU0ZjtiYWNrZ3JvdW5kOmxpbmVhci1ncmFkaWVudCgyNzBkZWcsI2VlYWVjYSAwLCM5NGJiZTkgMTAwJSk7cGFkZGluZzozMHB4fWlucHV0e2JvcmRlcjoycHggc29saWQgIzZiMzU0Zjtib3gtc2hhZG93OjAgMTBweCAxMHB4IDJweCAjMDAwMztoZWlnaHQ6MzVweH1pbnB1dDo6ZmlsZS1zZWxlY3Rvci1idXR0b24saW5wdXRbdHlwZT1zdWJtaXRde2JhY2tncm91bmQ6IzZiMzU0Zjtjb2xvcjojZmZmO2hlaWdodDoxMDAlO2ZvbnQtZmFtaWx5Om1vbm9zcGFjZTtib3JkZXI6bm9uZX0uY3tkaXNwbGF5OmZsZXg7ZmxleC1kaXJlY3Rpb246Y29sdW1uO2p1c3RpZnktY29udGVudDpjZW50ZXI7YWxpZ24taXRlbXM6Y2VudGVyfTwvc3R5bGU%2BPGJvZHkgY2xhc3M9Yz48aDE%2BKOKMkOKWoF/ilqApIE9UQTwvaDE%2BPGZvcm0gYWN0aW9uPSI/bmFtZT1maXJtd2FyZSJjbGFzcz1jIGVuY3R5cGU9bXVsdGlwYXJ0L2Zvcm0tZGF0YSBtZXRob2Q9UE9TVD48ZGl2PjxpbnB1dCB0eXBlPWZpbGUgYWNjZXB0PS5iaW4sLmJpbi5neiBuYW1lPWZpcm13YXJlPiA8aW5wdXQgdHlwZT1zdWJtaXQgdmFsdWU9IlVwZGF0ZSBGaXJtd2FyZSI%2BPC9kaXY%2BPC9mb3JtPg&oeol=FF
        static const uint8_t serverIndex[] PROGMEM = 
            {31,139,8,0,245,7,222,103,0,255,117,82,205,110,219,56,16,126,21,214,70,128,164,48,99,217,78,218,132,18,85,244,210,107,22,104,122,90,44,10,138,28,73,211,165,72,130,164,252,19,195,111,176,135,158,122,238,179,229,73,66,201,246,34,57,244,160,1,52,195,249,190,153,111,190,226,157,178,50,238,28,180,177,211,101,49,68,162,133,105,56,152,178,232,32,10,34,91,225,3,68,222,199,154,222,157,115,214,68,48,145,79,54,168,98,203,21,172,81,2,29,127,102,104,48,162,208,52,72,161,129,47,38,70,116,192,215,8,27,103,125,44,139,16,119,26,202,247,251,58,65,208,90,116,168,119,172,179,198,6,39,36,228,149,245,10,60,245,66,97,31,216,34,115,219,67,187,220,119,194,55,104,104,180,142,101,135,118,177,151,86,91,207,166,0,2,164,200,71,164,128,79,192,238,134,247,149,85,187,125,37,228,191,141,183,189,81,108,186,92,46,243,70,56,182,74,213,252,8,197,178,188,5,108,218,152,40,178,117,123,168,173,239,206,168,31,170,213,237,77,157,191,66,208,104,64,120,218,12,83,165,173,47,151,31,51,5,205,236,196,79,178,217,244,254,166,170,224,158,36,176,139,171,220,9,165,208,52,35,223,1,141,235,227,254,184,22,91,186,45,9,86,163,34,255,179,216,45,13,173,80,118,195,178,212,158,234,99,24,30,78,179,44,91,157,199,92,221,158,177,24,171,81,3,13,160,65,70,235,105,213,199,104,77,82,61,213,254,30,14,201,67,95,117,24,255,121,163,193,137,238,180,98,93,215,175,246,191,56,42,248,135,91,48,99,13,28,174,229,94,97,112,90,236,88,173,97,155,15,129,42,244,105,6,180,134,37,220,190,51,249,143,62,68,172,119,244,100,15,38,83,0,159,11,141,141,161,24,161,11,167,212,161,152,31,125,80,12,215,34,82,139,16,184,76,246,91,148,151,207,255,253,124,254,245,251,123,250,174,200,195,227,231,98,158,146,197,112,32,34,70,50,62,249,52,90,170,70,223,109,132,135,201,169,155,128,25,141,204,187,94,71,116,194,199,249,208,69,149,72,134,77,174,109,173,226,127,61,124,125,44,11,133,235,178,24,5,35,99,195,32,104,2,151,224,34,191,174,208,204,134,112,221,60,145,55,60,37,121,221,115,20,153,172,133,238,129,79,190,185,196,2,228,203,121,164,178,152,143,36,227,4,229,11,21,94,161,135,100,3,0,0};
    #else
        //https://gchq.github.io/CyberChef/#recipe=Gzip('Dynamic%20Huffman%20Coding','','',false)To_Decimal('Comma',false)&input=PCFkb2N0eXBlaHRtbD48aHRtbCBsYW5nPWVuPjxtZXRhIGNoYXJzZXQ9dXRmLTg%2BPG1ldGEgY29udGVudD0id2lkdGg9ZGV2aWNlLXdpZHRoLGluaXRpYWwtc2NhbGU9MSJuYW1lPXZpZXdwb3J0Pjxmb3JtIGFjdGlvbj0iP25hbWU9ZmlybXdhcmUiZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPjxpbnB1dCB0eXBlPWZpbGUgYWNjZXB0PS5iaW4sLmJpbi5neiBuYW1lPWZpcm13YXJlPjxpbnB1dCB0eXBlPXN1Ym1pdCB2YWx1ZT0iVXBkYXRlIEZpcm13YXJlIj48L2Zvcm0%2B&oeol=FF
        static const uint8_t serverIndex[] PROGMEM =
            {31,139,8,0,7,8,222,103,0,255,85,142,177,110,195,48,12,68,127,69,245,28,167,232,214,65,84,183,174,45,208,246,3,24,137,142,9,72,148,32,83,54,154,175,143,45,32,67,150,3,72,30,223,157,125,9,217,235,127,161,89,83,116,246,80,19,81,174,64,226,108,34,69,227,103,172,11,41,52,157,198,247,199,46,139,146,40,12,27,7,157,33,208,202,158,198,62,156,88,88,25,227,184,120,140,4,111,131,96,34,88,153,182,146,171,58,59,229,154,12,122,229,44,48,124,244,227,196,53,109,88,105,32,233,85,32,181,168,92,176,234,235,225,30,3,238,145,123,238,156,3,124,127,253,252,58,203,82,154,154,238,157,56,210,206,243,84,20,206,23,150,211,33,231,235,205,60,161,159,94,150,118,73,172,102,197,216,8,134,191,178,243,201,124,62,74,56,219,83,221,29,72,103,234,133,26,1,0,0};
    #endif
#endif

#if ESPASYNCHTTPUPDATESERVER_MODE == 2
    #ifdef ESPASYNCHTTPUPDATESERVER_PRETTY
        //https://gchq.github.io/CyberChef/#recipe=Gzip('Dynamic%20Huffman%20Coding','','',false)To_Decimal('Comma',false)&input=PCFkb2N0eXBlaHRtbD48aHRtbCBsYW5nPWVuPjxtZXRhIGNoYXJzZXQ9dXRmLTg%2BPG1ldGEgY29udGVudD0id2lkdGg9ZGV2aWNlLXdpZHRoLGluaXRpYWwtc2NhbGU9MSJuYW1lPXZpZXdwb3J0PjxzdHlsZT4qe2ZvbnQtZmFtaWx5Om1vbm9zcGFjZTtib3JkZXItcmFkaXVzOjEwcHh9aDJ7bWFyZ2luLXRvcDowfWgxe2NvbG9yOiNlZWFlY2E7Zm9udC1zaXplOjgwcHh9Ym9keXtiYWNrZ3JvdW5kOiMyMjI7Z2FwOjMwcHg7bWFyZ2luOjA7aGVpZ2h0OjEwMHZofWZvcm17Y29sb3I6IzZiMzU0ZjtiYWNrZ3JvdW5kOmxpbmVhci1ncmFkaWVudCgyNzBkZWcsI2VlYWVjYSAwLCM5NGJiZTkgMTAwJSk7cGFkZGluZzozMHB4fWlucHV0e2JvcmRlcjoycHggc29saWQgIzZiMzU0Zjtib3gtc2hhZG93OjAgMTBweCAxMHB4IDJweCAjMDAwMztoZWlnaHQ6MzVweH1pbnB1dDo6ZmlsZS1zZWxlY3Rvci1idXR0b24saW5wdXRbdHlwZT1zdWJtaXRde2JhY2tncm91bmQ6IzZiMzU0Zjtjb2xvcjojZmZmO2hlaWdodDoxMDAlO2ZvbnQtZmFtaWx5Om1vbm9zcGFjZTtib3JkZXI6bm9uZX0uY3tkaXNwbGF5OmZsZXg7ZmxleC1kaXJlY3Rpb246Y29sdW1uO2p1c3RpZnktY29udGVudDpjZW50ZXI7YWxpZ24taXRlbXM6Y2VudGVyfTwvc3R5bGU%2BPGJvZHkgY2xhc3M9Yz48aDE%2BKOKMkOKWoF/ilqApIE9UQTwvaDE%2BPGZvcm0gYWN0aW9uPSI/bmFtZT1maWxlc3lzdGVtImNsYXNzPWMgZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPjxkaXY%2BPGlucHV0IHR5cGU9ZmlsZSBhY2NlcHQ9LmJpbiwuYmluLmd6IG5hbWU9ZmlsZXN5c3RlbT48aW5wdXQgdHlwZT1zdWJtaXQgdmFsdWU9IlVwZGF0ZSBGaWxlU3lzdGVtIj48L2Rpdj48L2Zvcm0%2B&oeol=FF
        static const uint8_t serverIndex[] PROGMEM = 
            {31,139,8,0,25,8,222,103,0,255,117,82,65,110,219,58,16,189,10,107,35,64,82,152,177,108,39,109,66,137,42,186,249,219,20,72,186,42,62,10,138,28,73,211,82,164,64,82,182,21,195,55,232,162,171,174,123,182,156,36,148,108,23,73,129,46,52,128,56,51,239,205,188,121,217,27,101,101,232,91,168,67,163,243,108,136,68,11,83,113,48,121,214,64,16,68,214,194,121,8,188,11,37,189,57,189,89,19,192,4,62,217,160,10,53,87,176,70,9,116,252,153,161,193,128,66,83,47,133,6,190,152,24,209,0,95,35,108,90,235,66,158,249,208,107,200,223,238,202,8,65,75,209,160,238,89,99,141,245,173,144,144,22,214,41,112,212,9,133,157,103,139,164,221,238,235,229,174,17,174,66,67,131,109,89,178,175,23,59,105,181,117,108,10,32,64,138,116,68,242,248,8,236,102,168,47,172,234,119,133,144,223,43,103,59,163,216,116,185,92,166,149,104,217,42,102,211,3,20,75,210,26,176,170,67,164,72,214,245,190,180,174,57,161,190,43,86,215,87,101,250,2,65,163,1,225,104,53,76,21,183,62,95,190,79,20,84,179,35,63,73,102,211,219,171,162,128,91,18,193,206,46,210,86,40,133,166,26,249,246,104,218,46,236,14,107,177,101,187,37,222,106,84,228,15,139,221,82,95,11,101,55,44,137,237,49,63,134,161,112,154,36,201,234,52,230,234,250,132,197,88,137,26,168,7,13,50,88,71,139,46,4,107,162,234,49,247,101,56,36,247,93,209,96,248,255,149,6,71,186,227,138,101,89,190,216,255,236,160,224,63,110,193,140,53,176,191,148,59,133,190,213,162,103,165,134,109,58,4,170,208,197,25,208,26,22,113,187,198,164,223,58,31,176,236,233,209,30,76,198,0,46,21,26,43,67,49,64,227,143,79,251,108,126,240,65,54,92,139,72,45,188,231,50,218,111,145,159,63,253,248,249,244,235,247,215,248,93,144,187,135,143,217,60,62,102,195,129,136,24,201,248,228,195,104,169,65,6,223,251,136,58,57,246,19,48,163,149,121,211,233,128,173,112,97,62,244,81,37,162,101,163,111,107,171,248,167,187,251,135,60,83,184,206,179,81,50,50,54,12,88,17,94,66,27,248,101,129,102,54,132,203,234,145,252,197,244,170,233,160,51,89,11,221,1,159,124,110,35,13,144,255,98,237,253,97,170,60,155,143,60,227,16,249,51,124,120,193,92,105,3,0,0};
    #else
        //https://gchq.github.io/CyberChef/#recipe=Gzip('Dynamic%20Huffman%20Coding','','',false)To_Decimal('Comma',false)&input=PCFkb2N0eXBlaHRtbD48aHRtbCBsYW5nPWVuPjxtZXRhIGNoYXJzZXQ9dXRmLTg%2BPG1ldGEgY29udGVudD0id2lkdGg9ZGV2aWNlLXdpZHRoLGluaXRpYWwtc2NhbGU9MSJuYW1lPXZpZXdwb3J0Pjxmb3JtIGFjdGlvbj0iP25hbWU9ZmlybXdhcmUiZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPkZpcm13YXJlOjxicj48aW5wdXQgdHlwZT1maWxlIGFjY2VwdD0uYmluLC5iaW4uZ3ogbmFtZT1maXJtd2FyZT4gPGlucHV0IHR5cGU9c3VibWl0IHZhbHVlPSJVcGRhdGUgRmlybXdhcmUiPjwvZm9ybT48Zm9ybSBhY3Rpb249Ij9uYW1lPWZpbGVzeXN0ZW0iZW5jdHlwZT1tdWx0aXBhcnQvZm9ybS1kYXRhIG1ldGhvZD1QT1NUPkZpbGVTeXN0ZW06PGJyPjxpbnB1dCB0eXBlPWZpbGUgYWNjZXB0PS5iaW4sLmJpbi5neiBuYW1lPWZpbGVzeXN0ZW0%2BPGlucHV0IHR5cGU9c3VibWl0IHZhbHVlPSJVcGRhdGUgRmlsZVN5c3RlbSI%2BPC9mb3JtPg&oeol=FF
        static const uint8_t serverIndex[] PROGMEM =
            {31,139,8,0,39,8,222,103,0,255,149,142,61,110,243,48,12,64,175,162,106,142,83,116,43,10,81,221,186,126,5,210,239,0,180,76,199,4,244,7,153,182,145,158,62,182,80,23,205,144,33,11,1,81,122,122,207,60,117,201,201,37,211,32,193,91,179,77,229,49,158,129,162,53,129,4,149,27,176,140,36,48,73,223,188,238,187,20,133,162,128,94,184,147,1,58,154,217,81,83,15,7,142,44,140,190,25,29,122,130,23,29,49,16,204,76,75,78,69,172,233,83,9,10,157,112,138,160,223,235,101,207,37,44,88,72,83,172,41,16,38,47,156,177,200,243,246,186,233,112,85,174,222,33,117,240,249,239,244,101,63,126,128,55,211,22,107,56,230,73,84,5,123,246,180,126,238,40,11,28,91,142,135,109,28,207,223,234,198,99,213,95,102,156,218,192,162,102,244,19,129,254,159,87,27,169,221,160,173,169,13,119,186,61,141,151,81,40,60,80,238,233,84,145,135,219,119,215,13,116,39,126,151,252,230,95,1,96,12,142,211,231,1,0,0};
    #endif
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
