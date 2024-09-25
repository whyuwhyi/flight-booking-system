#pragma once
#include <HuffmanTreeNode.h>
#include <cmath>
#include <MinHeap.h>
#include <string>
#include <sstream>

template<typename T>
int strLength(T element);

template<typename T>
class HuffmanTree{
protected:
    HuffmanTreeNode<T>* root;
    void mergeTree(HuffmanTreeNode<T>& lTree,HuffmanTreeNode<T>& rTree,HuffmanTreeNode<T>* parent);
    void deleteTree(HuffmanTreeNode<T>* root);
public:
    HuffmanTree();
    HuffmanTree(const T* weight,int n);
    virtual ~HuffmanTree();
    HuffmanTreeNode<T>* Root();
    int height(HuffmanTreeNode<T>* root);
    int depth(HuffmanTreeNode<T>* node);
    void trans(HuffmanTreeNode<T>* root,T* array,int index);
    void show(HuffmanTreeNode<T>* root,const T guard);
};





template<typename T>
void HuffmanTree<T>::mergeTree(HuffmanTreeNode<T>& lTree,HuffmanTreeNode<T>& rTree,HuffmanTreeNode<T>* parent){
    HuffmanTreeNode<T>* lChild = new HuffmanTreeNode<T>(lTree),
                      * rChild = new HuffmanTreeNode<T>(rTree);
    lChild->setParent(parent);
    rChild->setParent(parent);
    parent->setValue(lChild->value()+rChild->value());
    parent->setLeftchild(lChild);
    parent->setRightchild(rChild);
}

template<typename T>
void HuffmanTree<T>::deleteTree(HuffmanTreeNode<T>* root){
    if(root != NULL){
        deleteTree(root->Leftchild());
        deleteTree(root->Rightchild());
        delete root;
        root = NULL;
    }
}

template<typename T>
HuffmanTree<T>::HuffmanTree():root(NULL){}

template<typename T>
HuffmanTreeNode<T>* HuffmanTree<T>::Root(){
    return root;
}

template<typename T>
HuffmanTree<T>::HuffmanTree(const T* weight,int n){
    MinHeap<HuffmanTreeNode<T>> heap(n);
    HuffmanTreeNode<T>* parent = NULL,
                      leftchild,
                      rightchild;
    for(int i = 0;i < n;i++)
        heap.insert(HuffmanTreeNode<T>(*(weight+i)));
    for(int i = 0;i < n-1;i++){
        parent  = new HuffmanTreeNode<T>();
        leftchild = heap.removeMin();
        rightchild = heap.removeMin();
        mergeTree(leftchild,rightchild,parent);
        heap.insert(*parent);
    }
    root = parent;
}

template<typename T>
HuffmanTree<T>::~HuffmanTree(){
    deleteTree(root);
}

template<typename T>
int HuffmanTree<T>::height(HuffmanTreeNode<T>* root){
    if(root == NULL)
        return 0;
    int lh = height(root->Leftchild());
    int rh = height(root->Rightchild());
    return lh>rh?(lh+1):(rh+1);
}

template<typename T>
int HuffmanTree<T>::depth(HuffmanTreeNode<T>* node){
    if(node == NULL)
        return 0;
    return depth(node->Parent())+1;
}

template<typename T>
void HuffmanTree<T>::trans(HuffmanTreeNode<T>* root,T* array,int index){
    if(root != NULL){
        array[index] = root->value();
        trans(root->Leftchild(),array,2*index+1);
        trans(root->Rightchild(),array,2*index+2);
    }
}

template<typename T>
void HuffmanTree<T>::show(HuffmanTreeNode<T>* root,const T guard){
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
                        lw = strLength(array[count-1]);
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
int strLength(const T element){
    std::stringstream ss;
    std::string str;
    ss << element;
    ss >> str;
    return str.length();
}
