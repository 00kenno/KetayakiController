//Stepperに関するクラスの定義

#ifndef CUSTOMSTEPPER_H
#define CUSTOMSTEPPER_H

#include <Arduino.h>
#include <Stepper.h>

// Stepping Motor GPIO
#define SMTR_IN1  10
#define SMTR_IN2  11
#define SMTR_IN3  12
#define SMTR_IN4  13

// Stepper Value
#define STEPS 2048

// externで別ファイルでの定義を利用できるようになります
// customStepper.cppで定義されています
extern Stepper stepper;

class customStepper {
  public:
    customStepper();
    void setupStepper();
    void stopStepper();
    void zeroAdjustment();
};

#endif // CUSTOMSTEPPER_H
