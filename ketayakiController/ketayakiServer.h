//Webサーバーに関するクラスの定義

#ifndef KETAYAKISERVER_H
#define KETAYAKISERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

#define APSSID "Ketayaki Manegement Portal"
#define APPSK "12345678"

class ketayakiServer {
  public:
    ketayakiServer();
    static void handleRoot();
    static void getData();
    void loop();
    void setup();
    static char ketayaki_status[32]; // ←これのことね static指定はいらない気がするけど触りたくない
    static int target_temp, mode;
};

#endif // KETAYAKISERVER_H
