#include <Arduino.h>
#include <PushBtn.h>

PushBtn button(17, PushBtn::Polarity::ACTIVE_LOW, 3000);

void setup() {
    Serial.begin(115200);
    button.begin();
}

void loop() {
    switch (button.poll(millis())) {
        case PushBtn::Event::CLICK:
            Serial.println("Click");
            break;
        case PushBtn::Event::LONG_PRESS:
            Serial.println("Long press");
            break;
        case PushBtn::Event::NONE:
            break;
    }
}
