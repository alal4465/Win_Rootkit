#pragma once
#include <Ntifs.h>
#include "FastMutex.h"
#include "AutoLock.h"

template <typename T>
class vector {

    T* arr;
    int capacity;
    int current_capacity;
    FastMutex Mutex;

public:
    vector();

    void push_back(const T& data);

    T get(int index);

    void pop();

    int size();

    int get_capacity();

    bool exists(const T& data);

    void free_mem();
};