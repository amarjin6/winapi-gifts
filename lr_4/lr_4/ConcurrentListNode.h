#ifndef CONCURRENT_LIST_NODE_H

#define CONCURRENT_LIST_NODE_H



#include <windows.h>



template <typename T>

class ConcurrentListNode

{

public:

    ConcurrentListNode(T value);

    ~ConcurrentListNode();



    T value;

    ConcurrentListNode* prev;

    ConcurrentListNode* next;

};



template<typename T>

inline ConcurrentListNode<T>::ConcurrentListNode(T value)

    : value(value)

{

    this->prev = nullptr;

    this->next = nullptr;

}



template<typename T>

inline ConcurrentListNode<T>::~ConcurrentListNode()

{



}



#endif