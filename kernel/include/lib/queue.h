#ifndef QUEUE_H
#define QUEUE_H

#include "../lib.h"

template<typename T>
struct Node {
    T data;
    Node* next;
    Node(const T& data) : data(data), next(nullptr) {}
};

template<typename T>
class queue {
    public:
        queue();
        ~queue();

        auto push(const T& item) -> void;
        auto pop() -> void;
        auto front() -> T&;
        auto front() const -> const T&;
        auto back() -> T&;
        auto back() const -> const T&;
        auto empty() const -> bool;
        auto size() const -> size_t;

        auto swap(queue<T>& other) -> void;

    private:
        Node<T>* head;
        Node<T>* tail;
        size_t count;
};

template<typename T>
queue<T>::queue() : head(nullptr), tail(nullptr), count(0) {}

template<typename T>
queue<T>::~queue() {
    while (!empty()) {
        pop();
    }
}

template<typename T>
auto queue<T>::push(const T& item) -> void {
    Node<T>* newNode = new Node<T>(item);
    if (empty()) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    ++count;
}

template<typename T>
auto queue<T>::pop() -> void {
    if (empty()) {
        return;
    }

    Node<T>* temp = head;
    head = head->next;
    delete temp;
    --count;

    if (empty()) {
        tail = nullptr;
    }
}

template<typename T>
auto queue<T>::front() -> T& {
    return head->data;
}

template<typename T>
auto queue<T>::front() const -> const T& {
    return head->data;
}

template<typename T>
auto queue<T>::back() -> T& {
    return tail->data;
}

template<typename T>
auto queue<T>::back() const -> const T& {
    return tail->data;
}

template<typename T>
auto queue<T>::empty() const -> bool {
    return count == 0;
}

template<typename T>
auto queue<T>::size() const -> size_t {
    return count;
}

template<typename T>
auto queue<T>::swap(queue<T>& other) -> void {
    Node<T>* tempHead = other.head;
    Node<T>* tempTail = other.tail;
    size_t tempCount = other.count;

    other.head = head;
    other.tail = tail;
    other.count = count;

    head = tempHead;
    tail = tempTail;
    count = tempCount;
}

#endif