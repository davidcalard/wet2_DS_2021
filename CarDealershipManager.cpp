//
// Created by omerg on 07/06/2021.
//

#include "CarDealershipManager.h"
#include "exception"

StatusType CDMAddAgency(void *DS){
    try {
        auto CDM=(CarDealershipManager*)DS;
        if(CDM== nullptr)return INVALID_INPUT;
        ///add new agency to UF
        return SUCCESS;

    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }

}
StatusType CDMSellCar(void *DS, int agencyID, int typeID, int k){
    try {
        auto CDM=(CarDealershipManager*)DS;
        if(CDM== nullptr || agencyID<0 || k<=0)return INVALID_INPUT;
        ///find agID
        return SUCCESS;

    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}
StatusType CDMUniteAgencies(void *DS, int agencyID1, int agencyID2){
    try {
        auto CDM=(CarDealershipManager*)DS;
        if(CDM== nullptr || agencyID1<0 || agencyID2<0)return INVALID_INPUT;
        ///find a1, find a2, union(a1,a2)
        return SUCCESS;

    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}
StatusType CDMGetIthSoldType(void *DS, int agencyID, int i, int* res){
    try {
        auto CDM=(CarDealershipManager*)DS;
        if(CDM== nullptr|| res== nullptr|| agencyID<0) return INVALID_INPUT;
        ///find aID
        if(i==0);///res=aID.min , return SUCC
        ///find i ranked node in aID tree
        return SUCCESS;


    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

