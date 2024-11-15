#pragma once

#include <LinkedList/Link.h>
#include <functional>
#include <iostream>
#include <stdexcept>

template<typename T>
class LinkedList {
private:
    Link<T>* head;
    Link<T>* tail;
    int nodeCount;

    void destroy();

    void quickSort(Link<T>* low, Link<T>* high, std::function<bool(const T&, const T&)> compare);
    Link<T>* partition(Link<T>* low, Link<T>* high, std::function<bool(const T&, const T&)> compare);

public:
    LinkedList();
    LinkedList(const LinkedList<T>& other);
    LinkedList(LinkedList<T>&& other) noexcept;
    LinkedList<T>& operator=(const LinkedList<T>& other);
    LinkedList<T>& operator=(LinkedList<T>&& other) noexcept;

    ~LinkedList();

    int size() const;
    Link<T>* getHead() const;
    void setHead(Link<T>* head);
    void append(const T& element);
    void clear();

    bool isEmpty() const;
    const T& getElementAt(int index) const;
    void setElementAt(int index, const T& element);
    void insert(int index, const T& element);
    bool remove(const T& element);
    void popBack();
    void popFront();
    Link<T>* find(const T& element) const;
    Link<T>* getLast() const;
    T removeLast();
    void traverse(std::function<void(const T&)> func) const;
    void traverse(std::function<void(T*)> func) const;

    void sort(std::function<bool(const T&, const T&)> compare);

    

    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const LinkedList<U>& list);

    template<typename U>
    friend std::istream& operator>>(std::istream& in, LinkedList<U>& list);
};


template<typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), nodeCount(0) { }

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head(nullptr), tail(nullptr), nodeCount(0) {
    Link<T>* current = other.head;
    while (current != nullptr) {
        append(current->getElement());
        current = current->getNext();
    }
}

template<typename T>
LinkedList<T>::LinkedList(LinkedList<T>&& other) noexcept
    : head(other.head), tail(other.tail), nodeCount(other.nodeCount) {
    other.head = nullptr;
    other.tail = nullptr;
    other.nodeCount = 0;
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
    if (this != &other) {
        destroy();
        Link<T>* current = other.head;
        while (current != nullptr) {
            append(current->getElement());
            current = current->getNext();
        }
    }
    return *this;
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& other) noexcept {
    if (this != &other) {
        destroy();
        
        head = other.head;
        tail = other.tail;
        nodeCount = other.nodeCount;

        other.head = nullptr;
        other.tail = nullptr;
        other.nodeCount = 0;
    }
    return *this;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    destroy();
}

template<typename T>
void LinkedList<T>::destroy() {
    Link<T>* current = head;
    while (current != nullptr) {
        Link<T>* next = current->getNext();
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    nodeCount = 0;
}

template<typename T>
int LinkedList<T>::size() const {
    return nodeCount;
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
    Link<T>* newLink = new Link<T>(element, tail, nullptr);
    if (tail != nullptr) {
        tail->setNext(newLink);
    } else {
        head = newLink;
    }
    tail = newLink;
    nodeCount++;
}

template<typename T>
void LinkedList<T>::clear() {
    destroy();
}

template<typename T>
bool LinkedList<T>::isEmpty() const {
    return nodeCount == 0;
}

template<typename T>
const T& LinkedList<T>::getElementAt(int index) const {
    if (index < 0 || index >= nodeCount) {
        throw std::out_of_range("Index out of range");
    }
    Link<T>* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->getNext();
    }
    return current->getElement();
}

template<typename T>
void LinkedList<T>::setElementAt(int index, const T& element) {
    if (index < 0 || index >= nodeCount) {
        throw std::out_of_range("Index out of range");
    }
    Link<T>* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->getNext();
    }
    current->setElement(element);
}

template<typename T>
void LinkedList<T>::insert(int index, const T& element) {
    if (index < 0 || index > nodeCount) {
        throw std::out_of_range("Index out of range");
    }
    if (index == nodeCount) {
        append(element);
    } else {
        Link<T>* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->getNext();
        }
        Link<T>* newLink = new Link<T>(element, current->getPrev(), current);
        if (current->getPrev() != nullptr) {
            current->getPrev()->setNext(newLink);
        } else {
            head = newLink;
        }
        current->setPrev(newLink);
        nodeCount++;
    }
}

template<typename T>
bool LinkedList<T>::remove(const T& element) {
    Link<T>* current = head;
    while (current != nullptr) {
        if (current->getElement() == element) {
            if (current->getPrev() != nullptr) {
                current->getPrev()->setNext(current->getNext());
            } else {
                head = current->getNext();
            }
            if (current->getNext() != nullptr) {
                current->getNext()->setPrev(current->getPrev());
            } else {
                tail = current->getPrev();
            }
            delete current;
            nodeCount--;
            return true;
        }
        current = current->getNext();
    }
    return false;
}

template<typename T>
void LinkedList<T>::popBack() {
    if (tail == nullptr) return;
    Link<T>* temp = tail;
    if (tail->getPrev() != nullptr) {
        tail->getPrev()->setNext(nullptr);
    } else {
        head = nullptr;
    }
    tail = tail->getPrev();
    delete temp;
    nodeCount--;
}

template<typename T>
void LinkedList<T>::popFront() {
    if (head == nullptr) return;
    Link<T>* temp = head;
    if (head->getNext() != nullptr) {
        head->getNext()->setPrev(nullptr);
    } else {
        tail = nullptr;
    }
    head = head->getNext();
    delete temp;
    nodeCount--;
}

template<typename T>
Link<T>* LinkedList<T>::find(const T& element) const {
    Link<T>* current = head;
    while (current != nullptr) {
        if (current->getElement() == element) {
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

template<typename T>
Link<T>* LinkedList<T>::getLast() const {
    return tail;
}

template<typename T>
T LinkedList<T>::removeLast() {
    if (tail == nullptr) {
        throw std::out_of_range("List is empty");
    }
    T element = tail->getElement();
    popBack();
    return element;
}

template<typename T>
void LinkedList<T>::traverse(std::function<void(const T&)> func) const {
    Link<T>* current = head;
    while (current != nullptr) {
        func(current->getElement());
        current = current->getNext();
    }
}

template<typename T>
void LinkedList<T>::traverse(std::function<void(T*)> func) const {
    Link<T>* current = head;
    while (current != nullptr) {
        func(&current->getElement());
        current = current->getNext();
    }
}

template<typename T>
void LinkedList<T>::sort(std::function<bool(const T&, const T&)> compare) {
    if (isEmpty() || head->getNext() == nullptr) return;
    quickSort(head, tail, compare);
}

template<typename T>
void LinkedList<T>::quickSort(Link<T>* low, Link<T>* high, std::function<bool(const T&, const T&)> compare) {
    if (low != nullptr && high != nullptr && low != high && low != high->getNext()) {
        Link<T>* p = partition(low, high, compare);
        quickSort(low, p->getPrev(), compare);
        quickSort(p->getNext(), high, compare);
    }
}

template<typename T>
Link<T>* LinkedList<T>::partition(Link<T>* low, Link<T>* high, std::function<bool(const T&, const T&)> compare) {
    T pivot = high->getElement();
    Link<T>* i = low->getPrev();

    for (Link<T>* j = low; j != high; j = j->getNext()) {
        if (compare(j->getElement(), pivot)) {
            i = (i == nullptr) ? low : i->getNext();
            std::swap(i->getElement(), j->getElement());
        }
    }
    i = (i == nullptr) ? low : i->getNext();
    std::swap(i->getElement(), high->getElement());
    return i;
}

template<typename U>
std::ostream& operator<<(std::ostream& out, const LinkedList<U>& list) {
    out << list.nodeCount << "\n";
    Link<U>* current = list.getHead();
    while (current != nullptr) {
        out << current->getElement() << "\n";
        current = current->getNext();
    }
    return out;
}

template<typename U>
std::istream& operator>>(std::istream& in, LinkedList<U>& list) {
    int count;
    in >> count;
    list.clear();
    U element;
    for (int i = 0; i < count; ++i) {
        in >> element;
        list.append(element);
    }
    return in;
}
