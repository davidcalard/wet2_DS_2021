#ifndef WET2_CARDEALERSHIPMANAGER_H
#define WET2_CARDEALERSHIPMANAGER_H

#include "UnionFind.h"



class CarDealershipManager {
public:
    UnionFind agencies;

    CarDealershipManager() = default;
    ~CarDealershipManager() = default;

    StatusType CDMAddAgency(void *DS){
        try {
            if(DS== nullptr)return INVALID_INPUT;
            auto CDM=(CarDealershipManager*)DS;
            CDM->agencies.insertSingleton();
            return SUCCESS;

        } catch (std::bad_alloc&) {
            return ALLOCATION_ERROR;
        }

    }
    StatusType CDMSellCar(void *DS, int agencyID, int typeID, int k){
        try {
            if(DS== nullptr || agencyID<0 || k<=0)return INVALID_INPUT;
            auto CDM=(CarDealershipManager*)DS;
            if(agencyID>= CDM->agencies.agenciesObjects.getSize())return FAILURE;
            auto agn= CDM->agencies.getTree(CDM->agencies.Find(agencyID));
            auto car=agn->findNode(typeID);
            auto singleton = CDM->agencies.getSingleton(CDM->agencies.Find(agencyID));
            if(car==NULL) {//not sold yet
                auto s_node= new RankAVLNode<Sales,carID>(k,typeID);
                if(s_node== nullptr)throw std::bad_alloc();
                CDM->agencies.agenciesObjects[agencyID]->sales->insertExistNode(s_node);
                agn->insertNode(typeID,s_node);
                car=agn->findNode(typeID);
            }else{
                CDM->agencies.getSingleton(CDM->agencies.Find(agencyID)).sales->removeExactNode(car->data);
                car->data->key+=k;
                CDM->agencies.getSingleton(CDM->agencies.Find(agencyID)).sales->insertExistNode(car->data);
            }
                if(!CDM->agencies.getSingleton(CDM->agencies.Find(agencyID)).min) {
                    CDM->agencies.getSingleton(CDM->agencies.Find(agencyID)).min = car->data ;
                }else{
                    if(car->data->key < singleton.min->key) CDM->agencies.getSingleton(CDM->agencies.Find(agencyID)).min = car->data;
                    if(car->data->key == singleton.min->key && typeID < singleton.min->key)
                        CDM->agencies.getSingleton(CDM->agencies.Find(agencyID)).min = car->data;
                }
                return SUCCESS;

            } catch (std::bad_alloc&) {
                    return ALLOCATION_ERROR;
            }
        }
        StatusType CDMUniteAgencies(void *DS, int agencyID1, int agencyID2){
            try {
                if(DS== nullptr || agencyID1<0 || agencyID2<0)return INVALID_INPUT;
                auto CDM=(CarDealershipManager*)DS;
                if(agencyID1>= CDM->agencies.agenciesObjects.getSize() ||
                   agencyID2>= CDM->agencies.agenciesObjects.getSize())return FAILURE;
                CDM->agencies.Union(agencyID2,agencyID1);
                return SUCCESS;

            } catch (std::bad_alloc&) {
                return ALLOCATION_ERROR;
            }
        }
        StatusType CDMGetIthSoldType(void *DS, int agencyID, int i, int* res){
            try {
                if(DS== nullptr|| res== nullptr|| agencyID<0 || i<0) return INVALID_INPUT;
                auto CDM=(CarDealershipManager*)DS;
                if(agencyID>= CDM->agencies.agenciesObjects.getSize())return FAILURE;
                if(CDM->agencies.getSingleton(CDM->agencies.Find(agencyID)).sales->root == NULL)return FAILURE;
                if(CDM->agencies.getSingleton(CDM->agencies.Find(agencyID)).sales->root->numOfNodes<=i)return FAILURE;
                if(i==0) {
                    *res = CDM->agencies.getSingleton(CDM->agencies.Find(agencyID)).min->data;
                    return SUCCESS;
                }
                *res = CDM->agencies.getSingleton(CDM->agencies.Find(agencyID)).sales->Select(i)->data;
                return SUCCESS;
            } catch (std::bad_alloc&) {
                return ALLOCATION_ERROR;
            }
        }

    };


#endif //WET2_CARDEALERSHIPMANAGER_H