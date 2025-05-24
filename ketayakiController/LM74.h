#ifndef LM74_H
#define LM74_H

#include <Arduino.h>
#include <stdint.h>

class LM74 {
  public:
    LM74(int SIO_PIN, int SC_PIN);
    void setup(int _CS_PIN);
    float get(int _CS_PIN);
    
  private:
    int _SIO_PIN, _SC_PIN;
};

#endif // LM74_H
