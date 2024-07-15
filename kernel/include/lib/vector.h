#ifndef VECTOR_H
#define VECTOR_H

#include "../lib.h"

template<typename T>
class vector {
public:
    vector();
    ~vector();
    void push_back(const T& value);
    void pop_back();
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    size_t size() const;
    bool empty() const;

private:
    void resize(size_t new_capacity);
    T* data;
    size_t count;
    size_t capacity;
};

template<typename T>
vector<T>::vector() : data(nullptr), count(0), capacity(0) {}

template<typename T>
vector<T>::~vector() {
    delete[] data;
}

template<typename T>
void vector<T>::push_back(const T& value) {
    if (count == capacity) {
        resize(capacity == 0 ? 1 : capacity * 2);
    }
    data[count++] = value;
}

template<typename T>
void vector<T>::pop_back() {
    if (!empty()) {
        --count;
    }
}

template<typename T>
T& vector<T>::operator[](size_t index) {
    if (index < count) {
        return data[index];
    }
    return *static_cast<T*>(nullptr);
}

template<typename T>
const T& vector<T>::operator[](size_t index) const {
    if (index < count) {
        return data[index];
    }
    return *static_cast<T*>(nullptr);
}

template<typename T>
size_t vector<T>::size() const {
    return count;
}

template<typename T>
bool vector<T>::empty() const {
    return count == 0;
}

template<typename T>
void vector<T>::resize(size_t new_capacity) {
    T* new_data = new T[new_capacity];
    for (size_t i = 0; i < count; ++i) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    capacity = new_capacity;
}

#endif