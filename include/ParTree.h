#pragma once
#include <ParTreeNode.h>

template<typename T>
class ParTree{
private:
    ParTreeNode<T>* array;
    int size;
public:
    ParTree(const int& size);
    virtual ~ParTree();
    int getSize();
    ParTreeNode<T>* getArray();
    ParTreeNode<T>* Find(ParTreeNode<T>* node) const;
    void Union(int i,int j);
    bool Different(int i,int j);
};




template<typename T>
ParTree<T>::ParTree(const int& size):array(NULL),size(size){
    if(size > 0)
        array = new ParTreeNode<T>[size];
    else
        this->size = 0;
}

template<typename T>
ParTree<T>::~ParTree(){
    if(array != NULL){
        delete[] array;
        array = NULL;
    }
}

template<typename T>
int ParTree<T>::getSize(){
    return size;
}

template<typename T>
ParTreeNode<T>* ParTree<T>::getArray(){
    return array;
}

template<typename T>
ParTreeNode<T>* ParTree<T>::Find(ParTreeNode<T>* node) const{
    ParTreeNode<T>* pointer = node;
    while(pointer->getParent() != NULL)
        pointer = pointer->getParent();
    return pointer;
}

template<typename T>
void ParTree<T>::Union(int i,int j){
    ParTreeNode<T>* pointeri = Find(&array[i]);
    ParTreeNode<T>* pointerj = Find(&array[j]);
    if(pointeri != pointerj){
        if(pointeri->getCount() >= pointerj->getCount()){
            pointerj->setParent(pointeri);
            pointeri->setCount(pointeri->getCount()+pointerj->getCount());
        }
        else{
            pointeri->setParent(pointerj);
            pointerj->setCount(pointeri->getCount()+pointerj->getCount());
        }
    }
}

template<typename T>
bool ParTree<T>::Different(int i,int j){
    ParTreeNode<T>* pointeri = Find(&array[i]);
    ParTreeNode<T>* pointerj = Find(&array[j]);
    return pointeri != pointerj;
}