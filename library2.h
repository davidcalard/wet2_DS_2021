//
// Created by omerg on 06/06/2021.
//


/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 2                                         */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library2.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET2
#define _234218_WET2


#ifdef __cplusplus
extern "C" {
#endif

/* Return Values
 * ----------------------------------- */
typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;


void *Init();

StatusType AddAgency(void *DS);

StatusType SellCar(void *DS, int agencyID, int typeID, int k);

StatusType UniteAgencies(void *DS, int agencyID1, int agencyID2);

StatusType GetIthSoldType(void *DS, int agencyID, int i, int* res);

void Quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET2 */
