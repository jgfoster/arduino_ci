#include <iostream>
#include "Event.h"

extern unsigned long micros();

long Event::_lastId;
List<Event*> Event::events; 
List<Observer*> Event::eventObservers;
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

void Event::removeObserver(observerFunction_t function) {
    for (long i = eventObservers.size() - 1; i >= 0; --i) {
        if (eventObservers.at(i)->function == function) {
            delete eventObservers.removeAt(i);
        }
    }
}

void Event::notify() {
    for (long i = 0; i < eventObservers.size(); ++i) {
        Observer* observer = eventObservers.at(i);
        observerFunction_t function = observer->function;
        (*function)(this);
    }
}

// ======================= DigitalPinEvent =======================

DigitalPinEvent::DigitalPinEvent(byte pin, bool value) : Event() {
    _pin = pin;
    _value = value;
    digitalPinEvents.pushBack(this);
}

DigitalPinEvent::~DigitalPinEvent() {
    _value = 0;
    digitalPinEvents.remove(this);
}

void DigitalPinEvent::addObserver(observerFunction_t function, byte pin, pinEventType_t eventType) {
    Observer* observer = new Observer(function);
    observer->details.digitalPin.pin = pin;
    observer->details.digitalPin.eventType = eventType;
    digitalPinEventObservers.pushBack(observer);
}

void DigitalPinEvent::removeObserver(observerFunction_t function) {
    for (long i = digitalPinEventObservers.size() - 1; i >= 0; --i) {
        if (digitalPinEventObservers.at(i)->function == function) {
            delete digitalPinEventObservers.removeAt(i);
        }
    }
}

DigitalPinEvent* DigitalPinEvent::getPriorEventForPin() {
    bool foundThis = false;
    for (long i = digitalPinEvents.size() - 1; i >= 0; --i) {
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
    for (long i = 0; i < digitalPinEventObservers.size(); ++i) {
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
    Event::notify();
}

// We have a separate factory method because we don't want the
// notification to happen in the superclass with an incomplete object
DigitalPinEvent* DigitalPinEvent::newEvent(byte pin, bool value) {
    DigitalPinEvent* event = new DigitalPinEvent(pin, value);
    event->notify();
    return event;
}

bool DigitalPinEvent::read(byte pin) {
    for (long i = digitalPinEvents.size() - 1; i >= 0; --i) {
        DigitalPinEvent* each = digitalPinEvents.at(i);
        if (each->getPin() == pin) {
            return each->getValue();
        }
    }
    return false;
}

// ======================= AnalogPinEvent =======================

AnalogPinEvent::AnalogPinEvent(byte pin, int value) : Event() {
    _pin = pin;
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

void AnalogPinEvent::removeObserver(observerFunction_t function) {
    for (long i = analogPinEventObservers.size() - 1; i >= 0; --i) {
        if (analogPinEventObservers.at(i)->function == function) {
            delete analogPinEventObservers.removeAt(i);
        }
    }
}

void AnalogPinEvent::notify() {
    for (long i = 0; i < analogPinEventObservers.size(); ++i) {
        Observer* observer = analogPinEventObservers.at(i);
        observerFunction_t function = observer->function;
        (*function)((Event*) this);
    }
    Event::notify();
}

// We have a separate factory method because we don't want the
// notification to happen in the superclass with an incomplete object
AnalogPinEvent* AnalogPinEvent::newEvent(byte pin, int value) {
    AnalogPinEvent* event = new AnalogPinEvent(pin, value);
    event->notify();
    return event;
}
