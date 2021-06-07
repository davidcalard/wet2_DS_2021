//
// Created by omerg on 06/06/2021.
//

#ifndef DS_WET2_RANKAVLTREE_H
#define DS_WET2_RANKAVLTREE_H

#include <iostream>
#include <ostream>
#include <algorithm>
#include "OrderedList.h"

using std::max;
using std::endl;
using std::cout;

enum RTreeStatus {R_T_SUCCESS,R_T_ALLOC_ERROR,R_T_FAIL,R_DONTKNOW};
/*------------------------------------------------------------------------------
------------------------------------RankAVLNode-------------------------------------
------------------------------------------------------------------------------*/

template <class Key, class Data>
class RankAVLNode
{
public:
    Key key;
    Data data;
    int height; //height of the tree that this node is the root
    int bf; //balance factor
    int numOfNodes; //number of node in a sub tree that this node is the root //TODO

    RankAVLNode *parent;
    RankAVLNode *left;
    RankAVLNode *right;

    RankAVLNode(Key k, Data d); //constructor
    RankAVLNode(const RankAVLNode<Key,Data>&); //copy constructor
    ~RankAVLNode(); //destructor

    bool isLeaf();
    void fillTree(OrderedList<RankAVLNode<Key,Data>>& list, int* num);
    void deleteForComplete(int* num_toDel);
    int getNumOfNodes(RankAVLNode<Key,Data>* node){
        if(node== nullptr)return 0;
        return node->numOfNodes;
    }
    bool operator<(const RankAVLNode<Key,Data>& node){
        return (this->data < node.data);
    }
    bool operator>(const RankAVLNode<Key,Data>& node){
        return (this->data > node.data);
    }
    bool operator==(const RankAVLNode<Key,Data>& node){
        return (this->data == node.data);
    }
};

/*------------------------------------------------------------------------------
------------------------------------RankAVLTree-------------------------------------
------------------------------------------------------------------------------*/

template <class Key, class Data>
class RankAVLTree
{
public:
    RankAVLNode<Key,Data> *root = nullptr;
    //RankAVLNode<Key,Data> *lastInOrder = nullptr;


    RankAVLTree(); //constructor
    ~RankAVLTree(); //destructor
    //insert Node into the right place in the tree while maintain the balance
    RTreeStatus insertNode(Key k, Data d);
    //remove Node from the tree while maintain the balance
    RTreeStatus removeNode(Key k);
    //find Node from the tree by key
    RankAVLNode<Key,Data> *findNode(Key k);
    //updates root tree
    void updateRoot(RankAVLNode<Key,Data> *n);

    //rotations
    void LLrotate(RankAVLNode<Key,Data> *n);
    void LRrotate(RankAVLNode<Key,Data> *n);
    void RRrotate(RankAVLNode<Key,Data> *n);
    void RLrotate(RankAVLNode<Key,Data> *n);

    //delete all nodes of the tree
    void clearTree();
    //print all nodes of the tree in ascending order
    void printTree();

    //find a node with index - rank
    RankAVLNode<Key,Data> *Select(int rank);

    //help to make almost complete tree
    RankAVLNode<Key,Data>* completeTree(int hight);
    int nodes2del(int nodes_num);
    void getCompleteTree(int nodes_num);

    //combine 2 rank trees
    RankAVLTree(RankAVLNode<Key,Data>* root1, RankAVLNode<Key,Data>* root2);
    void inOrder(RankAVLNode<Key,Data>* root,OrderedList<RankAVLNode<Key,Data>>& list,
            void (*func)(OrderedList<RankAVLNode<Key,Data>>&,RankAVLNode<Key,Data>& node));
};




/*------------------------------------------------------------------------------
-----------------------------RankAVLNode-Implementation-----------------------------
------------------------------------------------------------------------------*/

template <class Key, class Data>
RankAVLNode<Key, Data>::RankAVLNode(Key k, Data d): key(k), data(d), height(0), bf(0), numOfNodes(1), //TODO
                                                    parent(nullptr), left(nullptr), right(nullptr){}

/*----------------------------------------------------------------------------*/

template <class Key, class Data>
RankAVLNode<Key, Data>::~RankAVLNode(){}


/*-----for complete tree-------*/
template<class Key, class Data>
bool RankAVLNode<Key, Data>::isLeaf(){
    return (this->right== nullptr && this->left== nullptr);
}


template<class Key, class Data>
void RankAVLNode<Key, Data>::fillTree(OrderedList<RankAVLNode<Key,Data>>& list, int* num){
    if(this== nullptr|| (*num)<0)return;
    this->right->fillTree(list,num);
    (*num)-=1;
    this->key=list.end->object.key;
    this->data=list.end->object.data;
    list.remove(list.end);
    this->left->fillTree(list,num);
    this->numOfNodes =1+ this->getNumOfNodes(left)+ this->getNumOfNodes(right);
}

template<class Key, class Data>
void RankAVLNode<Key, Data>::deleteForComplete(int* num_toDel) {
    if(this == nullptr ||  (*num_toDel)<=0)return;
    this->right->deleteForComplete(num_toDel);
    if(this->isLeaf()) {
        (*num_toDel) -= 1;
        if (this->parent->right != nullptr) {
            this->parent->right=nullptr;
            delete this;
        } else {
            this->parent->left=nullptr;
            delete this;
        }
        return;
    }
    this->left->deleteForComplete(num_toDel);
}

template<class Key, class Data>
RankAVLNode<Key, Data>::RankAVLNode(const RankAVLNode<Key,Data>& n):key(n.key),data(n.data)
,height(0),bf(0),numOfNodes(0),parent(nullptr),left(nullptr),right(nullptr){}

/*------------------------------------------------------------------------------
-----------------------------RankAVLTree-Implementation-----------------------------
------------------------------------------------------------------------------*/

template <class Key, class Data>
RankAVLTree<Key, Data>::RankAVLTree(): root(nullptr) {}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
RankAVLTree<Key, Data>::~RankAVLTree(){
    this->clearTree();
}
/*---------------for complete tree----------------------------*/
int hightByNodes(int nodes_num){
    int log2n=0,tmp=nodes_num;
    while (tmp!=0){
        tmp=tmp/2;
        log2n++;
    }
    return log2n;
}


template<class Key, class Data>
RankAVLNode<Key,Data>* RankAVLTree<Key, Data>::completeTree(int hight){
    if(hight==0)return nullptr;

    if(hight<0)return nullptr;
    auto avLnode= new RankAVLNode<int,int>(hight,hight);

    RankAVLNode<Key,Data>* tmp_right= completeTree(hight-1);
    avLnode->right=tmp_right;
    if(tmp_right!= nullptr)tmp_right->parent=avLnode;

    RankAVLNode<Key,Data>* tmp_left= completeTree(hight-1) ;
    avLnode->left=tmp_left;
    if(tmp_left!= nullptr)tmp_left->parent=avLnode;

    avLnode->height=hight;
    return avLnode;
}

template<class Key, class Data>
int RankAVLTree<Key, Data>::nodes2del(int nodes_num){
    if(nodes_num<=0)return nodes_num;
    int hight = root->height, n=1;
    while (hight!=0){
        n*=2;
        hight--;
    }
    n-=1;
    return n-nodes_num;
}

template<class Key, class Data>
void RankAVLTree<Key, Data>::getCompleteTree(int nodes_num){
    this->root=completeTree(hightByNodes(nodes_num));
    int n2del= this->nodes2del(nodes_num);
    root->deleteForComplete(&n2del);
}




/*----------------------------------------------------------------------------*/

template<class Key, class Data>
RTreeStatus RankAVLTree<Key, Data>::insertNode(Key k, Data d){
    if (this->root == nullptr){ //tree is empty
        this->root = new RankAVLNode<Key, Data> (k, d);
        if(!this->root) return R_T_ALLOC_ERROR;
        return R_T_SUCCESS;
    }

    RankAVLNode<Key, Data> *sub_root = this->root;
    while (sub_root != nullptr){
        if(k < sub_root->key){
            if (sub_root->left != nullptr)  sub_root = sub_root->left;
            else{
                sub_root->left = new RankAVLNode<Key, Data> (k, d);
                if(!sub_root->left) return R_T_ALLOC_ERROR;
                sub_root->left->parent = sub_root; //update parent of new node
                updateNodeHeight(sub_root->left);
                updateNodeBF(sub_root);
                updateNumOfNodes(sub_root); //TODO
                rebalanceTreeAfterInsert(this, sub_root->left);
                return R_T_SUCCESS;
            }
        }

        else if(k > sub_root->key){
            if (sub_root->right != nullptr)     sub_root = sub_root->right;
            else{
                sub_root->right = new RankAVLNode<Key, Data> (k, d);
                if(!sub_root->right) return R_T_ALLOC_ERROR;
                sub_root->right->parent = sub_root; //update parent of new node
                updateNodeHeight(sub_root->right);
                updateNodeBF(sub_root);
                updateNumOfNodes(sub_root); //TODO
                rebalanceTreeAfterInsert(this, sub_root->right);
                return R_T_SUCCESS;
            }
        }

        else return R_T_FAIL; //k is already exist in the tree
    }
    return R_T_FAIL; //should not get here
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
RTreeStatus RankAVLTree<Key, Data>::removeNode(Key k){
    RankAVLNode<Key, Data> *toRemove = findNode(k);
    if (toRemove == nullptr) return R_T_FAIL; //k does not exist in the tree
    if (isLeaf(toRemove)){
        leafRemove(this, toRemove);
        return R_T_SUCCESS;
    }
    if (toRemove->left == nullptr && toRemove->right != nullptr){
        onlyRightSonRemove(this, toRemove);
        return R_T_SUCCESS;
    }
    if (toRemove->left != nullptr && toRemove->right == nullptr){
        onlyLeftSonRemove(this, toRemove);
        return R_T_SUCCESS;
    }
    else{ //toRemove->left != nullptr && toRemove->right != nullptr
        RankAVLNode<Key, Data> *nextNode = findNextNode(toRemove);
        swapNodes(toRemove, nextNode);
        if(toRemove == this->root){
            nextNode->parent = nullptr;
            this->root = nextNode; //update root
        }
        //now k is a leaf or has only one son
        if (isLeaf(toRemove)){
            leafRemove(this, toRemove);
            return R_T_SUCCESS;
        }
        if (toRemove->left == nullptr && toRemove->right != nullptr){
            onlyRightSonRemove(this, toRemove);
            return R_T_SUCCESS;
        }
        if (toRemove->left != nullptr && toRemove->right == nullptr){
            onlyLeftSonRemove(this, toRemove);
            return R_T_SUCCESS;
        }return R_DONTKNOW;
    }
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
RankAVLNode<Key, Data> *RankAVLTree<Key, Data>::findNode(Key k)
{
    RankAVLNode<Key, Data> *candidate = this->root;
    while (candidate != nullptr){
        if (k < candidate->key)     candidate = candidate->left;
        else if (k > candidate->key)     candidate = candidate->right;
        else    return candidate; // k == candidate->key
    }return nullptr; //k is not found in the tree
}
/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void RankAVLTree<Key, Data>::updateRoot(RankAVLNode<Key, Data> *n) {
    this->root = n;
    n->parent = nullptr;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void RankAVLTree<Key, Data>::LLrotate(RankAVLNode<Key, Data> *n){
    RankAVLNode<Key,Data> *temp=n->left;
    if(isRoot(n))   updateRoot(temp);
    else {
        temp->parent = n->parent;
        if (isLeftSon(n)) n->parent->left = temp;
        else n->parent->right = temp;
    }

    n->left = temp->right;
    if (temp->right != nullptr) temp->right->parent = n;

    temp->right = n;
    n->parent = temp;
    updateNodeHeight(n); //update changed heights
    updateNodeBF(n); //update relevant BF's
    updateNodeBF(temp);

    updateNumOfNodesForOneNode(n); //TODO
    updateNumOfNodesForOneNode(temp); //TODO

    return;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void RankAVLTree<Key, Data>::LRrotate(RankAVLNode<Key, Data> *n){
    RankAVLNode<Key,Data> *tempA = n->left;
    RankAVLNode<Key,Data> *tempB = tempA->right;
    if(isRoot(n))   updateRoot(tempB);
    else{
        tempB->parent = n->parent;
        if (isLeftSon(n)) n->parent->left = tempB;
        else n->parent->right = tempB;
    }
    if(!isLeaf(tempB)){
        n->left = tempB->right;
        if (tempB->right != nullptr) tempB->right->parent = n;
        tempA->right = tempB->left;
        if (tempB->left != nullptr) tempB->left->parent = tempA;
    }else{
        n->left = nullptr;
        tempA->right = nullptr;
    }
    tempB->right = n;
    n->parent = tempB;
    tempB->left = tempA;
    tempA->parent = tempB;
    n->height = n->height-2; //update changed heights
    tempA->height = tempA->height-1;
    tempB->height = tempB->height+1;
    updateNodeBF(n); //update relevant BF's
    updateNodeBF(tempA);
    updateNodeBF(tempB);

    updateNumOfNodesForOneNode(n); //TODO
    updateNumOfNodesForOneNode(tempA); //TODO
    updateNumOfNodesForOneNode(tempB); //TODO

    return;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void RankAVLTree<Key, Data>::RRrotate(RankAVLNode<Key, Data> *n){
    RankAVLNode<Key,Data> *temp=n->right;
    if(isRoot(n))   updateRoot(temp);
    else{
        temp->parent = n->parent;
        if(isRightSon(n))    n->parent->right = temp;
        else                n->parent->left = temp;
    }

    n->right = temp->left;
    if (temp->left != nullptr) temp->left->parent = n;

    temp->left = n;
    n->parent = temp;
    updateNodeHeight(n); //update changed heights
    updateNodeBF(n); //update relevant BF's
    updateNodeBF(temp);

    updateNumOfNodesForOneNode(n); //TODO
    updateNumOfNodesForOneNode(temp); //TODO

    return;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void RankAVLTree<Key, Data>::RLrotate(RankAVLNode<Key, Data> *n) {
    RankAVLNode<Key, Data> *tempA = n->right;
    RankAVLNode<Key, Data> *tempB = tempA->left;
    if (isRoot(n)) updateRoot(tempB);
    else {
        tempB->parent = n->parent;
        if (isRightSon(n)) n->parent->right = tempB;
        else n->parent->left = tempB;
    }
    if (!isLeaf(tempB)){
        n->right = tempB->left;
        if (tempB->left != nullptr) tempB->left->parent = n;
        tempA->left = tempB->right;
        if (tempA->left != nullptr) tempB->right->parent = tempA;
    }
    else{
        n->right = nullptr;
        tempA->left = nullptr;
    }

    tempB->left = n;
    n->parent = tempB;
    tempB->right = tempA;
    tempA->parent = tempB;
    n->height = n->height-2;        //update changed heights
    tempA->height = tempA->height-1;
    tempB->height = tempB->height+1;
    updateNodeBF(n);                //update relevant BF's
    updateNodeBF(tempA);
    updateNodeBF(tempB);

    updateNumOfNodesForOneNode(n); //TODO
    updateNumOfNodesForOneNode(tempA); //TODO
    updateNumOfNodesForOneNode(tempB); //TODO

    return;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void RankAVLTree<Key, Data>::clearTree(){
    if (this->root == nullptr) return;

    if (this->root->left != nullptr){
        clearSubTree(this->root->left);
        this->root->left = nullptr;
    }

    if (this->root->right != nullptr){
        clearSubTree(this->root->right);
        this->root->right = nullptr;
    }

    delete this->root;
    this->root = nullptr;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void RankAVLTree<Key, Data>::printTree()
{
    if (this->root == nullptr) return;

    if (this->root->left != nullptr){
        printSubTree(this->root->left);
    }

    cout<<" KEY: "<<this->root->key<<"ST: "<<root->numOfNodes<<endl;

    if (this->root->right != nullptr){
        printSubTree(this->root->right);
    }
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
RankAVLNode<Key, Data>* RankAVLTree<Key, Data>::Select(int rank) {
    return SelectSubTree(this->root, rank);
}

template<class T>
void outAdd(OrderedList<T>& list, T& obj){
    auto node=new OrderedListNode<T>(obj, list.end, nullptr);
    if(list.end!= nullptr)list.end->updateNext(node);
    list.end=node;
    if(list.start== nullptr)list.start=node;
    list.len++;
}

template<class Key, class Data>
RankAVLTree<Key, Data>::RankAVLTree(RankAVLNode<Key, Data> *root1, RankAVLNode<Key, Data> *root2):
root(nullptr){
    if(root1== nullptr && root2== nullptr)return ;
    if(root1== nullptr)return ;
    if(root2== nullptr)return ;

    int sum_nodes=root1->numOfNodes+root2->numOfNodes;
    this->getCompleteTree(sum_nodes);
    auto list1=OrderedList<RankAVLNode<Data,Key>>();
    auto list2=OrderedList<RankAVLNode<Data,Key>>();
    this->inOrder(root1,list1,outAdd);
    this->inOrder(root2,list2,outAdd);

    auto list=OrderedList<RankAVLNode<Key,Data>>(list1,list2);
    root->fillTree(list,&sum_nodes);
}

template<class Key, class Data>
void RankAVLTree<Key, Data>::inOrder(RankAVLNode<Key, Data> *root,OrderedList<RankAVLNode<Key,Data>>& list,
        void (*func)(OrderedList<RankAVLNode<Key,Data>>& ,RankAVLNode<Key, Data>&)){
    if(root== nullptr)return;
    this->inOrder(root->left,list,func);
    func(list,(*root));
    this->inOrder(root->right,list,func);
}


/*------------------------------------------------------------------------------
--------------------------------static functions--------------------------------
------------------------------------------------------------------------------*/

template<class Key, class Data>
static RankAVLNode<Key, Data>* SelectSubTree(RankAVLNode<Key,Data> *n, int rank) {
    if (isLeaf(n)){
        return n;
    }

    if (n->left == nullptr){
        int new_rank = rank - 1;
        if(!new_rank) return n;
        return SelectSubTree(n->right, new_rank);
    }

    else if (n->left != nullptr && n->left->numOfNodes == rank-1){
        return n;
    }

    else if (n->left != nullptr && n->left->numOfNodes > rank-1){
        return SelectSubTree(n->left, rank);
    }

    else if (n->left != nullptr && n->left->numOfNodes < rank-1){
        int new_rank = rank - (n->left->numOfNodes) - 1;
        return SelectSubTree(n->right, new_rank);
    }
    return nullptr;//will never get here
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void rebalanceTreeAfterInsert(RankAVLTree<Key,Data> *tree, RankAVLNode<Key,Data> *n)
{
    while (!isRoot(n)){
        if (n->parent->height>=n->height+1) return;

        updateNodeHeight(n->parent);
        updateNodeBF(n->parent);
        updateNumOfNodes(n->parent); //TODO: maybe no need

        if (n->parent->bf == 2 && n->parent->left != nullptr
            && n->parent->left->bf >= 0){
            tree->LLrotate(n->parent);
            return;
        }
        if (n->parent->bf == 2 && n->parent->left != nullptr
            && n->parent->left->bf == -1){
            tree->LRrotate(n->parent);
            return;
        }
        if (n->parent->bf == -2 && n->parent->right != nullptr
            && n->parent->right->bf == 1){
            tree->RLrotate(n->parent);
            return;
        }
        if (n->parent->bf == -2 && n->parent->right != nullptr
            && n->parent->right->bf <= 0){
            tree->RRrotate(n->parent);
            return;
        }
        n = n->parent;
    }
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
void rebalanceTreeAfterRemove(RankAVLTree<Key,Data> *tree, RankAVLNode<Key,Data> *n)
{
    while (n != nullptr){
        int old_n_height = n->height;

        if (n->bf == 2 && n->left != nullptr
            && n->left->bf >= 0){
            tree->LLrotate(n);
            if (old_n_height == n->height)  return;
        }
        if (n->bf == 2 && n->left != nullptr
            && n->left->bf == -1){
            tree->LRrotate(n);
            if (old_n_height == n->height)  return;
        }
        if (n->bf == -2 && n->right != nullptr
            && n->right->bf == 1){
            tree->RLrotate(n);
            if (old_n_height == n->height)  return;
        }
        if (n->bf == -2 && n->right != nullptr
            && n->right->bf <= 0){
            tree->RRrotate(n);
            if (old_n_height == n->height)  return;
        }
        n = n->parent;
        if (n != nullptr){
            updateNodeHeight(n);
            updateNodeBF(n);
            updateNumOfNodes(n); //TODO: maybe no need
        }
    }
}

/*----------------------------------------------------------------------------*/

//check if n is a root
template<class Key, class Data>
static bool isRoot (RankAVLNode<Key, Data> *n)
{
    return (n->parent == nullptr);
}

/*----------------------------------------------------------------------------*/

//check if n is a leaf
template<class Key, class Data>
static bool isLeaf (RankAVLNode<Key, Data> *n)
{
    return (n->left == nullptr && n->right == nullptr);
}

/*----------------------------------------------------------------------------*/

//check if node is the left son of his parent
template<class Key, class Data>
static bool isLeftSon (RankAVLNode<Key, Data> *n)
{
    return (!isRoot(n) && n->parent->left == n);
}

/*----------------------------------------------------------------------------*/

//check if node is the right son of his parent
template<class Key, class Data>
static bool isRightSon (RankAVLNode<Key, Data> *n)
{
    return (!isRoot(n) && n->parent->right == n);
}

/*----------------------------------------------------------------------------*/

//remove a leaf node
template<class Key, class Data>
static void leafRemove(RankAVLTree<Key, Data> *tree, RankAVLNode<Key, Data> *toRemove)
{
    if (isRoot(toRemove)){
        tree->root = nullptr;
        delete(toRemove);
        return;
    }

    else{
        if (isLeftSon(toRemove))    toRemove->parent->left = nullptr;

        if (isRightSon(toRemove))   toRemove->parent->right = nullptr;

        RankAVLNode<Key, Data> *to_rebalance = toRemove->parent;
        delete(toRemove);
        updateNumOfNodes(to_rebalance); //TODO
        while (to_rebalance) {
            updateNodeHeight(to_rebalance);
            updateNodeBF(to_rebalance);
            rebalanceTreeAfterRemove(tree, to_rebalance);
            to_rebalance = to_rebalance->parent;
        }
    }
}

/*----------------------------------------------------------------------------*/

//remove node that has only right son
template<class Key, class Data>
static void onlyRightSonRemove(RankAVLTree<Key, Data> *tree,
                               RankAVLNode<Key,Data> *toRemove){
    if (isRoot(toRemove)){
        tree->updateRoot(toRemove->right);
        delete(toRemove);
        return;
    }

    else{
        if (isLeftSon(toRemove)){
            toRemove->parent->left = toRemove->right;
            toRemove->right->parent = toRemove->parent;
        }
        if (isRightSon(toRemove)){
            toRemove->parent->right = toRemove->right;
            toRemove->right->parent = toRemove->parent;
        }
        RankAVLNode<Key, Data> *to_rebalance = toRemove->right;
        delete(toRemove);
        updateNumOfNodes(to_rebalance->parent); //TODO
        updateNodeHeight(to_rebalance);
        updateNodeBF(to_rebalance);
        rebalanceTreeAfterRemove(tree, to_rebalance);
    }
}

/*----------------------------------------------------------------------------*/

//remove node that have only left son
template<class Key, class Data>
static void onlyLeftSonRemove(RankAVLTree<Key, Data> *tree,
                              RankAVLNode<Key, Data> *toRemove)
{
    if (isRoot(toRemove)){
        tree->updateRoot(toRemove->left);
        delete(toRemove);
        return;
    }

    else{
        if (isLeftSon(toRemove)){
            toRemove->parent->left = toRemove->left;
            toRemove->left->parent = toRemove->parent;
        }
        if (isRightSon(toRemove)){
            toRemove->parent->right = toRemove->left;
            toRemove->left->parent = toRemove->parent;
        }
        RankAVLNode<Key, Data> *to_rebalance = toRemove->left;
        delete(toRemove);
        updateNumOfNodes(to_rebalance->parent); //TODO
        updateNodeHeight(to_rebalance);
        updateNodeBF(to_rebalance);
        rebalanceTreeAfterRemove(tree, to_rebalance);
    }
}

/*----------------------------------------------------------------------------*/

//find the leftmost son of root
template<class Key, class Data>
static RankAVLNode<Key, Data> * findLeftmostSon(RankAVLNode<Key, Data> *root)
{
    if (root->left == nullptr) return root;
    return findLeftmostSon(root->left);
}

/*----------------------------------------------------------------------------*/

//n is not the biggest node in tree!
//find the next node in the tree
template<class Key, class Data>
static RankAVLNode<Key, Data> * findNextNode(RankAVLNode<Key, Data> *n){
    if (n->right != nullptr)    return findLeftmostSon(n->right);

    else{//n->right == nullptr
        //if (isRoot(n))          return nullptr;
        if (isLeftSon(n))  return n->parent;
        else     return n->parent->parent;//isRightSon(n)
    }
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void swapNodes(RankAVLNode<Key, Data> *n1, RankAVLNode<Key, Data> *n2){
    if ((n1->right != nullptr && n1->right->key == n2->key) ||
        (n1->left != nullptr && n1->left->key == n2->key)) //n1 is parent of n2
    {
        swapSonAndParent(n2, n1);
        return;
    }

    else if ((n2->right != nullptr && n2->right->key == n1->key) ||
             (n2->left != nullptr && n2->left->key == n1->key)) //n2 is parent of n1
    {
        swapSonAndParent(n1, n2);
        return;
    }

    swapSeparateNodes(n1, n2);
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void swapSonAndParent(RankAVLNode<Key, Data> *son, RankAVLNode<Key,
        Data> *parent)
{
    int son_is_left_son_of_parent = isLeftSon(son);

    if (son->left != nullptr)   son->left->parent = parent;
    if (son->right != nullptr)  son->right->parent = parent;
    if (parent->parent != nullptr){
        if (isLeftSon(parent))   parent->parent->left = son;
        else    parent->parent->right = son;
    }
    if (isRightSon(son) && parent->left != nullptr) parent->left->parent = son;
    else if (isLeftSon(son)&&parent->right!=nullptr)parent->right->parent = son;

    //backup son pointers
    RankAVLNode<Key, Data> *son_left = son->left;
    RankAVLNode<Key, Data> *son_right = son->right;
    int son_height = son->height;
    int son_bf = son->bf;
    int son_numOfNodes = son->numOfNodes; //TODO

    //update son pointers
    son->parent = parent->parent;
    if (son_is_left_son_of_parent){
        son->left = parent;
        son->right = parent->right;
    }
    else{ //son_is_right_son_of_parent
        son->left = parent->left;
        son->right = parent;
    }
    son->height = parent->height;
    son->bf = parent->bf;
    son->numOfNodes = parent->numOfNodes; //TODO

    //update parent pointers
    parent->parent = son;
    parent->left = son_left;
    parent->right = son_right;
    parent->height = son_height;
    parent->bf = son_bf;
    parent->numOfNodes = son_numOfNodes; //TODO
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void swapSeparateNodes(RankAVLNode<Key, Data> *n1, RankAVLNode<Key,
        Data> *n2)
{
    if (n1->left != nullptr)    n1->left->parent = n2;

    if (n1->right != nullptr)   n1->right->parent = n2;

    if (n1-> parent != nullptr){
        if (isLeftSon(n1))  n1->parent->left = n2;
        else    n1->parent->right = n2;
    }

    if (n2->left != nullptr)    n2->left->parent = n1;

    if (n2->right != nullptr)   n2->right->parent = n1;

    if (n2-> parent != nullptr){
        if (isLeftSon(n2))      n2->parent->left = n1;
        else     n2->parent->right = n1;
    }

    //backup n1 pointers
    RankAVLNode<Key, Data> *n1_parent = n1->parent;
    RankAVLNode<Key, Data> *n1_left = n1->left;
    RankAVLNode<Key, Data> *n1_right = n1->right;
    int n1_height = n1->height;
    int n1_bf = n1->bf;
    int n1_numOfNodes = n1->numOfNodes; //TODO

    //update n1 pointers
    n1->parent = n2->parent;
    n1->left = n2->left;
    n1->right = n2->right;
    n1->height = n2->height;
    n1->bf = n2->bf;
    n1->numOfNodes = n2->numOfNodes; //TODO

    //update n2 pointers
    n2->parent = n1_parent;
    n2->left = n1_left;
    n2->right = n1_right;
    n2->height = n1_height;
    n2->bf = n1_bf;
    n2->numOfNodes = n1_numOfNodes; //TODO
}

/*----------------------------------------------------------------------------*/

//update the height of the vortex n
template<class Key, class Data>
static void updateNodeHeight(RankAVLNode<Key, Data> *n){
    if (n->left == nullptr && n->right == nullptr)  n->height = 0;

    else if (n->left == nullptr && n->right != nullptr)
        n->height = 1+n->right->height;

    else if (n->left != nullptr && n->right == nullptr)
        n->height = 1+n->left->height;

    else    n->height = 1 + max(n->left->height, n->right->height);
}

/*----------------------------------------------------------------------------*/

//update the BF of n and all his ancestors
template<class Key, class Data>
static void updateNodeBF(RankAVLNode<Key, Data> *n)
{
    int left_son_height;
    int right_son_height;

    if (n->left == nullptr && n->right == nullptr) {
        left_son_height = 0;
        right_son_height = 0;
    } else if (n->left == nullptr && n->right != nullptr) {
        left_son_height = 0;
        right_son_height = 1 + n->right->height;
    } else if (n->right == nullptr && n->left != nullptr) {
        right_son_height = 0;
        left_son_height = 1 + n->left->height;
    } else {
        left_son_height = 1 + n->left->height;
        right_son_height = 1 + n->right->height;
    }
    n->bf = left_son_height - right_son_height;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void clearSubTree(RankAVLNode<Key, Data> *root)
{
    if (root->left != nullptr){
        clearSubTree(root->left);
        root->left = nullptr;
    }

    if (root->right != nullptr){
        clearSubTree(root->right);
        root->right = nullptr;
    }

    delete root;
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void printSubTree(RankAVLNode<Key, Data> *root)
{
    if (root->left != nullptr)  printSubTree(root->left);

    cout<<" KEY: "<<root->key<<"ST: "<<root->numOfNodes<<endl;

    if (root->right != nullptr)  printSubTree(root->right);
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void updateNumOfNodes(RankAVLNode<Key, Data> *n) //TODO
{
    while(n != nullptr){
        int left = 0;
        if (n->left != nullptr){
            left = n->left->numOfNodes;
        }
        int right = 0;
        if (n->right != nullptr){
            right = n->right->numOfNodes;
        }
        n->numOfNodes = 1 + left + right;

        n = n->parent;
    }
}

/*----------------------------------------------------------------------------*/

template<class Key, class Data>
static void updateNumOfNodesForOneNode(RankAVLNode<Key, Data> *n) //TODO
{
    int left = 0;
    if (n->left != nullptr){
        left = n->left->numOfNodes;
    }
    int right = 0;
    if (n->right != nullptr){
        right = n->right->numOfNodes;
    }
    n->numOfNodes = 1 + left + right;
}
#endif //DS_WET2_RANKAVLTREE_H
