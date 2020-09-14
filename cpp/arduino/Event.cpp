#include "Event.h"

extern unsigned long micros();

unsigned long Event::_lastId;
List<Event*> Event::events; 
List<Observer*> Event::eventObservers;
List<PinEvent*> PinEvent::pinEvents; 
List<Observer*> PinEvent::pinEventObservers;
List<DigitalPinEvent*> DigitalPinEvent::digitalPinEvents; 
List<Observer*> DigitalPinEvent::digitalPinEventObservers;
List<AnalogPinEvent*> AnalogPinEvent::analogPinEvents; 
List<Observer*> AnalogPinEvent::analogPinEventObservers;

Event::Event() {
    _id = ++_lastId;
    _time = micros();
    events.pushBack(this);
}

Event::~Event() {
    _id = 0;
    _time = 0;
    events.remove(this);
}

void Event::addObserver(observerFunction_t function) {
    Observer* observer = new Observer(function);
    eventObservers.pushBack(observer);
}

void Event::removeObserver(observerFunction_t observer) {
    for (size_t i = 0; i < eventObservers.size(); ++i) {
        if (eventObservers.at(i)->function == observer) {
            delete eventObservers.removeAt(i);
            return;
        }
    }
    assert(false);
}

void Event::notify() {
    for (size_t i = 0; i < eventObservers.size(); ++i) {
        Observer* observer = eventObservers.at(i);
        observerFunction_t function = observer->function;
        (*function)(this);
    }
}

PinEvent::PinEvent(byte pin) : Event() {
    _pin = pin;
    pinEvents.pushBack(this);
}

PinEvent::~PinEvent() {
    _pin = 0;
    pinEvents.remove(this);
}

void PinEvent::addObserver(observerFunction_t function) {
    Observer* observer = new Observer(function);
    pinEventObservers.pushBack(observer);
}

void PinEvent::removeObserver(observerFunction_t observer) {
    for (size_t i = 0; i < pinEventObservers.size(); ++i) {
        if (pinEventObservers.at(i)->function == observer) {
            delete pinEventObservers.removeAt(i);
            return;
        }
    }
    assert(false);
}

void PinEvent::notify() {
    for (size_t i = 0; i < pinEventObservers.size(); ++i) {
        Observer* observer = pinEventObservers.at(i);
        observerFunction_t function = observer->function;
        (*function)((Event*) this);
    }
    Event::notify();
}

DigitalPinEvent::DigitalPinEvent(byte pin, bool value) : PinEvent(pin) {
    _value = value;
    digitalPinEvents.pushBack(this);
}

DigitalPinEvent::~DigitalPinEvent() {
    _value = 0;
    digitalPinEvents.remove(this);
}

void DigitalPinEvent::addObserver(observerFunction_t function, byte pin, pinEventType_t eventType) {
    Observer* observer = new Observer(function);
    digitalPinEventObservers.pushBack(observer);
    observer->details.digitalPin.pin = pin;
    observer->details.digitalPin.eventType = eventType;
    digitalPinEventObservers.pushBack(observer);
}

void DigitalPinEvent::removeObserver(observerFunction_t observer) {
    for (size_t i = 0; i < digitalPinEventObservers.size(); ++i) {
        if (digitalPinEventObservers.at(i)->function == observer) {
            delete digitalPinEventObservers.removeAt(i);
            return;
        }
    }
    assert(false);
}

DigitalPinEvent* DigitalPinEvent::getPriorEventForPin() {
    bool foundThis = false;
    for (size_t i = digitalPinEvents.size() - 1; i >= 0; --i) {
        DigitalPinEvent* each = digitalPinEvents.at(i);
        if (foundThis) {
            if (this->_pin == each->_pin) {
                return each;
            }
        } else {
            if (this == each) {
                foundThis = true;
            }
        }
    }
    return nullptr;
}

void DigitalPinEvent::notify() {
    for (size_t i = 0; i < digitalPinEventObservers.size(); ++i) {
        Observer* observer = digitalPinEventObservers.at(i);
        observerFunction_t function = observer->function;
        byte pin = observer->details.digitalPin.pin;
        pinEventType_t eventType = observer->details.digitalPin.eventType;
        if (pin == _pin || pin == ALL_PINS) {
            if (eventType == PIN_WRITTEN) {
                (*function)((Event*) this);
            } else if ((eventType == PIN_RISING && _value) 
                    || (eventType == PIN_FALLING && !_value) 
                    || (eventType == PIN_CHANGING)) {
                DigitalPinEvent* prior = getPriorEventForPin();
                if (!prior || (prior->_value != _value)) {
                    (*function)((Event*) this);
                }
            }
        }
    }
    PinEvent::notify();
}

DigitalPinEvent* DigitalPinEvent::newEvent(byte pin, bool value) {
    DigitalPinEvent* event = new DigitalPinEvent(pin, value);
    event->notify();
    return event;
}

AnalogPinEvent::AnalogPinEvent(byte pin, int value) : PinEvent(pin) {
    _value = value;
    analogPinEvents.pushBack(this);
}

AnalogPinEvent::~AnalogPinEvent() {
    _value = 0;
    analogPinEvents.remove(this);
}

void AnalogPinEvent::addObserver(observerFunction_t function) {
    Observer* observer = new Observer(function);
    analogPinEventObservers.pushBack(observer);
}

void AnalogPinEvent::removeObserver(observerFunction_t observer) {
    for (size_t i = 0; i < analogPinEventObservers.size(); ++i) {
        if (analogPinEventObservers.at(i)->function == observer) {
            delete analogPinEventObservers.removeAt(i);
            return;
        }
    }
    assert(false);
}

void AnalogPinEvent::notify() {
    for (size_t i = 0; i < analogPinEventObservers.size(); ++i) {
        Observer* observer = analogPinEventObservers.at(i);
        observerFunction_t function = observer->function;
        (*function)((Event*) this);
    }
    PinEvent::notify();
}

AnalogPinEvent* AnalogPinEvent::newEvent(byte pin, bool value) {
    AnalogPinEvent* event = new AnalogPinEvent(pin, value);
    event->notify();
    return event;
}
