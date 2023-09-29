#pragma once

#include <Arduino.h>

#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20
//https://github.com/buxtronix/arduino/blob/master/libraries/Rotary/Rotary.cpp
//https://circuits4you.com/2016/05/13/rotary-encoder-arduino-ky-040/

#define R_START 0x0
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

constexpr const uint8_t ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

class RotaryEncoder {
  //      o btn
  // [  ] o gnd
  // [  ] o enc A
  //      o enc B

private:
  uint8_t state = R_START;
  uint8_t pinA;
  uint8_t pinB;

public:

  RotaryEncoder(uint8_t pinA, uint8_t pinB) : pinA(pinA), pinB(pinB) {}
  void init() {
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);

    //attachInterruptArg(pinA, [](void* p){ ((RotaryEncoder*)p)->onPinUpdate(); }, this, CHANGE);
    //attachInterruptArg(pinB, [](void* p){ ((RotaryEncoder*)p)->onPinUpdate(); }, this, CHANGE);

  }
  int update() {
    return onPinUpdate();
  }
  RotaryEncoder(RotaryEncoder&& ) = default; //= delete;
  RotaryEncoder(const RotaryEncoder& ) = delete;
  RotaryEncoder& operator=(RotaryEncoder&& ) = default;// = delete;
  RotaryEncoder& operator=(const RotaryEncoder& ) = delete;
private:

  int onPinUpdate() {
    uint8_t val = (digitalRead(pinA) << 1) | digitalRead(pinB);
    state = ttable[state& 0xF][val];

    if (state == (R_START | DIR_CW)) {
        return 1;
    } 
    else if (state == (R_START | DIR_CCW)) {
        return -1;
    }
    return 0;
  }
};