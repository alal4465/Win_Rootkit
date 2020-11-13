#pragma once
#include <Ntifs.h>
#include "FastMutex.h"
#include "AutoLock.h"


template <typename T>
class vector {

    T* m_ptr;
    int m_capacity;
    int m_current_capacity;
    FastMutex m_mutex;

public:
    vector():
    m_capacity(1),
    m_current_capacity(0),
    m_ptr(static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * 1)))
    {
        m_mutex.Init();
    }

    ~vector() {
        ExFreePool(m_ptr);
    }

    void push_back(const T& data) {
        AutoLock<FastMutex> locker(m_mutex);

        // if the number of elements is equal to the max capacity
        if (m_current_capacity == m_capacity) {
            T* temp = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * 2 * m_capacity));

            // copying old array elements to new array 
            for (int i = 0; i < m_capacity; i++) {
                temp[i] = m_ptr[i];
            }

            // deleting previous array 
            ExFreePool(m_ptr);
            m_capacity *= 2;
            m_ptr = temp;
        }

        // Inserting data 
        m_ptr[m_current_capacity] = data;
        m_current_capacity++;
    }

    void pop()
    {
        AutoLock<FastMutex> locker(m_mutex);
        m_current_capacity--;
    }

    int size()
    {
        AutoLock<FastMutex> locker(m_mutex);
        return m_current_capacity;
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
        AutoLock<FastMutex> locker(m_mutex);
        return m_ptr[i];
    }

    T* begin() {
        AutoLock<FastMutex> locker(m_mutex);
        return m_ptr;
    }

    T* end() {
        AutoLock<FastMutex> locker(m_mutex);
        return m_ptr + m_current_capacity;
    }

    bool contains(const T& data)
    {
        AutoLock<FastMutex> locker(m_mutex);

        for (const auto& item : *this)
            if (item == data)
                return true;

        return false;

    }

    vector<T>& operator=(vector<T>& rhs) {
        m_capacity = rhs.m_capacity;
        m_current_capacity = rhs.m_current_capacity;

        ExFreePool(m_ptr);

        m_ptr = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * m_capacity));

        for (int i = 0; i < rhs.m_current_capacity; i++)
            m_ptr[i] = rhs[i];
        return *this;
    }

    vector<T>& operator=(vector<T>&& rhs) {
        m_capacity = rhs.m_current_capacity;
        m_current_capacity = rhs.m_current_capacity;

        ExFreePool(m_ptr);

        m_ptr = rhs.m_ptr;

        rhs.m_ptr = nullptr;
        rhs.m_capacity = 0;
        rhs.m_current_capacity = 0;

        return *this;
    }

    vector(vector<T>& rhs) {
        m_capacity = rhs.m_capacity;
        m_current_capacity = rhs.m_current_capacity;
        m_ptr = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * m_capacity));

        for (int i = 0; i < rhs.m_current_capacity; i++)
            m_ptr[i] = rhs[i];
    }

    vector(vector<T>&& rhs) {
        m_capacity = rhs.m_capacity;
        m_current_capacity = rhs.m_current_capacity;
        m_ptr = rhs.m_ptr;

        rhs.m_ptr = nullptr;
        rhs.m_capacity = 0;
        rhs.m_current_capacity = 0;
    }
};
