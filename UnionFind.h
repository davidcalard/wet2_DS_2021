


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

    UnionFindSingleton &singletonSmallRoot = goToRoot(*this, singletonSmall);
    UnionFindSingleton &singletonBigRoot = goToRoot(*this, singletonBig);
    RankAVLTree<carID ,RankAVLNode<Sales,carID>*>* bigTree = singletonBigRoot.group;
    RankAVLTree<carID ,RankAVLNode<Sales,carID>*>* smallTree = singletonSmallRoot.group;
    singletonBigRoot.group = new RankAVLTree<carID ,RankAVLNode<Sales,carID>*>(bigTree->root, smallTree->root);
    if(!singletonBigRoot.group) throw std::bad_alloc();
    delete bigTree;
    RankAVLTree<Sales,carID>* bigSTree = singletonBigRoot.sales;
    RankAVLTree<Sales,carID>* smallSTree = singletonSmallRoot.sales;
    singletonBigRoot.sales = new RankAVLTree<Sales,carID>(bigSTree->root, smallSTree->root);
    InOrderGroup(singletonBigRoot.group->root,singletonBigRoot.sales);
    if(!singletonBigRoot.sales) throw std::bad_alloc();
    ///updating the min for the new group
    if(singletonSmallRoot.min == nullptr);
    else if(singletonBigRoot.min == nullptr)singletonBigRoot.min = singletonBigRoot.sales->findExactNode(singletonSmallRoot.min->key, singletonSmallRoot.min->data);
    else if(singletonBigRoot.min->key > singletonSmallRoot.min->key ||
            (singletonBigRoot.min->key == singletonSmallRoot.min->key && singletonBigRoot.min->data > singletonSmallRoot.min->data)) {
        singletonBigRoot.min = singletonBigRoot.sales->findExactNode(singletonSmallRoot.min->key, singletonSmallRoot.min->data);
    }else{
        singletonBigRoot.min = singletonBigRoot.sales->findExactNode(singletonBigRoot.min->key, singletonBigRoot.min->data);
    }
    delete bigSTree;
    singletonSmallRoot.nextIndex = singletonBigRoot.key;
    singletonSmall.groupSize = singletonSmall.groupSize+singletonBig.groupSize;
    singletonBigRoot.groupSize = singletonSmallRoot.groupSize + singletonBigRoot.groupSize;
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
