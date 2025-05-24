#include "ketayakiServer.h"
ketayakiServer myServer;

void setup() {
  myServer.setup();
}

void loop() {
  myServer.loop();
  delay(5);
}

#include "LM74.h"
#define SIO_PIN 0
#define SC_PIN 1
#define CS1_PIN 2
#define CS2_PIN 3
#define CS3_PIN 4
LM74 mySensor(SIO_PIN, SC_PIN);

#include <QuickStats.h>
QuickStats stats;

#define SSR 28

void setup1() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  for (int i = 2; i < 5; i++) {
    mySensor.setup(i);
    delay(100);
  }

  pinMode(SSR, OUTPUT);
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
}

void loop1() {
  static uint32_t last_loop = 0;
  static uint32_t run_time = 0;
  uint32_t now = millis();
  if (myServer.mode != 0 && now - last_loop >= 1000) {
    last_loop = now;
    run_time++;
  }

  int number_of_data = 3;
  float temp_data[3];
  for (int i = 0; i < 3; i++) {
    temp_data[i] = mySensor.get(i+2) - 5.5;
    Serial.print(i+1);
    Serial.print(": ");
    Serial.println(temp_data[i]);
    delay(100);
  }
  float current_temp = stats.median(temp_data, number_of_data);

  switch (myServer.mode) {
    case 1:
      Serial.print(".");
      digitalWrite(SSR, HIGH);
      break;
    case 2:
      Serial.print("..");
      digitalWrite(SSR, HIGH);
      break;
    default:
      digitalWrite(SSR, LOW);
      break;
  }

  Serial.print("[");
  Serial.print(run_time);
  Serial.print(",");
  Serial.print(myServer.mode);
  Serial.print(",");
  Serial.print(current_temp, 1);
  Serial.print(",");
  Serial.print(myServer.target_temp);
  Serial.println("]");
  sprintf(myServer.ketayaki_status, "%d,%d,%.1f,%d", run_time, myServer.mode, current_temp, myServer.target_temp);
  delay(100);
}
