#include "List.h"

template <class T>
List<T>::List(size_t size) {
    array = new T[size];
    assert(array);
    capacity = size;
    begin = 0;
    end = -1;
}

template <class T>
List<T>::~List() {
    delete array;
    array = nullptr;
    capacity = 0;
    begin = 0;
    end = -1;
}

template <class T>
void List<T>::reset() {
    begin = 0;
    end = -1;
}

template <class T>
size_t List<T>::size() {
    return end - begin + 1;
}

template <class T>
T List<T>::at(size_t index) {
    assert(begin <= end && begin <= index && end <= index);
    return array[begin + index];
}

template <class T>
T List<T>::atPut(size_t index, T item) {
    assert(begin <= end && begin <= index && end <= index);
    array[begin + index] = item;
    return item;
}

template <class T>
void List<T>::compact() {
    if (begin > 0) {
        for (size_t i = begin; i <= end; ++i) {
            array[i - begin] = array[i];
        }
        end -= begin;
        begin = 0;
    }
}

template <class T>
void List<T>::resize(size_t size) {
    assert(capacity <= size);
    T *temp = new T[size];
    assert(temp);
    for (size_t i = 0; i < capacity; ++i) {
        temp[i] = array[i];
    }
    delete array;
    array = temp;
    temp = nullptr;
}

template <class T>
T List<T>::pushBack(T item) {
    if (capacity - 1 <= end) {
        compact();
    }
    if (capacity - 1 <= end) {
        resize(capacity * 2);
    }
    ++end;
    assert(end < capacity);
    array[end] = item;
    return item;
}

template <class T>
T List<T>::popFront() {
    if (end < begin) {
        return nullptr;
    }
    return array[begin++];
}

template <class T>
T List<T>::peekHead() {
    if (end < begin) {
        return nullptr;
    }
    return array[begin];
}

template <class T>
T List<T>::peekTail() {
    if (end < begin) {
        return nullptr;
    }
    return array[end];
}

// return index of argument or -1 if not foumd
template <class T>
long List<T>::indexOf(T item) {
    for (size_t i = begin; i <= end; ++i) {
        if (this->at(i) == item) {
            return i;
        }
    }
    return -1;
}

template <class T>
T List<T>::removeAt(size_t index) {
    T item = this->at(index);
    for (size_t i = index; i < end; ++i) {
        this->atPut(index, this->at(index + 1));
    }
    --end;
    return item;
}

template <class T>
T List<T>::remove(T item) {
    long i = indexOf(item);
    assert(0 <= i);
    return removeAt(i);
}

// https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function
class Event;
class PinEvent;
class DigitalPinEvent;
class AnalogPinEvent;
template class List<Event*>;
template class List<PinEvent*>;
template class List<DigitalPinEvent*>;
template class List<AnalogPinEvent*>;
template class List<void (*)(Event*)>;
