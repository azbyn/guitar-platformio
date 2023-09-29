#pragma once

#include <Arduino.h>
class Switch3w {

private:
  uint8_t pinA, pinB;

  int last_val = 0;

public:

  Switch3w(uint8_t pinA, uint8_t pinB) : pinA(pinA), pinB(pinB) {}
  void init() {
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);


  }

  bool first = true;
  int update() {
    int val = get_val();
    if (val == last_val) return 0;
    last_val = val;
    if (first) {
      first = false;
      return 0;
    }
    return val;
  }
  int get_val() {
    bool a = digitalRead(pinA) == LOW;
    bool b = digitalRead(pinB) == LOW;


    if (a && !b) return 1;
    if (!a && b) return 3;
    return 2;
  }
  Switch3w(Switch3w&& ) = default; //= delete;
  Switch3w(const Switch3w& ) = delete;
  Switch3w& operator=(Switch3w&& ) = default;// = delete;
  Switch3w& operator=(const Switch3w& ) = delete;
};