#pragma once

#include <LinkedList/Link.h>

template<typename T>
class LinkedList {
private:
    Link<T>* head;
    int nodeCount; // 记录节点个数

    void destory();
public:
    LinkedList();
    LinkedList(const LinkedList<T>& other);               // 拷贝构造函数
    LinkedList<T>& operator=(const LinkedList<T>& other); // 拷贝赋值运算符
    ~LinkedList();

    int size() const; // 返回节点个数
    Link<T>* getHead() const;
    void setHead(Link<T>* head);
    void append(const T& element);
    void clear();

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

template<typename U>
std::ostream& operator<<(std::ostream& out, const LinkedList<U>& list) {
    out << list.nodeCount << "\n"; // 输出节点个数
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