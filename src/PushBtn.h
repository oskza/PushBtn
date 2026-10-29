#ifndef PUSH_BTN_H
#define PUSH_BTN_H
#include <stdint.h>
#include <Timer.h>

class PushBtn {
public:
    enum class Polarity : uint8_t {
        ACTIVE_LOW,
        ACTIVE_HIGH
    };
    enum class Event : uint8_t {
        NONE,
        CLICK,
        LONG_PRESS
    };
    explicit PushBtn(
        uint8_t pin,
        Polarity polarity = Polarity::ACTIVE_LOW,
        uint32_t longPressTimeoutMs = 3000
    );
    void begin();
    Event poll(uint32_t nowMs);
    Polarity getPolarity() const;
    void setPolarity(Polarity polarity);
    uint32_t getLongPressTimeout() const;
    void setLongPressTimeout(uint32_t timeoutMs);
    uint32_t getDebounceTimeout() const;
    void setDebounceTimeout(uint32_t timeoutMs);
private:
    uint8_t _pin;
    Polarity _polarity;
    uint32_t _longPressTimeoutMs;
    uint32_t _debounceTimeoutMs;
    Timer _debounceTimer;
    Timer _pressTimer;
    bool _initialized;
    bool _lastPressed;
    bool _stablePressed;
    bool _longPressTriggered;
    bool _readPressed() const;
};
#endif
