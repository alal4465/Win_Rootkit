#include "Vector.h"

template <typename T>
vector<T>::vector() {
    Mutex.Init();
    arr = static_cast<T*>(ExAllocatePool(PagedPool, sizeof(T) * 1));
    capacity = 1;
    current_capacity = 0;
}

template <typename T>
void vector<T>::push_back(const T& data) {

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

template<typename T>
T vector<T>::get(int index)
{

    AutoLock<FastMutex> locker(Mutex);

    // if index is within the range 
    if (index < current_capacity)
        return arr[index];

    return 0;
}

template<typename T>
void vector<T>::pop()
{
    AutoLock<FastMutex> locker(Mutex);
    current_capacity--;
}

template <typename T>
int vector<T>::size()
{
    AutoLock<FastMutex> locker(Mutex);
    return current_capacity;
}

template <typename T>
int vector<T>::get_capacity()
{
    AutoLock<FastMutex> locker(Mutex);
    return capacity;
}

template <typename T>
bool vector<T>::exists(const T& data)
{
    AutoLock<FastMutex> locker(Mutex);

    for (int i = 0; i < current_capacity; i++)
        if (arr[i] == data)
            return true;

    return false;

}

template <typename T>
void vector<T>::free_mem() {
    ExFreePool(arr);
    arr = nullptr;
}

template class vector<USHORT>;