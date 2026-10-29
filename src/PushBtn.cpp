#include "PushBtn.h"
#include <Arduino.h>

PushBtn::PushBtn(uint8_t pin, Polarity polarity, uint32_t longPressTimeoutMs)
    : _pin(pin),
    _polarity(polarity),
    _longPressTimeoutMs(longPressTimeoutMs),
    _debounceTimeoutMs(50),
    _debounceTimer(),
    _pressTimer(),
    _initialized(false),
    _lastPressed(false),
    _stablePressed(false),
    _longPressTriggered(false) {}

bool PushBtn::_readPressed() const {
    return digitalRead(_pin) == (_polarity == Polarity::ACTIVE_LOW ? LOW : HIGH);
}

void PushBtn::begin() {
    pinMode(_pin, _polarity == Polarity::ACTIVE_LOW ? INPUT_PULLUP : INPUT_PULLDOWN);
    _lastPressed = _readPressed();
    _stablePressed = false;
    _longPressTriggered = false;
    _debounceTimer.stop();
    _pressTimer.stop();
    _initialized = true;
}

PushBtn::Event PushBtn::poll(uint32_t nowMs) {
    if (!_initialized)
        return Event::NONE;
    const bool pressed = _readPressed();
    if (!_debounceTimer.isRunning() || pressed != _lastPressed) {
        _lastPressed = pressed;
        _debounceTimer.start(nowMs);
    }
    if (pressed != _stablePressed && _debounceTimer.hasElapsed(nowMs, _debounceTimeoutMs)) {
        _stablePressed = pressed;
        if (pressed) {
            _pressTimer.start(nowMs);
            _longPressTriggered = false;
        } else {
            const uint32_t heldMs = _pressTimer.getElapsedTime(nowMs)
                - _debounceTimer.getElapsedTime(nowMs);
            _pressTimer.stop();
            if (!_longPressTriggered)
                return heldMs >= _longPressTimeoutMs ? Event::LONG_PRESS : Event::CLICK;
        }
    }
    if (_stablePressed && pressed && !_longPressTriggered
            && _pressTimer.hasElapsed(nowMs, _longPressTimeoutMs)) {
        _longPressTriggered = true;
        return Event::LONG_PRESS;
    }
    return Event::NONE;
}

PushBtn::Polarity PushBtn::getPolarity() const { return _polarity; }

void PushBtn::setPolarity(Polarity polarity) {
    if (_polarity == polarity)
        return;
    _polarity = polarity;
    if (_initialized)
        begin();
}

uint32_t PushBtn::getLongPressTimeout() const { return _longPressTimeoutMs; }

void PushBtn::setLongPressTimeout(uint32_t timeoutMs) { _longPressTimeoutMs = timeoutMs; }

uint32_t PushBtn::getDebounceTimeout() const { return _debounceTimeoutMs; }

void PushBtn::setDebounceTimeout(uint32_t timeoutMs) { _debounceTimeoutMs = timeoutMs; }
