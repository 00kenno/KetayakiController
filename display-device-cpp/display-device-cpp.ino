//表示デバイスソフト

#include <Arduino.h>

#include <LiquidCrystal_I2C.h> // https://github.com/johnrickman/LiquidCrystal_I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include "customStepper.h"
customStepper displayStepper;

#include "windvaneClient.h"
windvaneClient picoClient;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Serial began...");

  //setup LCD
  //Wire.setSDA(14);
  //Wire.setSCL(15);
  lcd.init(); // Wire.begin()が含まれる
  //Wire.setClock(100000);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("TORICA");
  lcd.setCursor(0, 1);
  lcd.print("Windvane");

  //Setup stepper
  displayStepper.setupStepper();
  displayStepper.zeroAdjustment();

  //setup windvaneClient
  picoClient.setupClient();
}

void loop() {
  picoClient.getData();

  if (picoClient.incrementAngle != 0) {
    int STEP = (int)(-256.0 * (float)picoClient.incrementAngle / 45.0);
    stepper.step(STEP);
    displayStepper.stopStepper();
  }

  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(picoClient.temp);
  lcd.print("'C");

  lcd.setCursor(0, 1);
  lcd.print("Pressure:");
  lcd.print(picoClient.pressure);
  lcd.print("hPa");

  Serial.print("Temp: ");
  Serial.println(picoClient.temp);
  
  Serial.print("Pressure: ");
  Serial.println(picoClient.pressure);

  delay(500);
}
