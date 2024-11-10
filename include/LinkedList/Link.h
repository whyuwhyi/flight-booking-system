
#pragma once

template<typename T>
class Link {
private:
    T element;
    Link<T>* next;
    Link<T>* prev;
public:
    Link(const T& element, Link<T>* prev = nullptr, Link<T>* next = nullptr);
    ~Link();

    void setElement(const T& element);
    T& getElement();
    void setNext(Link<T>* next);
    Link<T>* getNext();
    void setPrev(Link<T>* prev);
    Link<T>* getPrev();
};

template<typename T>
Link<T>::Link(const T& element, Link<T>* prev, Link<T>* next)
    : element(element), prev(prev), next(next) { }

template<typename T>
Link<T>::~Link() { }

template<typename T>
void Link<T>::setElement(const T& element) {
    this->element = element;
}

template<typename T>
T& Link<T>::getElement() {
    return element;
}

template<typename T>
void Link<T>::setNext(Link<T>* next) {
    this->next = next;
}

template<typename T>
Link<T>* Link<T>::getNext() {
    return next;
}

template<typename T>
void Link<T>::setPrev(Link<T>* prev) {
    this->prev = prev;
}

template<typename T>
Link<T>* Link<T>::getPrev() {
    return prev;
}
