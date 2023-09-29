#pragma once
#include <Arduino.h>

class Button {
    uint8_t pin;
    unsigned long next_valid_time = 0;
    void (*onPressed)();
public:
    Button(uint8_t pin, void (*onPressed)()): pin(pin), onPressed{onPressed} {}

    void init() {
        pinMode(pin, INPUT_PULLUP);

        attachInterruptArg(pin, [](void* p){ ((Button*)p)->onPinUpdate(); },
                           this, FALLING);
    }

    Button(const Button&) = delete;
    Button(Button&&) = default;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&) = default;

private:
    void onPinUpdate() {
        auto t = millis();
        if (t > next_valid_time) {

            next_valid_time = t + 10;
            onPressed();
        }
        //last_time = t;//millis();
    }
};