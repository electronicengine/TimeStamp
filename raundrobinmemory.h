#ifndef RAUNDROBINMEMORY_H
#define RAUNDROBINMEMORY_H

#include <QObject>
#include <queue>
#include <deque>
#include "containers.h"

template<typename T>
class RaundRobinMemory
{

    int Total_Size;
    int Current_Queue_Index;
    std::vector<std::vector<T>> Mem_;

public:
    RaundRobinMemory() : Total_Size(0), Current_Queue_Index(0)
    {

    }



    void setSize(int Size)
    {
        Total_Size = Size;

        for(int i=0; i<Total_Size; i++)
              Mem_.emplace_back(std::vector<T>{});
    }



    void pushBack(const T &Container)
    {
        Mem_[Current_Queue_Index].push_back(Container);
    }



    std::vector<T> &getLastQueue()
    {

        std::vector<T> &last_mem = Mem_[Current_Queue_Index++];

        if(Current_Queue_Index >= Total_Size)
            Current_Queue_Index = 0;

        Mem_[Current_Queue_Index].clear();

        return last_mem;

    }


};

#endif // RAUNDROBINMEMORY_H
