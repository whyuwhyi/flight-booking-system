#pragma once
#include <BinaryTreeNode.h>
#include <stack>
#include <queue>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

template<typename T>
void printRange(BinaryTreeNode<T>* root,const T min,const T max);

template<typename T>
int strLength(const T element);

template<typename T>
class BinaryTree{
protected:
    BinaryTreeNode<T>* root;
public:
    BinaryTree();
    virtual ~BinaryTree();
    bool isEmpty() const;
    BinaryTreeNode<T>* Root();
    BinaryTreeNode<T>* Parent(BinaryTreeNode<T>* current);
    BinaryTreeNode<T>* leftSibling(BinaryTreeNode<T>* current);
    BinaryTreeNode<T>* rightSibling(BinaryTreeNode<T>* current);
    void createTree(BinaryTreeNode<T>* root);
    void createTree(const T& info,BinaryTree<T>& leftTree,BinaryTree<T>& rightTree);
    void visit(BinaryTreeNode<T>* root);
    void preOrder(BinaryTreeNode<T>* root);
    void inOrder(BinaryTreeNode<T>* root);
    void postOrder(BinaryTreeNode<T>* root);
    void levelOrder(BinaryTreeNode<T>* root);
    void deleteBinaryTree(BinaryTreeNode<T>* root);
    int height(BinaryTreeNode<T>* root);
    void trans(BinaryTreeNode<T>* root,T* array,int index);
    void show(BinaryTreeNode<T>* root,T guard);
};









template<typename T>
BinaryTree<T>::BinaryTree():root(NULL){}

template<typename T>
BinaryTree<T>::~BinaryTree(){
    deleteBinaryTree(root);
}

template<typename T>
bool BinaryTree<T>::isEmpty() const{
    return (root==NULL);
}

template<typename T>
BinaryTreeNode<T>* BinaryTree<T>::Root(){
    return root;
}

template<typename T>
BinaryTreeNode<T>* BinaryTree<T>::Parent(BinaryTreeNode<T>* current){
    std::stack<BinaryTreeNode<T>*> st;
    st.push(NULL);
    BinaryTreeNode<T>* temp = root;
    while(temp != NULL){
        if(temp->Leftchild() == current||temp->Rightchild() == current)
            return temp;
        if(temp->Leftchild() != NULL)
            st.push(temp->Leftchild());
        if(temp->Rightchild() != NULL)
            st.push(temp->Rightchild());
        temp = st.top();
        st.pop();
    }
    std::cout << "The current Node has no parent." << std::endl;
    return NULL;
}

template<typename T>
BinaryTreeNode<T>* BinaryTree<T>::leftSibling(BinaryTreeNode<T>* current){
    std::stack<BinaryTreeNode<T>*> st;
    st.push(NULL);
    BinaryTreeNode<T>* temp = root;
    while(temp != NULL){
        if(temp->Rightchild() == current)
            return temp->Leftchild();
        if(temp->Leftchild() != NULL)
            st.push(temp->Leftchild());
        if(temp->Rightchild() != NULL)
            st.push(temp->Rightchild());
        temp = st.top();
        st.pop();
    }
    std::cout << "The current Node has no leftSibling." << std::endl;
    return NULL;
}

template<typename T>
BinaryTreeNode<T>* BinaryTree<T>::rightSibling(BinaryTreeNode<T>* current){
    std::stack<BinaryTreeNode<T>*> st;
    st.push(NULL);
    BinaryTreeNode<T>* temp = root;
    while(temp != NULL){
        if(temp->Leftchild() == current)
            return temp->Rightchild();
        if(temp->Leftchild() != NULL)
            st.push(temp->Leftchild());
        if(temp->Rightchild() != NULL)
            st.push(temp->Rightchild());
        temp = st.top();
        st.pop();
    }
    std::cout << "The current Node has no leftSibling." << std::endl;
    return NULL;
}

template<typename T>
void BinaryTree<T>::createTree(BinaryTreeNode<T>* root){
    this->root = root; 
}

template<typename T>
void BinaryTree<T>::createTree(const T& info,BinaryTree<T>& leftTree,BinaryTree<T>& rightTree){
    root = new BinaryTreeNode<T>(info,leftTree.Root(),rightTree.Root());
    leftTree.root = NULL;
    rightTree.root = NULL;
}

template<typename T>
void BinaryTree<T>::visit(BinaryTreeNode<T>* root){
    if(root != NULL)
        std::cout << root->value() << " ";
    else  
        std::cout << "The node is NULL." << std::endl;
}

template<typename T>
void BinaryTree<T>::preOrder(BinaryTreeNode<T>* root){
    if(root != NULL){
        visit(root);
        preOrder(root->Leftchild());
        preOrder(root->Rightchild());
    }
}

template<typename T>
void BinaryTree<T>::inOrder(BinaryTreeNode<T>* root){
    if(root != NULL){
        inOrder(root->Leftchild());
        visit(root);
        inOrder(root->Rightchild());
    }
}

template<typename T>
void BinaryTree<T>::postOrder(BinaryTreeNode<T>* root){
    if(root != NULL){
        postOrder(root->Leftchild());
        postOrder(root->Rightchild());
        visit(root);
    }
}

template<typename T>
void BinaryTree<T>::levelOrder(BinaryTreeNode<T>* root){
    std::queue<BinaryTreeNode<T>*> qe;
    BinaryTreeNode<T>* temp = root;
    if(temp != NULL)
        qe.push(temp);
    while(!qe.empty()){
        temp = qe.front();
        qe.pop();
        visit(temp);
        if(temp->Leftchild() != NULL)
            qe.push(temp->Leftchild());
        if(temp->Rightchild() != NULL)
            qe.push(temp->Rightchild());
    }
}

template<typename T>
void BinaryTree<T>::deleteBinaryTree(BinaryTreeNode<T>* root){
    if(root != NULL){
        deleteBinaryTree(root->Leftchild());
        deleteBinaryTree(root->Rightchild());
        std::cout << "In tree " << this->root->value() << ":Node " << root->value() << " has been distructed" << std::endl;
        delete root;
        root = NULL;
    }
    
}

template<typename T>
int BinaryTree<T>::height(BinaryTreeNode<T>* root){
    if(root == NULL) return 0;
    int lh = height(root->Leftchild());
    int rh = height(root->Rightchild());
    return lh > rh?lh+1:rh+1;
}

template<typename T>
void BinaryTree<T>::trans(BinaryTreeNode<T>* root,T* array,int index){
    if(root != NULL){
        array[index] = root->value();
        trans(root->Leftchild(),array,2*index+1);
        trans(root->Rightchild(),array,2*index+2);
    }
}

template<typename T>
void BinaryTree<T>::show(BinaryTreeNode<T>* root,T guard){
    int h = height(root);
    int l = pow(2,h+2)-1;    
    if(!h)
        std::cout << "The tree is empty." << std::endl;
    else{
        int n = (l+1)/4-1;
        std::cout << "The height of the tree is " << h << '.' << std::endl;
        T* array = new T[n],temp;
        for(int i = 0;i < n;i++)
            array[i] = guard;
        trans(root,array,0);
        for(int i = 0;i < h;i++){
            int ll = pow(2,i);
            if(i){
                for(int x = 0;x < (l+1)/2-2;x++)
                    std::cout << " ";
                for(int count = ll;count < 2*ll;count+=2){
                    if(array[count-1] == guard&&array[count] == guard){
                        for(int x = 0;x < l+2;x++)
                            std::cout << " ";
                    }
                    else if(array[count-1] == guard&&array[count] != guard){
                        for(int x = 0;x < (l+1)/2;x++)
                            std::cout << " ";
                        std::cout << "|";
                        for(int x = 0;x < (l-1)/2;x++)
                            std::cout << "_";
                        std::cout << " ";   
                    }
                    else if(array[count-1] != guard&&array[count] == guard){
                        std::cout << " ";
                        for(int x = 0;x < (l-1)/2;x++)
                            std::cout << "_";
                        std::cout << "|";
                        for(int x = 0;x < (l+1)/2;x++)
                            std::cout << " ";
                    }
                    else{
                        std::cout << " ";
                        for(int x = 0;x < (l-1)/2;x++)
                            std::cout << "_";
                        std::cout << "|";
                        for(int x = 0;x < (l-1)/2;x++)
                            std::cout << "_";
                        std::cout << " ";
                    }
                    for(int x = 0;x < l;x++)
                    std::cout << " ";
                }
                std::cout << std::endl;
                for(int x = 0;x < (l+1)/2-2;x++)
                    std::cout << " ";
                for(int count = ll;count < 2*ll;count+=2){ 
                    if(array[count-1] == guard)
                        std::cout << " ";
                    else 
                        std::cout << "|";
                    for(int x = 0;x < l;x++)
                        std::cout << " ";
                    if(array[count] == guard)
                        std::cout << " ";
                    else 
                        std::cout << "|";
                    for(int x = 0;x < l;x++)
                        std::cout << " ";
                }
                std::cout << std::endl;
                for(int x = 0;x < (l+1)/2-2;x++)
                    std::cout << " ";
                for(int count = ll;count < 2*ll;count+=2){
                    int lw = 1,rw = 1;
                     if(array[count-1] == guard)
                        std::cout << " ";
                    else{ 
                        std::cout << array[count-1];
                        lw = strLength(array[count]-1);
                    }
                    
                    if(array[count] == guard){
                        for(int x = 0;x < l;x++)
                            std::cout << " ";
                        std::cout << " ";
                    }
                    else{ 
                        rw = strLength(array[count]);
                        for(int x = 0;x < l+2-lw-rw;x++)
                            std::cout << " ";
                        std::cout << array[count];
                    }
                    for(int x = 0;x < l;x++)
                        std::cout << " ";
                }
                std::cout << std::endl;
            }
            else{
                for(int x = 0;x < (l+1)/2-2-strLength(array[0])/2;x++)
                    std::cout <<" ";
                std::cout << array[0] << std::endl;
            }
            l = (l-1)/2;
        }
        delete[] array;
    }
}

template<typename T>
void printRange(BinaryTreeNode<T>* root,const T min,const T max){
    if(root != NULL){
        if(root->value() < min)
            printRange(root->Rightchild(),min,max);
        else if(root->value() > max)
            printRange(root->Leftchild(),min,max);
        else{
            printRange(root->Leftchild(),min,max);
            std::cout << root->value() << " ";
            printRange(root->Rightchild(),min,max);
        }
    }
}

template<typename T>
int strLength(const T element){
    std::stringstream ss;
    std::string str;
    ss << element;
    ss >> str;
    return str.length();
}