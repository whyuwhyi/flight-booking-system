#pragma once
#include <iostream>
#include <TreeNode.h>
#include <BinaryTree.h>
#include <queue>
#include <stack>


template<typename T>
class Tree{
private:
    TreeNode<T>* root;
public:
    Tree();
    virtual ~Tree();
    void createTree(TreeNode<T>* root);
    bool isEmpty();
    TreeNode<T>* getRoot();
    TreeNode<T>* getParent(TreeNode<T>* current);
    TreeNode<T>* getPreviousSibling(TreeNode<T>* current);
    void deleteSubTree(TreeNode<T>* subRoot);
    void deleteNodes(TreeNode<T>* root);
    void visit(TreeNode<T>* node);
    void traverseByRootFirst(TreeNode<T>* root);
    void traverseByRootLast(TreeNode<T>* root);
    void traverseByWidth(TreeNode<T>* root);
    void treeToBinaryTree(TreeNode<T>* root,BinaryTree<T>& bTree);
    int nodeCount(TreeNode<T>* root);
    static bool compareTwoTree(TreeNode<T>* root1,TreeNode<T>* root2);
};





template<typename T>
Tree<T>::Tree():root(NULL){}

template<typename T>
Tree<T>::~Tree(){
    deleteSubTree(root);
    root = NULL;
}

template<typename T>
void Tree<T>::createTree(TreeNode<T>* root){
    this->root = root;
}

template<typename T>
bool Tree<T>::isEmpty(){
    return root == NULL;
}

template <typename T>
TreeNode<T>* Tree<T>::getRoot(){
    return root;
}

template<typename T>
TreeNode<T>* Tree<T>::getParent(TreeNode<T>* current){
    std::queue<TreeNode<T>*> nodeQueue;
    TreeNode<T>* pointer = root,
               * upperlevelpointer = NULL;
    if(current != NULL && pointer != current){
        while(pointer != NULL){
            if(current == pointer)
                return NULL;
            nodeQueue.push(pointer);
            pointer = pointer->getRightSibling();
        }
        while(!nodeQueue.empty()){
            pointer = nodeQueue.front();
            nodeQueue.pop();
            upperlevelpointer = pointer;
            pointer = pointer->getLeftMostChild();
            while(pointer != NULL){
                if(current == pointer)
                    return upperlevelpointer;
                else{
                    nodeQueue.push(pointer);
                    pointer = pointer->getRightSibling();
                }
            }
        }
    }
    return NULL;
}

template<typename T>
TreeNode<T>* Tree<T>::getPreviousSibling(TreeNode<T>* current){
    std::queue<TreeNode<T>*> nodeQueue;
    TreeNode<T>* prev = NULL,
               * temp = root;
    if(current == NULL||temp == NULL||current == temp)
        return NULL;
    while(temp != NULL){
        if(temp = current)
            return prev;
        nodeQueue.push(temp);
        prev = temp;
        temp = temp->getRightSibling();
    }
    while(!nodeQueue.empty()){
        prev = NULL;
        temp = nodeQueue.front();
        nodeQueue.pop();
        temp = temp->getLeftMostChild();
        while(temp != NULL){
            if(temp == current)
                return prev;
            nodeQueue.push(temp);
            prev = temp;
            temp = temp->getRightSibling();
        }
    }
    return NULL;
}


template<typename T>
void Tree<T>::deleteSubTree(TreeNode<T>* subRoot){
    TreeNode<T>* pointer;
    if(subRoot == NULL)
        return;
    pointer = getParent(subRoot);
    if(pointer == NULL)
        root = subRoot->getRightSibling();
    else if(pointer->getLeftMostChild() == subRoot)
        pointer->setChild(subRoot->getRightSibling());
    else{
        pointer = pointer->getLeftMostChild();
        while(pointer != subRoot)
            pointer = pointer->getRightSibling();
        pointer->setSibling(subRoot->getRightSibling());
    }
    subRoot->setSibling(NULL);
    deleteNodes(subRoot);
}

template<typename T>
void Tree<T>::deleteNodes(TreeNode<T>* root){
    if(root != NULL){
        deleteNodes(root->getLeftMostChild());
        deleteNodes(root->getRightSibling());
        delete root;
        root = NULL;
    }
}

template<typename T>
void Tree<T>::visit(TreeNode<T>* node){
    std::cout << node->getValue();
}

template<typename T>
void Tree<T>::traverseByRootFirst(TreeNode<T>* root){
    while(root !=  NULL){
        visit(root);
        traverseByRootFirst(root->getLeftMostChild());
        root = root->getRightSibling();
    }
}

template<typename T>
void Tree<T>::traverseByRootLast(TreeNode<T>* root){
    while(root !=  NULL){
        traverseByRootLast(root->getLeftMostChild());
        visit(root);
        root = root->getRightSibling();
    }
}

template<typename T>
void Tree<T>::traverseByWidth(TreeNode<T>* root){
    std::queue<TreeNode<T>*> nodeQueue;
    TreeNode<T>* temp = root;
    while(temp != NULL){
        nodeQueue.push(temp);
        temp = temp->getRightSibling();
    }
    while(!nodeQueue.empty()){
        temp = nodeQueue.front();
        nodeQueue.pop();
        visit(temp);
        temp = temp->getLeftMostChild();
        while(temp != NULL){
            nodeQueue.push(temp);
            temp = temp->getRightSibling();
        }
    }
}

template<typename T>
void Tree<T>::treeToBinaryTree(TreeNode<T>* root,BinaryTree<T>& bTree){
    BinaryTree<T> lTree,
                  rTree;
    if(root != NULL){
        treeToBinaryTree(root->getLeftMostChild(),lTree);
        treeToBinaryTree(root->getRightSibling(),rTree);
        bTree.createTree(root->getValue(),lTree,rTree);
    } 
}

template<typename T>
int Tree<T>::nodeCount(TreeNode<T>* root){
    if(root == NULL)
        return 0;
    else{
        int childCount = nodeCount(root->getLeftMostChild());
        int siblingCount = nodeCount(root->getRightSibling());
        return childCount+siblingCount+1;
    }
}

template<typename T>
bool Tree<T>::compareTwoTree(TreeNode<T>* root1,TreeNode<T>* root2){
    if(root1 == NULL&&root2 == NULL)
        return 1;
    else if(root1 != NULL&&root2 != NULL){
        if(root1->getValue() == root2->getValue()){
            bool childFlag = compareTwoTree(root1->getLeftMostChild(),root2->getLeftMostChild());
            bool siblingFlag = compareTwoTree(root1->getRightSibling(),root2->getRightSibling());
            return childFlag&&siblingFlag;
        }
        else
            return 0;
    }
    else 
        return 0;
}
