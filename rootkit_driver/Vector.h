#pragma once
#include <Ntifs.h>
#include "FastMutex.h"
#include "AutoLock.h"


template <typename T>
class vector {

    T* ptr;
    int capacity;
    int current_capacity;
    FastMutex mutex;

public:
    vector()
        : capacity(1),current_capacity(0)
    {
        KdPrint(("Vector constructor called\n"));
        mutex.Init();
        ptr = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * 1));
    }

    ~vector() {
        KdPrint(("Vector deconstructor called\n"));
        ExFreePool(ptr);
    }

    void push_back(const T& data) {
        AutoLock<FastMutex> locker(mutex);

        // if the number of elements is equal to the max capacity
        if (current_capacity == capacity) {
            T* temp = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * 2 * capacity));

            // copying old array elements to new array 
            for (int i = 0; i < capacity; i++) {
                temp[i] = ptr[i];
            }

            // deleting previous array 
            ExFreePool(ptr);
            capacity *= 2;
            ptr = temp;
        }

        // Inserting data 
        ptr[current_capacity] = data;
        current_capacity++;
    }

    void pop()
    {
        AutoLock<FastMutex> locker(mutex);
        current_capacity--;
    }

    int size()
    {
        AutoLock<FastMutex> locker(mutex);
        return current_capacity;
    }

    void* operator new(size_t size) {
        KdPrint(("Vector operator new\n"));
        return ExAllocatePool(PagedPool, size);
    }

    void operator delete(void* ptr) {
        KdPrint(("Vector operator delete\n"));
        ExFreePool(ptr);
    }

    T& operator[](int i) {
        AutoLock<FastMutex> locker(mutex);
        return ptr[i];
    }

    T* begin() {
        return ptr;
    }

    T* end() {
        return ptr + current_capacity;
    }

    bool contains(const T& data)
    {
        AutoLock<FastMutex> locker(mutex);

        for (const auto& item : *this)
            if (item == data)
                return true;

        return false;

    }

    vector<T>& operator=(vector<T>& rhs) {
        capacity = rhs.capacity;
        current_capacity = rhs.current_capacity;

        ExFreePool(ptr);

        ptr = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * capacity));

        for (int i = 0; i < rhs.current_capacity; i++)
            ptr[i] = rhs[i];
        return *this;
    }

    vector<T>& operator=(vector<T>&& rhs) {
        capacity = rhs.current_capacity;
        current_capacity = rhs.current_capacity;

        ExFreePool(ptr);

        ptr = rhs.ptr;

        rhs.ptr = nullptr;
        rhs.capacity = 0;
        rhs.current_capacity = 0;

        return *this;
    }

    vector(vector<T>& rhs) {
        capacity = rhs.capacity;
        current_capacity = rhs.current_capacity;
        ptr = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * capacity));

        for (int i = 0; i < rhs.current_capacity; i++)
            ptr[i] = rhs[i];
    }

    vector(vector<T>&& rhs) {
        capacity = rhs.capacity;
        current_capacity = rhs.current_capacity;
        ptr = rhs.ptr;

        rhs.ptr = nullptr;
        rhs.capacity = 0;
        rhs.current_capacity = 0;
    }
};
