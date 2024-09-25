#pragma once
#include <iostream>



template<typename T>
class MinHeap{
private:
    T* heapArray;
    int curSize;
    int maxSize;
    void buildHeap();
    void swap(int pos_x,int pos_y);
public:
    MinHeap(const int n);
    MinHeap(const T* array,const int n);
    virtual ~MinHeap();
    bool isLeaf(int pos) const;
    int leftchild(int pos) const;
    int rightchild(int pos) const;
    int parent(int pos) const;
    bool remove(int pos,T& node);
    bool insert(const T& newNode);
    T& removeMin();
    void siftUp(int pos);
    void siftDown(int pos);
    void show();
    static bool isMinheap(const T* array,int arrayLength);
};





template<typename T>
MinHeap<T>::MinHeap(const int n){
    if(n <= 0)
        return;
    else{
        curSize = 0;
        maxSize = n;
        heapArray = new T[n];
    }
}

template<typename T>
MinHeap<T>::MinHeap(const T* array,const int n){
    if(n <= 0)
        return;
    else{
        curSize = n;
        maxSize = n;
        heapArray = new T[n];
        for(int i = 0;i < n;i++)
            heapArray[i] = *(array+i);
        buildHeap();
    }
}

template<typename T>
MinHeap<T>::~MinHeap(){
    delete []heapArray;
}

template<typename T>
bool MinHeap<T>::isLeaf(int pos) const{
    return (pos >= curSize/2)&&(pos < curSize);
}

template<typename T>
int MinHeap<T>::leftchild(int pos) const{
    return 2*pos+1;
}

template<typename T>
int MinHeap<T>::rightchild(int pos) const{
    return 2*pos+2;
}

template<typename T>
int MinHeap<T>::parent(int pos) const{
    return (pos-1)/2;
}

template<typename T>
void MinHeap<T>::buildHeap(){
    for(int i = curSize/2-1;i >= 0;i--)
        siftDown(i);
}

template<typename T>
void MinHeap<T>::swap(int pos_x,int pos_y){
    if(pos_x < 0||pos_x >= maxSize||pos_y < 0||pos_y >= maxSize)
        return;
    T temp = heapArray[pos_x];
    heapArray[pos_x] = heapArray[pos_y];
    heapArray[pos_y] = temp;
}

template<typename T>
void MinHeap<T>::siftDown(int pos){
    int i = pos;
    int j = leftchild(i);
    T temp = heapArray[i];
    while(j < curSize){
        if((j < curSize-1)&&(heapArray[j] > heapArray[j+1]))
            j++;
        if(temp > heapArray[j]){
            heapArray[i] = heapArray[j];
            i = j;
            j = leftchild(i);
        }
        else
            break;
    }
    heapArray[i] = temp;
}

template<typename T>
void MinHeap<T>::siftUp(int pos){
    int tempPos = pos;
    T temp = heapArray[pos];
    while((tempPos > 0)&&(heapArray[parent(tempPos)] > temp)){
        heapArray[tempPos] = heapArray[parent(tempPos)];
        tempPos = parent(tempPos);
    }
    heapArray[tempPos] = temp;
}

template<typename T>
bool MinHeap<T>::insert(const T& newNode){
    if(curSize == maxSize)
        return 0;
    heapArray[curSize] = newNode;
    siftUp(curSize);
    curSize++;
    return 1;
}

template<typename T>
bool MinHeap<T>::remove(int pos,T & node){
    if(pos < 0||pos > curSize)
        return 0;
    node = heapArray[pos];
    heapArray[pos] = heapArray[--curSize];
    if(heapArray[parent(pos)] > heapArray[pos])
        siftUp(pos);
    else
        siftDown(pos);
    return 1;

}

template<typename T>
T& MinHeap<T>::removeMin(){
    if(curSize == 0){
        std::cerr << "No item to delete." << std::endl;
        exit(1);
    }
    else{
        swap(0,--curSize);
        if(curSize > 1)
            siftDown(0);
        return heapArray[curSize];
    }
}

template<typename T>
void MinHeap<T>::show(){
    for(int i = 0;i < curSize;i++)
        std::cout << heapArray[i] << " ";
}

template<typename T>
bool MinHeap<T>::isMinheap(const T* array,int arrayLength){
    for(int i = 0;i < arrayLength/2;i++){
        if(array[2*i+1] > array[i] || array[2*i+2] > array[i])
            return false;
    }
    return true;
}