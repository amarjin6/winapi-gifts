#ifndef CONCURRENT_LIST_H

#define CONCURRENT_LIST_H



#include "ConcurrentListNode.h"

#include "ConcurrentListReader.h"

#include <stdexcept>



template <typename T>

class ConcurrentList

{

public:

    ConcurrentList();

    ~ConcurrentList();



    void pushBack(T value);

    T popBack();

    ConcurrentListReader<T>* createReader();

    void tmp();

    int count();



private:

    int _count;

    long numberOfReaders;

    CRITICAL_SECTION noReadersCrit;

    CONDITION_VARIABLE noReadersVariable;



    volatile long isWriting;

    CRITICAL_SECTION isWritingCrit;

    CONDITION_VARIABLE isWritingVariable;



    ConcurrentListNode<T>* head;

    ConcurrentListNode<T>* tail;

};



template<typename T>

inline ConcurrentList<T>::ConcurrentList()

{

    this->head = nullptr;

    this->tail = nullptr;

    this->_count = 0;



    this->numberOfReaders = 0;

    InitializeConditionVariable(&noReadersVariable);

    InitializeCriticalSection(&noReadersCrit);



    this->isWriting = 0;

    InitializeConditionVariable(&this->isWritingVariable);

    InitializeCriticalSection(&this->isWritingCrit);

}



template<typename T>

inline ConcurrentList<T>::~ConcurrentList()

{



}



template<typename T>

inline void ConcurrentList<T>::pushBack(T value)

{

    EnterCriticalSection(&this->noReadersCrit);

    while (this->numberOfReaders != 0)

        SleepConditionVariableCS(&noReadersVariable, &noReadersCrit, INFINITE);



    auto node = new ConcurrentListNode<T>(value);



    if (this->tail == nullptr)

    {

        this->tail = node;

        this->head = node;

    }

    else

    {

        node->prev = this->tail;

        this->tail->next = node;

        this->tail = node;

    }



    this->_count++;

    LeaveCriticalSection(&this->noReadersCrit);

}



template<typename T>

inline T ConcurrentList<T>::popBack()

{

    EnterCriticalSection(this->noReadersCrit);

    while (this->numberOfReaders != 0)

        SleepConditionVariableCS(&noReadersVariable, &noReadersCrit, INFINITE);



    if (this->tail == nullptr)

    {

        LeaveCriticalSection(this->noReadersCrit);

        throw std::runtime_error("This list is empty");

    }



    T res = this->tail->value;

    auto newTail = this->tail->prev;



    if (newTail == nullptr)

        this->head = nullptr;

    else

        newTail->next = nullptr;



    delete(this->tail);

    this->tail = newTail;

    this->_count--;



    LeaveCriticalSection(this->noReadersCrit);

    return res;

}





template<typename T>

inline ConcurrentListReader<T>* ConcurrentList<T>::createReader()

{

    InterlockedIncrement(&numberOfReaders);

    return new ConcurrentListReader<T>(head, &this->noReadersVariable, &this->numberOfReaders);

}



template<typename T>

inline void ConcurrentList<T>::tmp()

{

    InterlockedDecrement(&this->numberOfReaders);



    if (this->numberOfReaders == 0)

        WakeAllConditionVariable(&this->noReadersVariable);

}



template<typename T>

inline int ConcurrentList<T>::count()

{

    return this->_count;

}



#endif