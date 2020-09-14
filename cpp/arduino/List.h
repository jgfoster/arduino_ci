#pragma once
#include <assert.h>
typedef unsigned long size_t;

template <class T>
class List {
    private:
        T *array;
        size_t capacity;
        size_t begin;
        size_t end;
        void compact();
        void resize(size_t size);
    public:
        List(size_t size = 4);
        ~List();
        void reset();
        size_t size();
        bool isEmpty();
        T at(size_t index);
        T atPut(size_t index, T item);
        T pushBack(T item);
        T popFront();
        T peekHead();
        T peekTail();
        long indexOf(T item);
        T removeAt(size_t index);
        T remove(T item);
};
