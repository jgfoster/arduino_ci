#pragma once
#include "List.h"

typedef unsigned char byte;
class Event;
typedef void (*observerFunction_t)(Event* event);
// this means that we only support pins 0-254!
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
        ~Event();
        long getId() { return _id; }
        long getTime() { return _time; }
        virtual void notify();
        virtual bool isDigitalPinEvent() { return false; }
        virtual bool isAnalogPinEvent() { return false; }
        static void reset() { events.reset(); eventObservers.reset(); _lastId = 0; };
        static void addObserver(observerFunction_t observer);
        static void removeObserver(observerFunction_t observer);
    protected: 
        Event();
    private:
        long _id;
        long _time;
        static long _lastId;
        static List<Event*> events; 
        static List<Observer*> eventObservers;
};

class DigitalPinEvent : public Event {
    public:
        ~DigitalPinEvent();
        virtual void notify();
        virtual bool isDigitalPinEvent() { return true; }
        byte getPin() { return _pin; }
        bool getValue() { return _value; };
        static DigitalPinEvent* newEvent(byte pin, bool value);
        static bool read(byte pin);
        static void reset() { digitalPinEvents.reset(); digitalPinEventObservers.reset(); };
        static void addObserver(observerFunction_t observer, byte pin = ALL_PINS, pinEventType_t eventType = PIN_WRITTEN);
        static void removeObserver(observerFunction_t observer);
    protected: 
        DigitalPinEvent(byte pin, bool value);
    private:
        byte _pin;
        bool _value;
        DigitalPinEvent* getPriorEventForPin();
        static List<DigitalPinEvent*> digitalPinEvents;
        static List<Observer*> digitalPinEventObservers;
};

class AnalogPinEvent : public Event {
    public:
        ~AnalogPinEvent();
        virtual void notify();
        virtual bool isAnalogPinEvent() { return false; }
        byte getPin() { return _pin; }
        int getValue() { return _value; }
        static AnalogPinEvent* newEvent(byte pin, int value);
        static int read(byte pin);
        static void reset() { analogPinEvents.reset(); analogPinEventObservers.reset(); };
        static void addObserver(observerFunction_t observer);
        static void removeObserver(observerFunction_t observer);
    protected: 
        AnalogPinEvent(byte pin, int value);
    private:
        byte _pin;
        int _value;
        static List<AnalogPinEvent*> analogPinEvents;
        static List<Observer*> analogPinEventObservers;
};
