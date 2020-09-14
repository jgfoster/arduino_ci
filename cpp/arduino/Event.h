#pragma once
#include "List.h"

typedef unsigned char byte;
typedef unsigned long size_t;
class Event;
typedef void (*observerFunction_t)(Event* event);
// this means that we only support 254 pins!
#define ALL_PINS 255
enum pinEventType_t {
    PIN_WRITTEN,    // even if same value
    PIN_CHANGING,   // only if value changes
    PIN_RISING,     // low to high
    PIN_FALLING     // high to low
};
class Observer {
    public: 
        Observer(observerFunction_t function) { this->function = function; }
        observerFunction_t function;
        union {
            struct {
                byte pin;
                pinEventType_t eventType;
            } digitalPin;
        } details;
};

class Event {
    public:
        Event();
        ~Event();
        size_t getId() { return _id; }
        size_t getTime() { return _time; }
        virtual void notify();
        virtual bool isPinEvent() { return false; }
        virtual bool isDigitalPinEvent() { return false; }
        virtual bool isAnalogPinEvent() { return false; }
        static void eraseHistory() { events.reset(); };
        static void addObserver(observerFunction_t observer);
        static void removeObserver(observerFunction_t observer);
    private:
        size_t _id;
        size_t _time;
        static size_t _lastId;
        static List<Event*> events; 
        static List<Observer*> eventObservers;
};

class PinEvent : public Event {
    public:
        PinEvent(byte pin);
        ~PinEvent();
        virtual void notify();
        virtual bool isPinEvent() { return true; }
        int getPin() { return _pin; }
        static void eraseHistory() { pinEvents.reset(); };
        static void addObserver(observerFunction_t observer);
        static void removeObserver(observerFunction_t observer);
    protected:
        byte _pin;
    private:
        static List<PinEvent*> pinEvents;
        static List<Observer*> pinEventObservers;
};

class DigitalPinEvent : public PinEvent {
    public:
        DigitalPinEvent(byte pin, bool value);
        ~DigitalPinEvent();
        virtual void notify();
        virtual bool isDigitalPinEvent() { return true; }
        bool getValue() { return _value; };
        static DigitalPinEvent* newEvent(byte pin, bool value);
        static void eraseHistory() { digitalPinEvents.reset(); };
        static void addObserver(observerFunction_t observer, byte pin = ALL_PINS, pinEventType_t eventType = PIN_WRITTEN);
        static void removeObserver(observerFunction_t observer);
    private:
        bool _value;
        DigitalPinEvent* getPriorEventForPin();
        static List<DigitalPinEvent*> digitalPinEvents;
        static List<Observer*> digitalPinEventObservers;
};

class AnalogPinEvent : public PinEvent {
    public:
        AnalogPinEvent(byte pin, int value);
        ~AnalogPinEvent();
        virtual void notify();
        virtual bool isAnalogPinEvent() { return false; }
        int getValue() { return _value; }
        static AnalogPinEvent* newEvent(byte pin, bool value);
        static void eraseHistory() { analogPinEvents.reset(); };
        static void addObserver(observerFunction_t observer);
        static void removeObserver(observerFunction_t observer);
    private:
        int _value;
        static List<AnalogPinEvent*> analogPinEvents;
        static List<Observer*> analogPinEventObservers;
};
