#pragma once

#include <Arduino.h>
class Potentiometer {

private:
  uint8_t pin;

public:

  Potentiometer(uint8_t pin) : pin(pin) {}
  void init() {
    analogSetPinAttenuation(pin, ADC_6db);// ADC_11db);
    /*
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
*/

  }
  float update() {
    return analogRead(pin)/1;
  }
  Potentiometer(Potentiometer&& ) = default; //= delete;
  Potentiometer(const Potentiometer& ) = delete;
  Potentiometer& operator=(Potentiometer&& ) = default;// = delete;
  Potentiometer& operator=(const Potentiometer& ) = delete;
};