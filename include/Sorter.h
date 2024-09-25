#pragma once
#include <MinHeap.h>
#define QUICKSORT_THRESHOLD 16
#define MERGESORT_THRESHOLD 28


template<typename T>
class Sorter{
public:
    static void swap(T& a,T& b);
    static void insertSort(T array[],int n);
    static void modifyInsertSort(T array[],int n,int delta);
    static void shellSort(T array[],int n);
    static void selectSort(T array[],int n);
    static void bubbleSort(T array[],int n);
    static void modifyBubbleSort(T array[],int n);
    static void quickSort(T array[],int left,int right);
    static void modifyQuickSort(T array[],int n);
    static void modQuickSort(T array[],int left,int right);
    static int selectPivot(int left,int right);
    static int partition(T array[],int left,int right);
    static void mergeSort(T array[],T tempArray[],int left,int right);
    static void merge(T array[],T tempArray[],int left,int right,int middle);
    static void modifyMergeSort(T array[],T tempArray[],int left,int right);
    static void modifyMerge(T array[],T tempArray[],int left,int right,int middle);
    static void bucketSort(T array[],int n,int max);
    static void heapSort(T array[],int n);
    static void radixSort(T array[],int n,int d,int r);
};





template<typename T>
void Sorter<T>::swap(T& a,T&b){
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
void Sorter<T>::insertSort(T array[],int n){
    for(int i = 1;i < n;i++){
        T temp = array[i];
        int j = i-1;
        while(j >= 0&& temp < array[j]){
            array[j+1] = array[j];
            j--;
        }
        array[j+1] = temp;
    }
}

template<typename T>
void Sorter<T>::modifyInsertSort(T array[],int n,int delta){
    for(int i = delta;i < n;i += delta)
        for(int j = i;j >= delta;j-= delta)
            if(array[j-delta] > array[j])
                Sorter<T>::swap(array[j-delta],array[j]);
            else
                break;
}

template<typename T>
void Sorter<T>::shellSort(T array[],int n){
    for(int delta = n/2;delta > 0;delta /= 2)
        for(int i = 0;i < delta;i++)
            Sorter<T>::modifyInsertSort(&array[i],n-1,delta);
}


template<typename T>
void Sorter<T>::selectSort(T array[],int n){
    for(int i = 0;i < n-1;i++){
        int minIndex = i;
        for(int j = minIndex+1;j < n;j++)
            if(array[j] < array[minIndex])
                minIndex = j;
        Sorter<T>::swap(array[i],array[minIndex]);
    }
}

template<typename T>
void Sorter<T>::bubbleSort(T array[],int n){
    for(int i = 0;i < n-1;i++)
        for(int j = i+1;j < n;j++)
            if(array[j-1] > array[j])
                swap(array[j-1],array[j]);
}

template<typename T>
void Sorter<T>::modifyBubbleSort(T array[],int n){
    bool noSwap;
    for(int i = 0;i < n-1;i++){
        noSwap = 1;
        for(int j = i+1;j < n;j++)
            if(array[j-1] > array[j]){
                swap(array[j-1],array[j]);
                noSwap = 0;
            }
        if(noSwap)
            break;
    }
}

template<typename T>
void Sorter<T>::quickSort(T array[],int left,int right){
    if(right <= left)
        return;
    int pivot = Sorter<T>::selectPivot(left,right);
    Sorter<T>::swap(array[pivot],array[right]);
    pivot = Sorter<T>::partition(array,left,right);
    Sorter<T>::quickSort(array,left,pivot-1);
    Sorter<T>::quickSort(array,pivot+1,right);
}

template<typename T>
void Sorter<T>::modQuickSort(T array[],int left,int right){
    if(right-left+1 > QUICKSORT_THRESHOLD){
        int pivot = Sorter<T>::selectPivot(left,right);
        Sorter<T>::swap(array[pivot],array[right]);
        pivot = Sorter<T>::partition(array,left,right);
        Sorter<T>::quickSort(array,left,pivot-1);
        Sorter<T>::quickSort(array,pivot+1,right);
    }
}

template<typename T>
void Sorter<T>::modifyQuickSort(T array[],int n){
    modQuickSort(array,0,n-1);
    insertSort(array,n);
}

template<typename T>
int Sorter<T>::selectPivot(int left,int right){
    return (left+right)/2;
}

template<typename T>
int Sorter<T>::partition(T array[],int left,int right){
    int l = left,r = right;
    T temp = array[r];
    while(l != r){
        while(array[l] <= temp&&r > l)
            l++;
        if(l < r)
            array[r--] = array[l];
        while(array[r] >= temp&&r >l)
            r--;
        if(l < r)
            array[l++] = array[r];
    }
    array[l] = temp;
    return l;
}

template<typename T>
void Sorter<T>::mergeSort(T array[],T tempArray[],int left,int right){
    if(left < right){
        int middle = (left+right)/2;
        Sorter<T>::mergeSort(array,tempArray,left,middle);
        Sorter<T>::mergeSort(array,tempArray,middle+1,right);
        Sorter<T>::merge(array,tempArray,left,right,middle);
    }
}

template<typename T>
void Sorter<T>::merge(T array[],T tempArray[],int left,int right,int middle){
    for(int i = left;i <= right;i++)
        tempArray[i] = array[i];
    int index1 = left,index2 = middle+1;
    int i = left;
    while(index1 <= middle&&index2 <= right){
        if(tempArray[index1] <= tempArray[index2])
            array[i++] = tempArray[index1++];
        else
            array[i++] = tempArray[index2++];
    }
    while(index1 <= middle)
        array[i++] = tempArray[index1++];
    while(index2 <= right)
        array[i++] = tempArray[index2++];
}

template<typename T>
void Sorter<T>::modifyMergeSort(T array[],T tempArray[],int left,int right){
    if(right-left+1 > MERGESORT_THRESHOLD){
        int middle = (left+right)/2;
        Sorter<T>::mergeSort(array,tempArray,left,middle);
        Sorter<T>::mergeSort(array,tempArray,middle+1,right);
        Sorter<T>::merge(array,tempArray,left,right,middle);
    }
    else{
        Sorter<T>::insertSort(array,right-left+1);
    }
}

template<typename T>
void Sorter<T>::modifyMerge(T array[],T tempArray[],int left,int right,int middle){
    for(int i = 0;i <= middle;i++)
        tempArray[i] = array[i];
    for(int i = 1;i <= right-middle;i++)
        tempArray[right-i+1] = array[i+middle];
    for(int index1 = left,index2 = right, i = left;i <= right;i++)
        if(tempArray[index1] < tempArray[index2])
            array[i] = tempArray[index1++];
        else
            array[i] = tempArray[index2--];
}

template<typename T>
void Sorter<T>::bucketSort(T array[],int n,int max){
    T* tempArray = new T[n];
    int* count = new int[max];
    for(int i = 0;i < n;i++)
        tempArray[i] = array[i];
    for(int i = 0;i < max;i++)
        count[i] = 0;
    for(int i = 0;i < n;i++)
        count[array[i]]++;
    for(int i = 1;i < max;i++)
        count[i] = count[i-1]+count[i];
    for(int i = n-1;i >= 0;i--)
        array[--count[tempArray[i]]] = tempArray[i];
    delete[] count;
}

template<typename T>
void Sorter<T>::heapSort(T array[],int n){
    MinHeap<T> mh(array,n);
    for(int i = 0;i < n;i++)
        array[i] = mh.removeMin();
}

template<typename T>
void Sorter<T>::radixSort(T array[],int n,int d,int r){
    T* tempArray = new T[n];
    int* count = new int[r];
    int radix = 1;
    for(int i = 1;i <= d;i++){
        for(int j = 0;j < r;j++)
            count[j] = 0;
        for(int j = 0;j < n;j++){
            int k =(array[j]/radix)%r;
            count[k]++;
        }
        for(int j = 1;j < r;j++)
            count[j] = count[j-1]+count[j];
        for(int j = n-1;j >= 0;j--){
            int k = (array[j]/radix)%r;
            count[k]--;
            tempArray[count[k]] = array[j];
        }
        for(int j = 0;j < n;j++)
            array[j] = tempArray[j];
        radix = radix*r;
    }
    delete[] tempArray;
    delete[] count;
}

