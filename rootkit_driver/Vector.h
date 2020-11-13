#pragma once
#include <Ntifs.h>
#include "FastMutex.h"
#include "AutoLock.h"


template <typename T>
class vector {

    T* ptr_;
    int capacity_;
    int current_capacity_;
    FastMutex mutex_;

public:
    vector():
    capacity_(1),
    current_capacity_(0),
    ptr_(static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * 1)))
    {
        mutex_.Init();
    }

    ~vector() {
        ExFreePool(ptr_);
    }

    void push_back(const T& data) {
        AutoLock<FastMutex> locker(mutex_);

        // if the number of elements is equal to the max capacity
        if (current_capacity_ == capacity_) {
            T* temp = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * 2 * capacity_));

            // copying old array elements to new array 
            for (int i = 0; i < capacity_; i++) {
                temp[i] = ptr_[i];
            }

            // deleting previous array 
            ExFreePool(ptr_);
            capacity_ *= 2;
            ptr_ = temp;
        }

        // Inserting data 
        ptr_[current_capacity_] = data;
        current_capacity_++;
    }

    void pop()
    {
        AutoLock<FastMutex> locker(mutex_);
        current_capacity_--;
    }

    int size()
    {
        AutoLock<FastMutex> locker(mutex_);
        return current_capacity_;
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
        AutoLock<FastMutex> locker(mutex_);
        return ptr_[i];
    }

    T* begin() {
        AutoLock<FastMutex> locker(mutex_);
        return ptr_;
    }

    T* end() {
        AutoLock<FastMutex> locker(mutex_);
        return ptr_ + current_capacity_;
    }

    bool contains(const T& data)
    {
        AutoLock<FastMutex> locker(mutex_);

        for (const auto& item : *this)
            if (item == data)
                return true;

        return false;

    }

    vector<T>& operator=(vector<T>& rhs) {
        capacity_ = rhs.capacity_;
        current_capacity_ = rhs.current_capacity_;

        ExFreePool(ptr_);

        ptr_ = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * capacity_));

        for (int i = 0; i < rhs.current_capacity_; i++)
            ptr_[i] = rhs[i];
        return *this;
    }

    vector<T>& operator=(vector<T>&& rhs) {
        capacity_ = rhs.current_capacity_;
        current_capacity_ = rhs.current_capacity_;

        ExFreePool(ptr_);

        ptr_ = rhs.ptr_;

        rhs.ptr_ = nullptr;
        rhs.capacity_ = 0;
        rhs.current_capacity_ = 0;

        return *this;
    }

    vector(vector<T>& rhs) {
        capacity_ = rhs.capacity_;
        current_capacity_ = rhs.current_capacity_;
        ptr_ = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * capacity_));

        for (int i = 0; i < rhs.current_capacity_; i++)
            ptr_[i] = rhs[i];
    }

    vector(vector<T>&& rhs) {
        capacity_ = rhs.capacity_;
        current_capacity_ = rhs.current_capacity_;
        ptr_ = rhs.ptr_;

        rhs.ptr_ = nullptr;
        rhs.capacity_ = 0;
        rhs.current_capacity_ = 0;
    }
};
