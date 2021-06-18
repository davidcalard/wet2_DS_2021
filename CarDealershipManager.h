//
// Created by omerg on 07/06/2021.
//

#ifndef WET2_CARDEALERSHIPMANAGER_H
#define WET2_CARDEALERSHIPMANAGER_H


#include "library2.h"
#include "UnionFind.h"


class CarDealershipManager {
public:
    UnionFind agencies;

    CarDealershipManager() = default;
    ~CarDealershipManager() = default;

    StatusType CDMAddAgency(void *DS);
    StatusType CDMSellCar(void *DS, int agencyID, int typeID, int k);
    StatusType CDMUniteAgencies(void *DS, int agencyID1, int agencyID2);
    StatusType CDMGetIthSoldType(void *DS, int agencyID, int i, int* res);
};


#endif //WET2_CARDEALERSHIPMANAGER_H
