#pragma once

#ifndef NULL
#define NULL 0
#endif

template<typename T>
class HuffmanTreeNode{
protected:
    T info;
    HuffmanTreeNode<T>* leftchild;
    HuffmanTreeNode<T>* rightchild;
    HuffmanTreeNode<T>* parent;
public:
    HuffmanTreeNode();
    HuffmanTreeNode(const T& element);
    HuffmanTreeNode(const HuffmanTreeNode<T>& node);
    HuffmanTreeNode(const T& element,HuffmanTreeNode<T>* leftchild,HuffmanTreeNode<T>* rightchild,HuffmanTreeNode<T>* parent);
    virtual ~HuffmanTreeNode();
    T value() const;
    HuffmanTreeNode<T>* Leftchild() const;
    HuffmanTreeNode<T>* Rightchild() const;
    HuffmanTreeNode<T>* Parent() const;
    void setValue(const T& element);
    void setLeftchild(HuffmanTreeNode<T>* leftchild); 
    void setRightchild(HuffmanTreeNode<T>* rightchild); 
    void setParent(HuffmanTreeNode<T>* parent);
    bool isLeaf() const;
    bool operator<(const HuffmanTreeNode<T>& node); 
    bool operator>(const HuffmanTreeNode<T>& node); 
};




template<typename T>
HuffmanTreeNode<T>::HuffmanTreeNode():info(),leftchild(NULL),rightchild(NULL),parent(NULL){}

template<typename T>
HuffmanTreeNode<T>::HuffmanTreeNode(const T& element):info(element),leftchild(NULL),rightchild(NULL),parent(NULL){}

template<typename T>
HuffmanTreeNode<T>::HuffmanTreeNode(const HuffmanTreeNode<T>& node):info(node.info),leftchild(node.leftchild),rightchild(node.rightchild),parent(node.parent){
    if(node.leftchild != NULL)
        node.leftchild->parent = this;
    if(node.rightchild != NULL)
        node.rightchild->parent = this;
}

template<typename T>
HuffmanTreeNode<T>::HuffmanTreeNode(const T& element,HuffmanTreeNode<T>* leftchild,HuffmanTreeNode<T>* rightchild,HuffmanTreeNode<T>* parent):
                    info(element),leftchild(leftchild),rightchild(rightchild),parent(parent){}

template<typename T>
HuffmanTreeNode<T>::~HuffmanTreeNode(){}

template<typename T>
T HuffmanTreeNode<T>::value() const{
    return info;
}

template<typename T>
HuffmanTreeNode<T>* HuffmanTreeNode<T>::Leftchild() const{
    return leftchild;
}

template<typename T>
HuffmanTreeNode<T>* HuffmanTreeNode<T>::Rightchild() const{
    return rightchild;
}

template<typename T>
HuffmanTreeNode<T>* HuffmanTreeNode<T>::Parent() const{
    return parent;
}

template<typename T>
void HuffmanTreeNode<T>::setValue(const T& element){
    info = element;
}

template<typename T>
void HuffmanTreeNode<T>::setLeftchild(HuffmanTreeNode<T>* leftchild){
    this->leftchild = leftchild;
}

template<typename T>
void HuffmanTreeNode<T>::setRightchild(HuffmanTreeNode<T>* rightchild){
    this->rightchild = rightchild;
}

template<typename T>
void HuffmanTreeNode<T>::setParent(HuffmanTreeNode<T>* parent){
    this->parent = parent;
}

template<typename T>
bool HuffmanTreeNode<T>::isLeaf() const{
    return (leftchild == NULL)&&(rightchild == NULL);
}

template<typename T>
bool HuffmanTreeNode<T>::operator<(const HuffmanTreeNode<T>& node){
    return this->info < node.info;
}

template<typename T>
bool HuffmanTreeNode<T>::operator>(const HuffmanTreeNode<T>& node){
    return this->info > node.info;
}