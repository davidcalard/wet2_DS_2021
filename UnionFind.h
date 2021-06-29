


#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "DynamicArray.h"
#include "RankAVLTree.h"

#define INVALID_VAL -1
#define NO_NEXT -1

enum UFStatus {UF_SUCCESS,UF_ALLOC_FAIL,UF_FAIL};
typedef int carID, Sales;


/*------------------------------------------------------------------------------
-------------------------------------Union-Find---------------------------------
------------------------------------------------------------------------------*/

/*----------------------------------Singleton---------------------------------*/
class UnionFindSingleton {
public:
    int key;
    int groupSize;
    int nextIndex = NO_NEXT;
    RankAVLTree<carID ,RankAVLNode<Sales,carID>*>* group;
    RankAVLTree<Sales,carID>* sales;
    RankAVLNode<Sales,carID>* min = nullptr;

    explicit UnionFindSingleton(int key);
    UnionFindSingleton(const UnionFindSingleton& toCopy);
    UnionFindSingleton()=default;
    ~UnionFindSingleton();


};

UnionFindSingleton::UnionFindSingleton(int key):key(key), groupSize(1),
group(new RankAVLTree<carID ,RankAVLNode<Sales,carID>*>),sales(new RankAVLTree<Sales,carID>) {

}

UnionFindSingleton::UnionFindSingleton(const UnionFindSingleton &toCopy):key(toCopy.key), groupSize(toCopy.groupSize),
                    nextIndex(toCopy.nextIndex),group(nullptr),sales(nullptr),min(toCopy.min){
}

UnionFindSingleton::~UnionFindSingleton() {
    delete group;
    delete sales;
}


/*----------------------------------Union-Find--------------------------------*/

class UnionFind {
public:
    DynamicArray<UnionFindSingleton*> agenciesObjects;
    UnionFind() = default;
    ~UnionFind();

    UFStatus insertSingleton();
    UFStatus Union(int key1,int key2);
    UFStatus clear();
    int Find(int key);// returns the key of singleton in group containing data
    RankAVLTree<carID ,RankAVLNode<Sales,carID>*>* getTree(int key);// data of the singleton, null if it doesn't contain
    UnionFindSingleton &getSingleton(int key);
};


/*------------------------------------------------------------------------------
-------------------------Union-Find-Implementations-----------------------------
------------------------------------------------------------------------------*/

/*-------------------------------Help-Functions-------------------------------*/

inline UnionFindSingleton &goToRoot(UnionFind &UF,UnionFindSingleton &s){
    int rtnIndex = s.key;
    while (UF.agenciesObjects[rtnIndex]->nextIndex!=NO_NEXT){
        rtnIndex = UF.agenciesObjects[rtnIndex]->nextIndex;
    }
    return *UF.agenciesObjects[rtnIndex];
}

inline void compressPath(UnionFind &UF,UnionFindSingleton &node,UnionFindSingleton &root){
    int index = node.key, temp = node.nextIndex;
    while (temp != NO_NEXT){
    UF.agenciesObjects[index]->nextIndex = root.key;
    index = temp;
    temp = UF.agenciesObjects[temp]->nextIndex;
    }
}

/*----------------------------------Singleton---------------------------------*/


/*-------------------------------Union-Find-----------------------------------*/

UFStatus UnionFind::insertSingleton() {
    auto newSingleton = new UnionFindSingleton(agenciesObjects.getSize());
    if(agenciesObjects.insertNext(newSingleton) == AS_FAIL)
        throw std::bad_alloc();
    return UF_SUCCESS;
}

UFStatus UnionFind::Union(int key1, int key2) {
    if(key1==key2)  return UF_SUCCESS;
    //union by size
    UnionFindSingleton &singletonSmall = agenciesObjects[key1]->groupSize <= agenciesObjects[key2]->groupSize ?
                                        *agenciesObjects[key1] : *agenciesObjects[key2];
    UnionFindSingleton &singletonBig = agenciesObjects[key1]->groupSize > agenciesObjects[key2]->groupSize ?
                                        *agenciesObjects[key1] : *agenciesObjects[key2];

    singletonSmall = goToRoot(*this,singletonSmall);
    singletonBig = goToRoot(*this,singletonBig);
    RankAVLTree<carID ,RankAVLNode<Sales,carID>*>* bigTree = singletonBig.group;
    RankAVLTree<carID ,RankAVLNode<Sales,carID>*>* smallTree = singletonSmall.group;
    singletonBig.group = new RankAVLTree<carID ,RankAVLNode<Sales,carID>*>(bigTree->root,smallTree->root);
    if(!singletonBig.group) throw std::bad_alloc();
    delete bigTree;
    RankAVLTree<Sales,carID>* bigSTree = singletonBig.sales;
    RankAVLTree<Sales,carID>* smallSTree = singletonSmall.sales;
    singletonBig.sales = new RankAVLTree<Sales,carID>(bigSTree->root,smallSTree->root);
    if(!singletonBig.sales) throw std::bad_alloc();
    ///updating the min for the new group
    if(singletonSmall.min== nullptr);
    else if(singletonBig.min== nullptr)singletonBig.min = singletonBig.sales->findExactNode(singletonSmall.min->key,singletonSmall.min->data);
    else if(singletonBig.min->key > singletonSmall.min->key ||
        (singletonBig.min->key == singletonSmall.min->key && singletonBig.min->data > singletonSmall.min->data)) {
        singletonBig.min = singletonBig.sales->findExactNode(singletonSmall.min->key,singletonSmall.min->data);
    }else{
        singletonBig.min = singletonBig.sales->findExactNode(singletonBig.min->key,singletonBig.min->data);
    }
    /*problem with destruction*/delete bigSTree;
    singletonSmall.nextIndex = singletonBig.key;
    singletonBig.groupSize = singletonSmall.groupSize+singletonBig.groupSize;
    return UF_SUCCESS;
}

int UnionFind::Find(int key) {
    if(key<0 || key>=agenciesObjects.getSize()) return INVALID_VAL;
    UnionFindSingleton &singletonTemp = *agenciesObjects[key];
    UnionFindSingleton &groupRoot = goToRoot(*this,singletonTemp);
    compressPath(*this,singletonTemp,groupRoot);
    return groupRoot.key;
}

RankAVLTree<carID ,RankAVLNode<Sales,carID>*>* UnionFind::getTree(int key) {
    return agenciesObjects[key]->group;
}

UnionFindSingleton& UnionFind::getSingleton(int key) {//assume key is legal
    return *agenciesObjects[key];
}

UFStatus UnionFind::clear() {
    for(int i=0;i<agenciesObjects.getSize();i++){
        delete agenciesObjects[i];
    }
    return UF_SUCCESS;
}

UnionFind::~UnionFind() {
    clear();
}

#endif //WET2_UNIONFIND_H
