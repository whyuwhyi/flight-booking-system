#pragma once
#include <iostream>


template<typename T>
class Link{
private:
    T element;
    Link<T>* next;
public:
    Link(const T& element,Link<T>* next = nullptr);
    Link(Link<T>* next = nullptr);
    ~Link();
    void setElement(const T& element);
    T& getElement();
    void setNext(Link<T>* next);
    Link<T>* getNext();

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const Link<U>& link);

    template <typename U>
    friend std::istream& operator>>(std::istream& in, Link<U>& link);

};





template<typename T>
Link<T>::Link(const T& element,Link<T>* next):element(element),next(next){}

template<typename T>
Link<T>::Link(Link<T>* next):element(),next(next){}

template<typename T>
Link<T>::~Link(){};

template<typename T>
void Link<T>::setElement(const T& element){
    this->element = element;
}

template<typename T>
T& Link<T>::getElement() {
    return element;
}

template<typename T>
void Link<T>::setNext(Link<T>* next){
    this->next = next;
}

template<typename T>
Link<T>* Link<T>::getNext(){
    return next;
}

template<typename U>
std::ostream& operator<<(std::ostream& out, const Link<U>& link) {
    out << link.element;
    return out;
}

template<typename U>
std::istream& operator>>(std::istream& in, Link<U>& link) {
    in >> link.element;
    return in;
}