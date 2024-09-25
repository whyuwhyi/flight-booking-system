#pragma once
#include <BinaryTree.h>


template <typename T>
class BinarySearchTree:public BinaryTree<T>{
public:
    void insertNode(BinaryTreeNode<T>* newpoint);
    void createTree(const T* array,int n );
    void deleteNode(const T value);
};









template<typename T>
void BinarySearchTree<T>::insertNode(BinaryTreeNode<T>* newPointer){
    if(this->root == NULL){
        this->root = newPointer;
        return;
    }
    else{
        BinaryTreeNode<T>* pointer = this->root;
        while(pointer != NULL){
            if(newPointer->value() == pointer->value())
                return;
            else if(newPointer->value() < pointer->value()){
                if(pointer->Leftchild() == NULL){
                    pointer->setLeftchild(newPointer);
                    return;
                }
                else
                    pointer = pointer->Leftchild();
            }
            else{
                if(pointer->Rightchild() == NULL){
                    pointer->setRightchild(newPointer);
                    return ;
                }
                else
                    pointer = pointer->Rightchild();
            }
        }
    }
}

template<typename T>
void BinarySearchTree<T>::createTree(const T* array,int n){
    for(int i = 0;i < n;i++)
        insertNode(new BinaryTreeNode<T>(array[i]));
}

template<typename T>
void BinarySearchTree<T>::deleteNode(const T value){
    BinaryTreeNode<T>* pointer = this->root,
                     * parent = NULL,
                     * tempPointer,
                     * tempParent = NULL;
    while(pointer != NULL){
        if(pointer->value() == value)
            break;
        else if(pointer->value() < value){
            parent = pointer;
            pointer = pointer->Rightchild();
        }
        else{
            parent = pointer;
            pointer = pointer->Leftchild();
        }
    }
    if(pointer == NULL){
        std::cerr << "No value to be deleted was found." << std::endl;
        return;
    }
    else if(pointer->Leftchild() == NULL)
        tempPointer = pointer->Rightchild();
    else{
        tempPointer = pointer->Leftchild();
        while(tempPointer != NULL){
            tempParent = tempPointer;
            tempPointer = tempPointer->Rightchild();
        }
        if(tempParent == NULL)
            pointer->setLeftchild(tempPointer->Leftchild());
        else
            tempParent->setRightchild(tempPointer->Leftchild());
        tempPointer->setLeftchild(pointer->Leftchild());
        tempPointer->setRightchild(pointer->Rightchild());
    }
    if(parent == NULL)
        this->root = tempPointer;
    else if(parent->Leftchild() == pointer)
        parent->setLeftchild(tempPointer);
    else    
        parent->setRightchild(tempPointer);
    delete pointer;
    pointer = NULL;
}
