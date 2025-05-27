//Stepperに関するメンバ関数の定義

#include <Arduino.h>
#include "customStepper.h"

// ステッピングモータ制御用インスタンスの生成（SMTR_IN2 とSMTR_IN3を入れ替え）
Stepper stepper(STEPS, SMTR_IN1, SMTR_IN3, SMTR_IN2, SMTR_IN4);

customStepper::customStepper () {}

void customStepper::setupStepper () {
  pinMode(20, INPUT);//フォトトランジスタからの電流検出用
  pinMode(21, OUTPUT);//赤外線LEDの制御用
  stepper.setSpeed(10);
}

void customStepper::stopStepper () {
  digitalWrite(SMTR_IN1, LOW);
  digitalWrite(SMTR_IN3, LOW);
  digitalWrite(SMTR_IN2, LOW);
  digitalWrite(SMTR_IN4, LOW);
}

void customStepper::zeroAdjustment () {
  digitalWrite(21, HIGH);
  while (digitalRead(20) == 0) {
    stepper.step(1);
    Serial.print("zero adjustment... : ");
    Serial.println(digitalRead(20));//GNDにバイパスする抵抗：10kΩ
  }
  stopStepper();
  digitalWrite(21, LOW);
  Serial.println("Stepper ready");
  digitalWrite(LED_BUILTIN, HIGH);
}
