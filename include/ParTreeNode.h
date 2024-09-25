#pragma once

#ifndef NULL
#define NULL 0
#endif

template<typename T>
class ParTreeNode{
private:
    T value;
    ParTreeNode<T>* parent;
    int count;
public:
    ParTreeNode();
    virtual ~ParTreeNode();
    T getValue() const;
    void setValue(const T& value);
    ParTreeNode<T>* getParent() const;
    void setParent(ParTreeNode<T>* parent);
    int getCount() const;
    void setCount(const int& count);
};



template<typename T>
ParTreeNode<T>::ParTreeNode():value(),parent(NULL),count(0){}

template<typename T>
ParTreeNode<T>::~ParTreeNode(){}

template<typename T>
T ParTreeNode<T>::getValue() const{
    return value;
}

template<typename T>
void ParTreeNode<T>::setValue(const T& value){
    this->value = value;
}

template<typename T>
ParTreeNode<T>* ParTreeNode<T>::getParent() const{
    return parent;
}

template<typename T>
void ParTreeNode<T>::setParent(ParTreeNode<T>* parent){
    this->parent = parent;
}

template<typename T>
int ParTreeNode<T>::getCount() const{
    return count;
}

template<typename T>
void ParTreeNode<T>::setCount(const int& count){
    this->count = count;
}


