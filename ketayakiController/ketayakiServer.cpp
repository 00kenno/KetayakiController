//Webサーバーに関するメンバ関数の定義

#include <Arduino.h>
#include "ketayakiServer.h"
#include "index.h"

const char *softAP_ssid = APSSID;
const char *softAP_password = APPSK;
const byte DNS_PORT = 53;

DNSServer dnsServer;
WebServer server(80);
IPAddress apIP(172, 217, 28, 1);
IPAddress netMsk(255, 255, 255, 0);

ketayakiServer::ketayakiServer () {}

char ketayakiServer::ketayaki_status[32];

int ketayakiServer::target_temp = 0;
int ketayakiServer::mode = 0;

void ketayakiServer::handleRoot () {
  if (server.method() == HTTP_POST) {
    String mode_str = server.arg("mode");
    //Serial.println(mode_str);
    //ここでmodeを更新
    if (mode_str == "standby") {
      mode = 0;    
    }
    else if (mode_str == "thermostat") {
      mode = 1;
    }
    else if (mode_str == "const_rate_heating") {
      mode = 2;
    }
    
    String target_temp_str = server.arg("target_temp");
    target_temp = target_temp_str.toInt();
  }
  
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", html);
}

void ketayakiServer::getData () {
  server.send(200, "text/html", ketayaki_status);
}

void ketayakiServer::loop () {
  dnsServer.processNextRequest();
  server.handleClient();
}

void ketayakiServer::setup () {
  WiFi.disconnect(true);//これがないとwebServerを再起動できない
  delay(1000);//この遅延は必須（これがないとwebServerが起動しない）
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500);  // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", handleRoot);
  server.on("/generate_204", handleRoot);
  server.on("/fwlink", handleRoot);
  server.onNotFound(handleRoot);
  server.on("/get/data", getData);
  server.begin();
}
