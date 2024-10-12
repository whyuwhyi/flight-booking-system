#pragma once

#include <LinkedList/Link.h>
#include <functional>
#include <iostream>
#include <stdexcept>

template<typename T>
class LinkedList {
private:
    Link<T>* head;
    int nodeCount;

    void destory();
public:
    LinkedList();
    LinkedList(const LinkedList<T>& other);
    ~LinkedList();

    int size() const;
    Link<T>* getHead() const;
    void setHead(Link<T>* head);
    void append(const T& element);
    void clear();

    bool isEmpty() const;
    T& getElementAt(int index) const;
    void insert(int index, const T& element);
    bool remove(const T& element);
    void popBack();
    void popFront();
    Link<T>* find(const T& element) const;
    Link<T>* getLast() const;
    T removeLast();
    void traverse(std::function<void(const T&)> func) const;
    void traverse(std::function<void(T*)> func) const;

    LinkedList<T>& operator=(const LinkedList<T>& other);


    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const LinkedList<U>& list);

    template<typename U>
    friend std::istream& operator>>(std::istream& in, LinkedList<U>& list);
};

template<typename T>
LinkedList<T>::LinkedList() : head(nullptr), nodeCount(0) { }

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head(nullptr), nodeCount(0) {
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
        nodeCount++;
    }
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
    if (this != &other) {
        clear();
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
            nodeCount++;
        }
    }
    return *this;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    destory();
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
    nodeCount++;
}

template<typename T>
void LinkedList<T>::destory() {
    Link<T>* node;
    while (head != nullptr) {
        node = head;
        head = head->getNext();
        delete node;
    }
}

template<typename T>
void LinkedList<T>::clear() {
    destory();
    head = nullptr;
    nodeCount = 0;
}

template<typename T>
bool LinkedList<T>::isEmpty() const {
    return head == nullptr;
}

template<typename T>
T& LinkedList<T>::getElementAt(int index) const {
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
void LinkedList<T>::insert(int index, const T& element) {
    if (index < 0 || index > nodeCount) {
        throw std::out_of_range("Index out of range");
    }
    Link<T>* newLink = new Link<T>(element);
    if (index == 0) {
        newLink->setNext(head);
        head = newLink;
    } else {
        Link<T>* current = head;
        for (int i = 0; i < index - 1; ++i) {
            current = current->getNext();
        }
        newLink->setNext(current->getNext());
        current->setNext(newLink);
    }
    nodeCount++;
}

template<typename T>
bool LinkedList<T>::remove(const T& element) {
    Link<T>* current = head;
    Link<T>* previous = nullptr;
    while (current != nullptr) {
        if (current->getElement() == element) {
            if (previous == nullptr) {
                head = current->getNext();
            } else {
                previous->setNext(current->getNext());
            }
            delete current;
            nodeCount--;
            return true;
        }
        previous = current;
        current = current->getNext();
    }
    return false;
}

template<typename T>
void LinkedList<T>::popBack() {
    if (head == nullptr) return;
    if (head->getNext() == nullptr) {
        delete head;
        head = nullptr;
    } else {
        Link<T>* current = head;
        Link<T>* previous = nullptr;
        while (current->getNext() != nullptr) {
            previous = current;
            current = current->getNext();
        }
        previous->setNext(nullptr);
        delete current;
    }
    nodeCount--;
}

template<typename T>
void LinkedList<T>::popFront() {
    if (head == nullptr) return;
    Link<T>* temp = head;
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
    if (head == nullptr) return nullptr;
    Link<T>* current = head;
    while (current->getNext() != nullptr) {
        current = current->getNext();
    }
    return current;
}

template<typename T>
T LinkedList<T>::removeLast() {
    if (head == nullptr) {
        throw std::out_of_range("List is empty");
    }
    if (head->getNext() == nullptr) {
        T element = head->getElement();
        delete head;
        head = nullptr;
        nodeCount--;
        return element;
    }
    Link<T>* current = head;
    Link<T>* previous = nullptr;
    while (current->getNext() != nullptr) {
        previous = current;
        current = current->getNext();
    }
    T element = current->getElement();
    delete current;
    previous->setNext(nullptr);
    nodeCount--;
    return element;
}

template <typename T>
void LinkedList<T>::traverse(std::function<void(const T&)> func) const {
    Link<T>* current = head;
    while (current != nullptr) {
        func(current->getElement());
        current = current->getNext();
    }
}

template <typename T>
void LinkedList<T>::traverse(std::function<void(T*)> func) const {
    Link<T>* current = head;
    while (current != nullptr) {
        func(&current->getElement());
        current = current->getNext();
    }
}

template<typename U>
std::ostream& operator<<(std::ostream& out, const LinkedList<U>& list) {
    out << list.nodeCount << "\n";
    Link<U>* current = list.getHead();
    while (current != nullptr) {
        out << *current << "\n";
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
    Link<U>* tail = nullptr;
    for (int i = 0; i < count; ++i) {
        in >> element;
        Link<U>* newLink = new Link<U>(element);
        if (tail == nullptr) {
            list.setHead(newLink);
        } else {
            tail->setNext(newLink);
        }
        tail = newLink;
        list.nodeCount++;
    }
    return in;
}
