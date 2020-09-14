#pragma once
#include <assert.h>
#include <stdexcept>
// #include <iostream>

template <class T>
class List {
    private:
        T *array;
        // Use of `long` rather than `unsigned long` so -1 is available
        long capacity;
        long begin;
        long end;
        void compact();
        void resize(long size);
    public:
        List(long size = 4);
        ~List();
        void reset();
        long size();
        bool isEmpty();
        T at(long index);
        T atPut(long index, T item);
        T pushBack(T item);
        T popFront();
        T peekHead();
        T peekTail();
        long indexOf(T item);
        T removeAt(long index);
        T remove(T item);
};

// Implementation doesn't work in its own .cpp file!
// https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function

template <class T>
List<T>::List(long size) {
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
long List<T>::size() {
    return end - begin + 1;
}

template <class T>
bool List<T>::isEmpty() {
    return end < begin;
}

template <class T>
T List<T>::at(long index) {
    if (!((begin <= end) && (0 <= index) && (index < size()))) {
        throw std::runtime_error("Index out of range!");
    }
    return array[begin + index];
}

template <class T>
T List<T>::atPut(long index, T item) {
    if (!((begin <= end) && (0 <= index) && (index < size()))) {
        throw std::runtime_error("Index out of range!");
    }
    array[begin + index] = item;
    return item;
}

template <class T>
void List<T>::compact() {
    if (begin > 0) {
        for (long i = begin; i <= end; ++i) {
            array[i - begin] = array[i];
        }
        end -= begin;
        begin = 0;
    }
}

template <class T>
void List<T>::resize(long size) {
    if (!(capacity <= size)) {
        throw std::logic_error("Size is less than capacity!");
    }
    T *temp = new T[size];
    assert(temp);
    for (long i = 0; i < capacity; ++i) {
        temp[i] = array[i];
    }
    delete array;
    array = temp;
    temp = nullptr;
    capacity = size;
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
    if (!(end < capacity)) {
        throw std::logic_error("No room!");
    }
    array[end] = item;
    return item;
}

template <class T>
T List<T>::popFront() {
    if (end < begin) {
        throw std::runtime_error("List is empty!");
    }
    return array[begin++];
}

template <class T>
T List<T>::peekHead() {
    if (end < begin) {
        throw std::runtime_error("List is empty!");
    }
    return array[begin];
}

template <class T>
T List<T>::peekTail() {
    if (end < begin) {
        throw std::runtime_error("List is empty!");
    }
    return array[end];
}

// return index of argument or -1 if not foumd
template <class T>
long List<T>::indexOf(T item) {
    for (long i = begin; i <= end; ++i) {
        if (at(i) == item) {
            return i;
        }
    }
    return -1;
}

template <class T>
T List<T>::removeAt(long index) {
    T item = at(index);
    for (long i = index; i < end; ++i) {
        atPut(i, at(i + 1));
    }
    --end;
    return item;
}

template <class T>
T List<T>::remove(T item) {
    long i = indexOf(item);
    if (!(0 <= i)) {
        throw std::runtime_error("Index out of range!");
    }
    return removeAt(i);
}
