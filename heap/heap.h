#pragma once
#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

template <typename T>
class IHeap {
public:
    virtual ~IHeap() = 0;
    
    virtual void insert(T) = 0;
    virtual T getMin() const = 0;
    virtual void extractMin() = 0;
    virtual void meld(IHeap&&) = 0;
    virtual bool empty() const = 0;
};

template <typename T> IHeap<T>::~IHeap() {}

#endif // #ifndef HEAP_H_INCLUDED
