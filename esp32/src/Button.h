#pragma once
#include <Arduino.h>

class Button {
    uint8_t pin;
    unsigned long next_valid_time = 0;

    bool lastState = 1;
public:
    Button(uint8_t pin): pin(pin) {}

    void init() {
        pinMode(pin, INPUT_PULLUP);

        //attachInterruptArg(pin, [](void* p){ ((Button*)p)->onPinUpdate(); },
        //                   this, FALLING);
    }

    Button(const Button&) = delete;
    Button(Button&&) = default;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&) = default;


    int isDown() {
        if (digitalRead(pin) == HIGH) {
            lastState = 1;
        } else {
            if (lastState == 1) {
                lastState = 0;
                auto t = millis();
                if (t > next_valid_time) {
                    next_valid_time = t + 200;
                    return 1;
                }
            }
        }
        return 0;
    }

private:
/*
    void onPinUpdate() {
        auto t = millis();
        if (t > next_valid_time) {

            next_valid_time = t + 10;
            onPressed();
        }
        //last_time = t;//millis();
    }*/
};