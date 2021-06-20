//
// Created by omerg on 07/06/2021.
//

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
            if(car== nullptr)return FAILURE;
            agn->removeNode(typeID);
            car->data+=k;
            auto singleton= CDM->agencies.getSingleton(CDM->agencies.Find(agencyID));
            agn->insertNode(typeID,car->data);
            if(!singleton.min) singleton.min = agn->findNode(typeID);
            else{
                if(car->data<singleton.min->data) singleton.min = agn->findNode(typeID);
                if(car->data==singleton.min->data && typeID < singleton.min->key)
                    singleton.min = agn->findNode(typeID);
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

            if(DS== nullptr|| res== nullptr|| agencyID<0) return INVALID_INPUT;
            auto CDM=(CarDealershipManager*)DS;
            if(agencyID>= CDM->agencies.agenciesObjects.getSize())return FAILURE;
            auto singleton = CDM->agencies.getSingleton(CDM->agencies.Find(agencyID));
            if(i==0) {
                *res = singleton.min->key;
                return SUCCESS;
            }
            *res = singleton.group->Select(i)->key;
            return SUCCESS;


        } catch (std::bad_alloc&) {
            return ALLOCATION_ERROR;
        }
    }

};


#endif //WET2_CARDEALERSHIPMANAGER_H
