#pragma once

#include <LinkedList/Link.h>


template<typename T>
class LinkedList{
private:
    Link<T>* head;
public:
    LinkedList();
    ~LinkedList();
    Link<T>* getHead() const;
    void setHead(Link<T>* head);
    void append(const T& element);
    void destroy();

    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const LinkedList<U>& list);

    template<typename U>
    friend std::istream& operator>>(std::istream& in, LinkedList<T>& list);
};


template<typename T>
LinkedList<T>::LinkedList():head(nullptr){ }

template<typename T>
LinkedList<T>::~LinkedList() {
    this->destroy();
}

template<typename T>
Link<T>* LinkedList<T>::getHead() const{
    return head;
}


template<typename T>
void LinkedList<T>::setHead(Link<T>* head) {
    this->head = head;
}

template<typename T>
void LinkedList<T>::append(const T& element) {
    Link<T>* pointer = head;
    if (head == NULL) {
        head = new Link<T>(element);
        return ;
    }
    while (pointer->getNext() != NULL) {
        pointer = pointer->getNext();
    }
    pointer->setNext(new Link<T>(element));
}

template<typename T>
void LinkedList<T>::destroy() {
    Link<T>* node;
    while(head != nullptr){
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
        }
        else {
            tail->setNext(newLink);
        }
        tail = newLink;
    }
    return in;
}
