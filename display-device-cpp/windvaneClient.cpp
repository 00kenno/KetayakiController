//HTTPClientに関するメンバ関数の定義

#include <Arduino.h>
#include "windvaneClient.h"

const char *ssid = STASSID;
const char *pass = STAPSK;

WiFiMulti WiFiMulti;

windvaneClient::windvaneClient () {}

void windvaneClient::setupClient () {
  delay(5000);
  WiFiMulti.addAP(ssid/*, pass*/);
  Serial.println("HTTP ready");
}

void windvaneClient::getData () {
  // WiFiの接続待ち
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    digitalWrite(LED_BUILTIN,HIGH);
    HTTPClient http;
    if (http.begin("http://172.217.28.1/get/data")) {  // HTTP
      // 接続を開始
      // HTTPヘッダーの送信
      httpCode = 0;
      httpCode = http.GET();
      // 応答がなかった場合（httpCode == 0）エラーになる
      if (httpCode > 0) {
        // サーバーからのレスポンスを受け取った後の制御
        // サーバーでファイルを検出
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          httpStr = http.getString();
          Serial.println(httpStr);

          // Stringをカンマ区切りで分割
          dataNum = 0;
          splitFrom = 0;
          while (1) {
            splitTo = httpStr.indexOf(',', splitFrom);
            if (splitTo == -1) {
              subStr = httpStr.substring(splitFrom);
              data[dataNum] = subStr.toFloat();
              break;
            }
            subStr = httpStr.substring(splitFrom, splitTo);
            data[dataNum] = subStr.toFloat();
            splitFrom = splitTo + 1;
            dataNum++;
          }

          //assign temp and pressure
          temp = data[1];
          pressure = (int)data[2];
          Serial.print("temp: ");
          Serial.println(temp);
          Serial.print("pressure: ");
          Serial.println(pressure);

          //assign angle and calc it
          absoluteAngle = (int)data[0];//absoluteにint型の絶対角度を入れる
          incrementAngle = absoluteAngle - preAbsoluteAngle;//前回の絶対角度(pAbsolute)を引いた値を相対角度にする
          if (incrementAngle > 180 && incrementAngle <= 360) {
            incrementAngle -= 360;//180°より大きい値は逆にする
          }
          else if (incrementAngle < -180 && incrementAngle >= -360) {
            incrementAngle += 360;//-180°より小さい値は逆にする
          }
          else if (incrementAngle > 360 || incrementAngle < -360) {
            incrementAngle = 0;
          }
          preAbsoluteAngle = absoluteAngle;//次回使うために値を代入
          
          Serial.print(absoluteAngle);
          Serial.print(" : ");
          Serial.println(incrementAngle);
          
          digitalWrite(LED_BUILTIN,LOW);
        }
      }
      else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP] Unable to connect\n");
    }
  }
}
