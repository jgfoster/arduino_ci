#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <Event.h>

List<Event*> events;
List<Event*> digitalPinEvents;
List<Event*> analogPinEvents;

void eventObserver(Event* event) {
    events.pushBack(event);
}

void digitalPinEventObserver(Event* event) {
    digitalPinEvents.pushBack(event);
}

void analogPinEventObserver(Event* event) {
    analogPinEvents.pushBack((Event*) event);
}

void resetAll() {
    pinMode(LED_BUILTIN, OUTPUT);
    Event::reset();
    DigitalPinEvent::reset();
    AnalogPinEvent::reset();
    events.reset();
    digitalPinEvents.reset();
    analogPinEvents.reset();
}

unittest(event_reset) {
    resetAll();
    assertEqual(0, events.size());
    assertEqual(0, digitalPinEvents.size());
    assertEqual(0, analogPinEvents.size());
}

unittest(event_read) {
    resetAll();
    assertFalse(DigitalPinEvent::read(LED_BUILTIN));
    digitalWrite(LED_BUILTIN, HIGH);
    assertTrue(DigitalPinEvent::read(LED_BUILTIN));
}

unittest(event_digitalPinEvent) {
    resetAll();
    DigitalPinEvent::addObserver(digitalPinEventObserver);
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(2, digitalPinEvents.size());
    DigitalPinEvent* high = (DigitalPinEvent*) digitalPinEvents.peekHead();
    DigitalPinEvent* low  = (DigitalPinEvent*) digitalPinEvents.peekTail();
    assertNotNull(high);
    assertNotNull(low);
    assertNotEqual(high, low);
    assertEqual(1, high->getId());
    assertEqual(2, low->getId());
    assertEqual(0, high->getTime());
    assertEqual(1, low->getTime());
    assertEqual(LED_BUILTIN, high->getPin());
    assertEqual(LED_BUILTIN, low->getPin());
    assertTrue(high->getValue());
    assertFalse(low->getValue());
}

unittest(event_addObserver) {
    resetAll();
    Event::addObserver(eventObserver);
    DigitalPinEvent::addObserver(digitalPinEventObserver);
    AnalogPinEvent::addObserver(analogPinEventObserver);
    digitalWrite(LED_BUILTIN, HIGH);
    assertEqual(1, events.size());
    assertEqual(1, digitalPinEvents.size());
    assertEqual(0, analogPinEvents.size());
    assertEqual(events.peekTail(), digitalPinEvents.peekTail());
}

unittest(event_removeObserver) {
    resetAll();
    Event::addObserver(eventObserver);
    DigitalPinEvent::addObserver(digitalPinEventObserver);
    digitalWrite(LED_BUILTIN, HIGH);
    Event::removeObserver(eventObserver);
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(1, events.size());
    assertEqual(2, digitalPinEvents.size());
}

unittest(event_type) {
    resetAll();
    Event::addObserver(eventObserver);
    digitalWrite(LED_BUILTIN, HIGH);
    Event* event = events.peekTail();
    assertTrue(event->isDigitalPinEvent());
    assertFalse(event->isAnalogPinEvent());
}

unittest(event_observeOnePin) {
    resetAll();
    Event::addObserver(eventObserver);
    DigitalPinEvent::addObserver(digitalPinEventObserver, 0, PIN_WRITTEN);
    digitalWrite(LED_BUILTIN, HIGH);
    assertEqual(1, events.size());
    assertEqual(0, digitalPinEvents.size());
    digitalWrite(0, HIGH);
    assertEqual(2, events.size());
    assertEqual(1, digitalPinEvents.size());
}

unittest(event_observePinChanging) {
    resetAll();
    Event::addObserver(eventObserver);
    DigitalPinEvent::addObserver(digitalPinEventObserver, ALL_PINS, PIN_CHANGING);
    digitalWrite(LED_BUILTIN, HIGH);
    assertEqual(1, events.size());
    assertEqual(1, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, HIGH);
    assertEqual(2, events.size());
    assertEqual(1, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(3, events.size());
    assertEqual(2, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(4, events.size());
    assertEqual(2, digitalPinEvents.size());
}

unittest(event_observePinRising) {
    resetAll();
    Event::addObserver(eventObserver);
    DigitalPinEvent::addObserver(digitalPinEventObserver, ALL_PINS, PIN_RISING);
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(1, events.size());
    assertEqual(0, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(2, events.size());
    assertEqual(0, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, HIGH);
    assertEqual(3, events.size());
    assertEqual(1, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, HIGH);
    assertEqual(4, events.size());
    assertEqual(1, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(5, events.size());
    assertEqual(1, digitalPinEvents.size());
}

unittest(event_observePinFalling) {
    resetAll();
    Event::addObserver(eventObserver);
    DigitalPinEvent::addObserver(digitalPinEventObserver, ALL_PINS, PIN_FALLING);
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(1, events.size());
    assertEqual(1, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(2, events.size());
    assertEqual(1, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, HIGH);
    assertEqual(3, events.size());
    assertEqual(1, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, HIGH);
    assertEqual(4, events.size());
    assertEqual(1, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(5, events.size());
    assertEqual(2, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, LOW);
    assertEqual(6, events.size());
    assertEqual(2, digitalPinEvents.size());
    digitalWrite(LED_BUILTIN, HIGH);
    assertEqual(7, events.size());
    assertEqual(2, digitalPinEvents.size());
}

unittest_main()
