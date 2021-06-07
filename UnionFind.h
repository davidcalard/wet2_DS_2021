#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "DynamicArray.h"

#define INVALID_VAL -1

enum UFStatus {UF_SUCCESS,UF_ALLOC_FAIL,UF_FAIL};

/*------------------------------------------------------------------------------
-------------------------------------Union-Find---------------------------------
------------------------------------------------------------------------------*/

/*----------------------------------Singleton---------------------------------*/

template <class Data>
class UnionFindSingleton {
public:
    int key;
    Data group;
    UnionFindSingleton* next = nullptr;


    UnionFindSingleton(int key) = default;
    ~UnionFindSingleton() = default;

};

/*----------------------------------Union-Find--------------------------------*/



template<class Data> //key == int
class UnionFind {
    DynamicArray<UnionFindSingleton<Data>>* agenciesObjects;
public:
    UnionFind() = default;
    ~UnionFind() = default;

    UFStatus insertSingleton(Data& data);
    UFStatus Union(int key1,int key2);
    int Find(int key);// returns the key of singleton in group containing data
    Data getData(int key);// data of the singleton, null if it doesn't contain
};


/*------------------------------------------------------------------------------
-------------------------Union-Find-Implementations-----------------------------
------------------------------------------------------------------------------*/

/*----------------------------------Singleton---------------------------------*/


/*-------------------------------Union-Find-----------------------------------*/

template<class Data>
UFStatus UnionFind<Data>::insertSingleton(Data &data) {
    if(agenciesObjects->insertNext(UnionFindSingleton<Data>(agenciesObjects->getSize())) == AS_FAIL)
        return UF_ALLOC_FAIL;
    return UF_SUCCESS;
}

template<class Data>
UFStatus UnionFind<Data>::Union(int key1, int key2) {
    if(key1==key2)  return UF_SUCCESS;
    //union by size
    UnionFindSingleton<Data> singletonSmall = key1 < key2 ? agenciesObjects[key1] : agenciesObjects [key2];
    UnionFindSingleton<Data> singletonBig = key1 > key2 ? agenciesObjects[key1] : agenciesObjects [key2];


    if (singletonSmall.next) //to get the data of the group
        singletonSmall = singletonSmall.next;
    if (singletonBig.next)  //to get the data of the group
        singletonBig = singletonBig.next;
    //TODO: combine data


    singletonSmall.next = singletonBig;
    return UF_SUCCESS;
}

template<class Data>
int UnionFind<Data>::Find(int key) {
    UnionFindSingleton<Data> singletonTemp = agenciesObjects[key];
    if(!singletonTemp)   return INVALID_VAL;
    while(singletonTemp.next){
        singletonTemp = singletonTemp.next;
    }
    return singletonTemp.key;
}

template<class Data>
Data UnionFind<Data>::getData(int key) {
    return agenciesObjects[key]->group;
}

#endif //WET2_UNIONFIND_H
