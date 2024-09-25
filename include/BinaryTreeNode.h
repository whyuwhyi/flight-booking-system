#pragma once

#ifndef NULL
#define NULL 0
#endif

template<typename T>
class BinaryTreeNode{
protected:
    BinaryTreeNode<T> *leftchild,*rightchild;
    T info;
public:
    BinaryTreeNode();
    BinaryTreeNode(const T& element);
    BinaryTreeNode(const T& element,BinaryTreeNode<T> *leftchild,BinaryTreeNode<T> *rightchild);
    virtual ~BinaryTreeNode();
    T value() const;
    BinaryTreeNode<T>* Leftchild() const;
    BinaryTreeNode<T>* Rightchild() const;
    void setLeftchild(BinaryTreeNode<T> *leftchild);
    void setRightchild(BinaryTreeNode<T> *rightchild);
    void setValue(const T& value);
    bool isLeaf() const;
};




template<typename T>
BinaryTreeNode<T>::BinaryTreeNode():info(),leftchild(NULL),rightchild(NULL){}

template<typename T>
BinaryTreeNode<T>::BinaryTreeNode(const T& element):info(element),leftchild(NULL),rightchild(NULL){}

template<typename T>
BinaryTreeNode<T>::BinaryTreeNode(const T& element,BinaryTreeNode<T> *leftchild,BinaryTreeNode<T> *rightchild)
                                    :info(element),leftchild(leftchild),rightchild(rightchild){}

template<typename T>
BinaryTreeNode<T>::~BinaryTreeNode(){

}

template<typename T>
T BinaryTreeNode<T>::value() const{
    return info;
}

template<typename T>
BinaryTreeNode<T>* BinaryTreeNode<T>::Leftchild() const{
    return leftchild;
}

template<typename T>
BinaryTreeNode<T>* BinaryTreeNode<T>::Rightchild() const{
    return rightchild;
}

template<typename T>
void BinaryTreeNode<T>::setLeftchild(BinaryTreeNode<T> *leftchild){
    this->leftchild = leftchild;
}

template<typename T>
void BinaryTreeNode<T>::setRightchild(BinaryTreeNode<T> *rightchild){
    this->rightchild = rightchild;
}

template<typename T>
void BinaryTreeNode<T>::setValue(const T& value){
    this->info = value;
}

template<typename T>
bool BinaryTreeNode<T>::isLeaf() const{
    return (leftchild == NULL)&&(rightchild == NULL);
};
