//HTTPClientに関するクラスの定義

#ifndef WINDVANECLIENT_H
#define WINDVANECLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define STASSID "TORICA Windvane"
#define STAPSK "12345678"

class windvaneClient {
  public:
    windvaneClient();
    void setupClient();
    void getData();
    int incrementAngle, pressure;
    float temp;

  private:
    int httpCode, dataNum, splitFrom, splitTo, preAbsoluteAngle, absoluteAngle;
    float data[5];
    String subStr, httpStr;
};

#endif // WINDVANECLIENT_H
