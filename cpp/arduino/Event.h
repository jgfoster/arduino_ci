#pragma once
#include "List.h"

typedef unsigned char byte;
typedef unsigned long size_t;
class Event;
typedef void (*observer_t)(Event* event);

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
        static void addObserver(observer_t observer) { eventObservers.pushBack(observer); }
        static void removeObserver(observer_t observer) { eventObservers.remove(observer); }
    private:
        size_t _id;
        size_t _time;
        static size_t _lastId;
        static List<Event*> events; 
        static List<observer_t> eventObservers;
};

class PinEvent : public Event {
    public:
        PinEvent(byte pin);
        ~PinEvent();
        virtual void notify();
        virtual bool isPinEvent() { return true; }
        int getPin() { return _pin; }
        static void eraseHistory() { pinEvents.reset(); };
    protected:
    private:
        byte _pin;
        static List<PinEvent*> pinEvents;
        static List<observer_t> pinEventObservers;
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
    private:
        bool _value;
        static List<DigitalPinEvent*> digitalPinEvents;
        static List<observer_t> digitalPinEventObservers;
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
    private:
        int _value;
        static List<AnalogPinEvent*> analogPinEvents;
        static List<observer_t> analogPinEventObservers;
};
