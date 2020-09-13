#include "Event.h"

extern unsigned long micros();

unsigned long Event::_lastId;
List<Event*> Event::events; 
List<observer_t> Event::eventObservers;
List<PinEvent*> PinEvent::pinEvents; 
List<observer_t> PinEvent::pinEventObservers;
List<DigitalPinEvent*> DigitalPinEvent::digitalPinEvents; 
List<observer_t> DigitalPinEvent::digitalPinEventObservers;
List<AnalogPinEvent*> AnalogPinEvent::analogPinEvents; 
List<observer_t> AnalogPinEvent::analogPinEventObservers;

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

void Event::notify() {
    for (size_t i = 0; i < eventObservers.size(); ++i) {
        observer_t function = eventObservers.at(i);
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

void PinEvent::notify() {
    for (size_t i = 0; i < pinEventObservers.size(); ++i) {
        observer_t function = pinEventObservers.at(i);
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

void DigitalPinEvent::notify() {
    for (size_t i = 0; i < digitalPinEventObservers.size(); ++i) {
        observer_t function = digitalPinEventObservers.at(i);
        (*function)((Event*) this);
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

void AnalogPinEvent::notify() {
    for (size_t i = 0; i < analogPinEventObservers.size(); ++i) {
        observer_t function = analogPinEventObservers.at(i);
        (*function)((Event*) this);
    }
    PinEvent::notify();
}

AnalogPinEvent* AnalogPinEvent::newEvent(byte pin, bool value) {
    AnalogPinEvent* event = new AnalogPinEvent(pin, value);
    event->notify();
    return event;
}
