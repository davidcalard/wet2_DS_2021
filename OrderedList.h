//
// Created by omerg on 03/05/2020.
//



//===================================================================================
#ifndef WET1_ORDEREDLIST_H
#define WET1_ORDEREDLIST_H

#include <cstdlib>


template<class T>
class OrderedListNode {
public:
    T object;
    OrderedListNode*  prev;
    OrderedListNode* next;

    OrderedListNode(T object, OrderedListNode* prev= nullptr, OrderedListNode* next= nullptr):
             object(object),prev(prev),next(next){}
    OrderedListNode(const OrderedListNode& node):object(T()),prev(nullptr),next(nullptr){}
    ~OrderedListNode(){
        prev= nullptr;
        next= nullptr;
    }

    void updateNext(OrderedListNode<T>* n){next=n;}
    void updatePrev(OrderedListNode<T>* p){prev=p;}


    friend bool operator==(const OrderedListNode& a, T b){
        return (a.object == b);
    }
    friend bool operator!=(const OrderedListNode& a, T b){
        return (a.object != b);
    }
    friend bool operator<(const OrderedListNode& a, const OrderedListNode& b){
        return a.object < b.object;
    }
    friend bool operator>(const OrderedListNode& a, const OrderedListNode& b){
        return a.object > b.object;
    }




};

template<class T>
class OrderedList{
public:
    OrderedListNode<T>* start;
    OrderedListNode<T>* end;
    int len;

    OrderedList(): start(nullptr),end(nullptr),len(0){}
    ~OrderedList() {destroyRecurs(this->start);}


    void destroyRecurs(OrderedListNode<T>* strt){
        if(strt== nullptr)return;
        destroyRecurs(strt->next);
        delete strt;
    }

    void add(T& obj){
        auto node=new OrderedListNode<T>(obj,end, nullptr);
        end=node;
        if(start== nullptr)start=node;
        len++;
    }

    void remove(OrderedListNode<T>* node){
        if(node== nullptr)return;
        auto p=node->prev;
        auto n=node->next;
        if(n== nullptr)this->end=p;
        else n->prev=p;
        if(p== nullptr)this->start=n;
        else p->next=n;
        delete node;
        len--;
    }

    bool isEmpty(){return start == nullptr;}

    //combine 2 Ordered list
    OrderedList(OrderedList& l1,OrderedList& l2):start(nullptr),end(nullptr),len(0){
        OrderedListNode<T>* itr1, *itr2;
        itr1=l1.start;
        itr2=l2.start;
        for (int i = 0; i <l1.len+l2.len ; ++i) {
            if(((itr2!= nullptr && itr1!= nullptr) && itr1->object < itr2->object)||
                    (itr2== nullptr&&itr1!= nullptr)){
                this->add(itr1->object);
                itr1=itr1->next;
            } else {
                this->add(itr2->object);
                itr2=itr2->next;
            }
        }
    }
};



#endif //WET1_ORDEREDLIST_H
