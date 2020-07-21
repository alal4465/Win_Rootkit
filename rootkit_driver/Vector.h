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
    vector()
        : capacity(1),current_capacity(0)
    {
        KdPrint(("Vector constructor called\n"));
        Mutex.Init();
        arr = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * 1));
    }

    ~vector() {
        KdPrint(("Vector deconstructor called\n"));
        ExFreePool(arr);
    }

    void* operator new(size_t size) {
        KdPrint(("Vector operator new\n"));
        return ExAllocatePool(PagedPool, size);
    }

    void operator delete(void* ptr) {
        KdPrint(("Vector operator delete\n"));
        ExFreePool(ptr);
    }

    void push_back(const T& data) {
        AutoLock<FastMutex> locker(Mutex);

        // if the number of elements is equal to the max capacity
        if (current_capacity == capacity) {
            T* temp = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * 2 * capacity));

            // copying old array elements to new array 
            for (int i = 0; i < capacity; i++) {
                temp[i] = arr[i];
            }

            // deleting previous array 
            ExFreePool(arr);
            capacity *= 2;
            arr = temp;
        }

        // Inserting data 
        arr[current_capacity] = data;
        current_capacity++;
    }

    T& operator[](int i) {
        AutoLock<FastMutex> locker(Mutex);
        if (i >= current_capacity)
            return NULL;

        return arr[i];
    }

    void pop()
    {
        AutoLock<FastMutex> locker(Mutex);
        current_capacity--;
    }

    int size()
    {
        AutoLock<FastMutex> locker(Mutex);
        return current_capacity;
    }

    bool exists(const T& data)
    {
        AutoLock<FastMutex> locker(Mutex);

        for (int i = 0; i < current_capacity; i++)
            if (arr[i] == data)
                return true;

        return false;

    }
};
