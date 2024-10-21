#ifndef ScanWifi_H
#define ScanWifi_H

#ifdef ESP32
  #include <WiFi.h>
  #include <WebServer.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#endif

#ifdef ESP32
  extern WebServer server;
#else
  extern ESP8266WebServer server;
#endif

void connectWifiAP(const char* ssid, const char* pass);
void wifiHotspot(const char* ssidAP, const char* passAP);
void setupServer();
void handleClient();

#endif
