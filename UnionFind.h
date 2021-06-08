#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "DynamicArray.h"
#include "RankAVLTree.h"

#define INVALID_VAL -1

enum UFStatus {UF_SUCCESS,UF_ALLOC_FAIL,UF_FAIL};



/*------------------------------------------------------------------------------
-------------------------------------Union-Find---------------------------------
------------------------------------------------------------------------------*/

/*----------------------------------Singleton---------------------------------*/

class UnionFindSingleton {
public:
    int key;
    int groupSize;
    RankAVLTree<int,int>* group;
    UnionFindSingleton* next = nullptr;

    explicit UnionFindSingleton(int key);
    ~UnionFindSingleton();

 //   UnionFindSingleton& operator=(UnionFindSingleton const &toCopy);

};

UnionFindSingleton::UnionFindSingleton(int key):key(key), groupSize(1), group(new RankAVLTree<int,int>) {

}

UnionFindSingleton::~UnionFindSingleton() {
    delete group;
}

/*UnionFindSingleton &UnionFindSingleton::operator=(UnionFindSingleton const &toCopy) {
    key = toCopy.key;
    groupSize = toCopy.groupSize;
    group = toCopy.group;
    next= toCopy.next;
    return *this;
}*/

/*----------------------------------Union-Find--------------------------------*/

class UnionFind {
    DynamicArray<UnionFindSingleton> agenciesObjects;
public:
    UnionFind() = default;
    ~UnionFind() = default;

    UFStatus insertSingleton();
    UFStatus Union(int key1,int key2);
    int Find(int key);// returns the key of singleton in group containing data
    RankAVLTree<int,int> *getTree(int key);// data of the singleton, null if it doesn't contain
};


/*------------------------------------------------------------------------------
-------------------------Union-Find-Implementations-----------------------------
------------------------------------------------------------------------------*/

/*-------------------------------Help-Functions-------------------------------*/

inline UnionFindSingleton getRoot(UnionFindSingleton s){
    while (s.next){
        s=*s.next;
    }
    return s;
}

inline void compressPath(UnionFindSingleton node,UnionFindSingleton root){
    if(!node.next) return;//group is Singleton
    UnionFindSingleton temp = *node.next;
    do{
       node.next = &root;
       node = temp;
        temp = *temp.next;
    }while (temp.next);
}

/*----------------------------------Singleton---------------------------------*/


/*-------------------------------Union-Find-----------------------------------*/

UFStatus UnionFind::insertSingleton() {
    auto newSingleton = UnionFindSingleton(agenciesObjects.getSize());
    if(agenciesObjects.insertNext(newSingleton) == AS_FAIL)
        return UF_ALLOC_FAIL;
    return UF_SUCCESS;
}

UFStatus UnionFind::Union(int key1, int key2) {
    if(key1==key2)  return UF_SUCCESS;
    //union by size
    UnionFindSingleton singletonSmall = agenciesObjects[key1].groupSize <= agenciesObjects[key2].groupSize ?
                                        agenciesObjects[key1] : agenciesObjects[key2];
    UnionFindSingleton singletonBig = agenciesObjects[key1].groupSize > agenciesObjects[key2].groupSize ?
                                        agenciesObjects[key1] : agenciesObjects[key2];

    singletonSmall = getRoot(singletonSmall);
    singletonBig = getRoot(singletonBig);
    //TODO: combine data


    singletonSmall.next = &singletonBig;
    return UF_SUCCESS;
}

int UnionFind::Find(int key) {
    if(key<0 || key>=agenciesObjects.getSize()) return INVALID_VAL;
    UnionFindSingleton singletonTemp = agenciesObjects[key];
    UnionFindSingleton groupRoot = getRoot(singletonTemp);
    compressPath(singletonTemp,groupRoot);
    return groupRoot.key;
}

RankAVLTree<int,int> *UnionFind::getTree(int key) {
    return agenciesObjects[key].group;
}

#endif //WET2_UNIONFIND_H
