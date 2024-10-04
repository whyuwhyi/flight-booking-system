#pragma once

#include <LinkedList/Link.h>

template<typename T>
class LinkedList {
private:
    Link<T>* head;
public:
    LinkedList();
    LinkedList(const LinkedList<T>& other);               // 拷贝构造函数
    LinkedList<T>& operator=(const LinkedList<T>& other); // 拷贝赋值运算符
    ~LinkedList();

    Link<T>* getHead() const;
    void setHead(Link<T>* head);
    void append(const T& element);
    void destroy();

    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const LinkedList<U>& list);

    template<typename U>
    friend std::istream& operator>>(std::istream& in, LinkedList<U>& list);
};

template<typename T>
LinkedList<T>::LinkedList() : head(nullptr) { }

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head(nullptr) {
    Link<T>* current = other.head;
    Link<T>* tail = nullptr;
    while (current != nullptr) {
        Link<T>* newLink = new Link<T>(current->getElement());
        if (tail == nullptr) {
            head = newLink;
        } else {
            tail->setNext(newLink);
        }
        tail = newLink;
        current = current->getNext();
    }
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
    if (this != &other) {
        destroy();
        Link<T>* current = other.head;
        Link<T>* tail = nullptr;
        while (current != nullptr) {
            Link<T>* newLink = new Link<T>(current->getElement());
            if (tail == nullptr) {
                head = newLink;
            } else {
                tail->setNext(newLink);
            }
            tail = newLink;
            current = current->getNext();
        }
    }
    return *this;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    destroy();
}

template<typename T>
Link<T>* LinkedList<T>::getHead() const {
    return head;
}

template<typename T>
void LinkedList<T>::setHead(Link<T>* head) {
    this->head = head;
}

template<typename T>
void LinkedList<T>::append(const T& element) {
    Link<T>* newLink = new Link<T>(element);
    if (head == nullptr) {
        head = newLink;
    } else {
        Link<T>* pointer = head;
        while (pointer->getNext() != nullptr) {
            pointer = pointer->getNext();
        }
        pointer->setNext(newLink);
    }
}

template<typename T>
void LinkedList<T>::destroy() {
    Link<T>* node;
    while (head != nullptr) {
        node = head;
        head = head->getNext();
        delete node;
    }
    head = nullptr;
}

template<typename U>
std::ostream& operator<<(std::ostream& out, const LinkedList<U>& list) {
    Link<U>* current = list.getHead();
    while (current != nullptr) {
        out << *current;
        current = current->getNext();
    }
    return out;
}

template<typename U>
std::istream& operator>>(std::istream& in, LinkedList<U>& list) {
    U element;
    Link<U>* tail = nullptr;
    while (in >> element) {
        Link<U>* newLink = new Link<U>(element);
        if (tail == nullptr) {
            list.setHead(newLink);
        } else {
            tail->setNext(newLink);
        }
        tail = newLink;
    }
    return in;
}