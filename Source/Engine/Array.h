#pragma once
#include "Types.h"
#include <assert.h>
#include <new>
#include <string.h>

template <typename T>
class Array {
private:
    T* m_data = nullptr;
    uint32 m_size = 0;
    uint32 m_capacity = 0;

    void Copy(const Array<T> & copy);
    void AdjustSize(uint32 newSize);
    void AdjustCapacity();

public:
    Array() { }
    Array(uint32 capacity) : m_capacity(capacity) { AdjustCapacity(); }
    Array(const Array<T> & copy);
    ~Array();

    Array<T> & operator=(const Array<T> & rhs);
    T& operator[](uint32 index);
    const T& operator[](uint32 index) const;

    T* begin() const { return m_data; }
    T* end() const { return m_data + m_size; }

    uint32 Size() const { return m_size; }
    bool Empty() const { return m_size == 0; }
    uint32 Capacity() const { return m_capacity; }

    T& Back();
    void Clear();
    void Push(const T& object);
    void Push(T&& object);
    T Pop();
    void Delete(uint32 index);
};


template<typename T>
void Array<T>::Copy(const Array<T> & copy)
{
    m_capacity = copy.m_capacity;
    m_data = (T*)operator new(m_capacity * sizeof(T));
    m_size = copy.m_size;

    memcpy(m_data, copy.m_data, copy.m_size * sizeof(T));
}

template<typename T>
void Array<T>::AdjustSize(uint32 newSize)
{ 
    if (newSize > m_capacity) {
        m_capacity = m_capacity * 2 + 1;
        AdjustCapacity();
    }

    m_size = newSize;
}

template<typename T>
void Array<T>::AdjustCapacity()
{

    T* newData = (T*)operator new(m_capacity * sizeof(T));

    if (m_data) {
        memcpy(newData, m_data, m_size * sizeof(T));
        operator delete(m_data);
    }

    m_data = newData;

}

template<typename T>
Array<T>::Array(const Array<T> & copy)
{
    Copy(copy);
}

template<typename T>
Array<T>::~Array()
{
    Clear();
}

template<typename T>
Array<T> & Array<T>::operator=(const Array<T> & rhs)
{
    operator delete(m_data);
    Copy(rhs);

    return *this;
}

template<typename T>
T& Array<T>::operator[](uint32 index)
{
    assert(index >= 0 && index < m_size);
    return m_data[index];
}

template<typename T>
const T& Array<T>::operator[](uint32 index) const
{
    assert(index >= 0 && index < m_size);
    return m_data[index];
}

template<typename T>
T& Array<T>::Back()
{
    return m_data[m_size - 1];
}

template<typename T>
void Array<T>::Clear()
{
    for (uint32 index = 0; index < m_size; ++index) {
        (m_data + index)->~T();
    }

    operator delete(m_data);
    m_data = nullptr;
    m_size = 0;
    m_capacity = 0;
}

template<typename T>
void Array<T>::Push(const T& object)
{
    AdjustSize(m_size + 1);
    new(m_data + m_size - 1) T(object);
}

template<typename T>
void Array<T>::Push(T&& object)
{
    AdjustSize(m_size + 1);
    new(m_data + m_size - 1) T(object);
}

template<typename T>
T Array<T>::Pop()
{
    T value(m_data[m_size - 1]);
    (m_data + m_size - 1)->~T();
    AdjustSize(m_size - 1);

    return value;
}

template<typename T>
void Array<T>::Delete(uint32 index)
{
    assert(index >= 0 && index < m_size);
    (m_data + index)->~T();
    memcpy(m_data + index, m_data + m_size - 1, sizeof(T));
    AdjustSize(m_size - 1);
}