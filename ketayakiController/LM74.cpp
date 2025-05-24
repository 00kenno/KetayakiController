#include <Arduino.h>
#include "LM74.h"

LM74::LM74 (int SIO_PIN, int SC_PIN) {
  _SIO_PIN = SIO_PIN;
  _SC_PIN = SC_PIN;
}

void LM74::setup (int _CS_PIN) {
  pinMode(_SIO_PIN, INPUT); //Slave Input/Output
  pinMode(_CS_PIN, OUTPUT); //Chip Select input
  pinMode(_SC_PIN, OUTPUT); //Slave Clock

  digitalWrite(_CS_PIN, HIGH);
  digitalWrite(_SC_PIN, LOW);

  while (1) {
    digitalWrite(_CS_PIN, LOW); // Transfer start
    for (int i = 12; i >= 0; i--) {
      digitalWrite(_SC_PIN, HIGH);
      digitalWrite(_SC_PIN, LOW);
    }
    digitalWrite(_SC_PIN, HIGH);
    int d2 = digitalRead(_SIO_PIN);
    digitalWrite(_SC_PIN, LOW);
    for (int i = 0; i < 2; i++) {
      digitalWrite(_SC_PIN, HIGH);
      digitalWrite(_SC_PIN, LOW);
    }
    digitalWrite(_CS_PIN, HIGH); // Transfer end
    
    if (d2 == 1) {
      break;
    }
  }
}

float LM74::get (int _CS_PIN) {
  int16_t data = 0;
  
  digitalWrite(_CS_PIN, LOW); // Transfer start
  digitalWrite(_SC_PIN, HIGH);
  data = digitalRead(_SIO_PIN) << 15;
  digitalWrite(_SC_PIN, LOW);
    for (int i = 11; 0 <= i; i--) {
    digitalWrite(_SC_PIN, HIGH);
    int bit = digitalRead(_SIO_PIN);
    bit = bit << i;
    data |= bit;
    digitalWrite(_SC_PIN, LOW);
  }
  for (int i = 0; i < 3; i++) {
    digitalWrite(_SC_PIN, HIGH);
    digitalWrite(_SC_PIN, LOW);
  }
  digitalWrite(_CS_PIN, HIGH); // Transfer end

  return (float)data * 0.0625;
}
