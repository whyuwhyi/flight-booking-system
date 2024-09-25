#pragma once

#ifndef NULL
#define NULL 0
#endif

template<typename T>
class TreeNode{
private:
    T value;
    TreeNode<T>* child;
    TreeNode<T>* sibling;
public:
    TreeNode();
    TreeNode(const T& element);
    virtual ~TreeNode();
    bool isLeaf() const;
    T getValue() const;
    TreeNode<T>* getLeftMostChild() const;
    TreeNode<T>* getRightSibling() const;
    void setValue(const T& value);
    void setChild(TreeNode<T>* child);
    void setSibling(TreeNode<T>* sibling);
    void insertAsFirstChild(TreeNode<T>* firstChild);
    void insertAsFirstSibling(TreeNode<T>* firstSibling);
};





template<typename T>
TreeNode<T>::TreeNode():value(),child(NULL),sibling(NULL){}

template<typename T>
TreeNode<T>::TreeNode(const T& element):value(element),child(NULL),sibling(NULL){}

template<typename T>
TreeNode<T>::~TreeNode(){}

template<typename T>
bool TreeNode<T>::isLeaf() const{
    return child == NULL;
}

template<typename T>
T TreeNode<T>::getValue() const{
    return value;
}

template<typename T>
TreeNode<T>* TreeNode<T>::getLeftMostChild() const{
    return child;
}

template<typename T>
TreeNode<T>* TreeNode<T>::getRightSibling() const{
    return sibling;
}

template<typename T>
void TreeNode<T>::setValue(const T& value){
    this->value = value;
}

template<typename T>
void TreeNode<T>::setChild(TreeNode<T>* child){
    this->child = child;
}

template<typename T>
void TreeNode<T>::setSibling(TreeNode<T>* sibling){
    this->sibling = sibling;
}

template<typename T>
void TreeNode<T>::insertAsFirstChild(TreeNode<T>* firstChild){
    if(child != NULL){
        firstChild->sibling = child;
        child = firstChild;
    }
    else
        child = firstChild;
}

template<typename T>
void TreeNode<T>::insertAsFirstSibling(TreeNode<T>* firstSibling){
    if(sibling != NULL){
        firstSibling->sibling = sibling;
        sibling = firstSibling;
    }
    else
        sibling = firstSibling;
}
