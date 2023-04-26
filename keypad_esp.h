#include "esphome.h"
#include <array>
#include <string>

class KeypadComponent : public Component, public CustomAPIDevice {
public:
    const int numCols = 3;
    const int numRows = 4;
    const std::array<int, 3> columnPins {4, 0, 2};
    const std::array<int, 4> rowPins {5, 14, 12, 13};
    const int buzzPin = 15;
    const int ledPin = 1;
    const std::array<std::array<char, 3>, 4> keys {{
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
        }};
    void setup() override {
        for (const int pin : columnPins) {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW);
        }
        for (const int pin : rowPins) {
            pinMode(pin, INPUT);
        }
    }

    void loop() override {
        std::string keys;
        for (size_t col = 0; col < columnPins.size(); ++col) {
            const int colPin = columnPins[col];
            digitalWrite(colPin, HIGH);

            for (size_t row = 0; row < rowPins.size(); ++row) {
                const int rowPin = rowPins[row];
                
                if (digitalRead(rowPin)) {
                    keys.push_back(keys[row][col]);
                }
            }
            digitalWrite(colPin, LOW);
        }

        if (keys.size() != 1) {
            return;
        }

        code.push_back(keys.front());
        if (code.size() == 4) {
            fire_homeassistant_event("keypad", {{"code", code}});
            code.clear();
        }
    }

private:
    std::string code;
    
};
