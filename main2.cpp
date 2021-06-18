/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Spring 2021                                          */
/* Homework : Wet 1                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main1.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The command's strings */
typedef enum {
    NONE_CMD = -2,
    COMMENT_CMD = -1,
    INIT_CMD = 0,
    ADDAGENCY_CMD = 1,
    SELLCAR_CMD = 2,
    UNITEAGENCIES_CMD = 3,
    GETITHSOLDTYPE_CMD = 4,
    QUIT_CMD = 5
} commandType;

static const int numActions = 6;
static const char *commandStr[] = {
        "Init",
        "AddAgency",
        "SellCar",
        "UniteAgencies",
        "GetIthSoldType",
        "Quit"
};

static const char* ReturnValToStr(int val) {
    switch (val) {
        case SUCCESS:
            return "SUCCESS";
        case ALLOCATION_ERROR:
            return "ALLOCATION_ERROR";
        case FAILURE:
            return "FAILURE";
        case INVALID_INPUT:
            return "INVALID_INPUT";
        default:
            return "";
    }
}

/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum {
    error_free, error
} errorType;
static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString,ErrorParams) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString, ErrorParams); return error; }

static bool isInit = false;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv) {

    char buffer[MAX_STRING_INPUT_SIZE];

    // Reading commands
    while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
        fflush(stdout);
        if (parser(buffer) == error)
            break;
    };
    return 0;
}

/***************************************************************************/
/* Command Checker                                                         */
/***************************************************************************/

static commandType CheckCommand(const char* const command,
                                const char** const command_arg) {
    if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
        return (NONE_CMD);
    if (StrCmp("#", command)) {
        if (strlen(command) > 1)
            printf("%s", command);
        return (COMMENT_CMD);
    };
    for (int index = 0; index < numActions; index++) {
        if (StrCmp(commandStr[index], command)) {
            *command_arg = command + strlen(commandStr[index]) + 1;
            return ((commandType)index);
        };
    };
    return (NONE_CMD);
}

/***************************************************************************/
/* Commands Functions                                                      */
/***************************************************************************/

static errorType OnInit(void** DS, const char* const command);
static errorType OnAddAgency(void* DS, const char* const command);
static errorType OnSellCar(void* DS, const char* const command);
static errorType OnUniteAgencies(void* DS, const char* const command);
static errorType OnGetIthSoldType(void* DS, const char* const command);
static errorType OnQuit(void** DS, const char* const command);

/***************************************************************************/
/* Parser                                                                  */
/***************************************************************************/

static errorType parser(const char* const command) {
    static void *DS = NULL; /* The general data structure */
    const char* command_args = NULL;
    errorType rtn_val = error;

    commandType command_val = CheckCommand(command, &command_args);

    switch (command_val) {

        case (INIT_CMD):
            rtn_val = OnInit(&DS, command_args);
            break;
        case (ADDAGENCY_CMD):
            rtn_val = OnAddAgency(DS, command_args);
            break;
        case (SELLCAR_CMD):
            rtn_val = OnSellCar(DS, command_args);
            break;
        case (UNITEAGENCIES_CMD):
            rtn_val = OnUniteAgencies(DS, command_args);
            break;
        case (GETITHSOLDTYPE_CMD):
            rtn_val = OnGetIthSoldType(DS, command_args);
            break;
        case (QUIT_CMD):
            rtn_val = OnQuit(&DS, command_args);
            break;

        case (COMMENT_CMD):
            rtn_val = error_free;
            break;
        case (NONE_CMD):
            rtn_val = error;
            break;
        default:
            assert(false);
            break;
    };
    return (rtn_val);
}

static errorType OnInit(void** DS, const char* const command) {
    if (isInit) {
        printf("init was already called.\n");
        return (error_free);
    };
    isInit = true;

    ValidateRead(0, 0, "%s failed.\n", commandStr[INIT_CMD]);
    *DS = Init();

    if (*DS == NULL) {
        printf("init failed.\n");
        return error;
    };

    printf("init done.\n");
    return error_free;
}

static errorType OnAddAgency(void* DS, const char* const command) {
    ValidateRead(0, 0, "%s failed.\n", commandStr[ADDAGENCY_CMD]);
    StatusType res = AddAgency(DS);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[ADDAGENCY_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[ADDAGENCY_CMD], ReturnValToStr(res));
    return error_free;
}


static errorType OnSellCar(void* DS, const char* const command) {
    int agencyID, typeID, k;
    ValidateRead(sscanf(command, "%d %d %d", &agencyID, &typeID, &k), 3, "%s failed.\n", commandStr[SELLCAR_CMD]);
    StatusType res = SellCar(DS, agencyID, typeID, k);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[SELLCAR_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[SELLCAR_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnUniteAgencies(void* DS, const char* const command) {
    int agencyID1, agencyID2;
    ValidateRead(sscanf(command, "%d %d", &agencyID1, &agencyID2), 2, "%s failed.\n", commandStr[UNITEAGENCIES_CMD]);
    StatusType res = UniteAgencies(DS, agencyID1, agencyID2);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[UNITEAGENCIES_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[UNITEAGENCIES_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnGetIthSoldType(void* DS, const char* const command) {
    int agencyID, i, typeID;
    ValidateRead(sscanf(command, "%d %d", &agencyID, &i), 2, "%s failed.\n", commandStr[GETITHSOLDTYPE_CMD]);
    StatusType res = GetIthSoldType(DS, agencyID, i, &typeID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[GETITHSOLDTYPE_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %d\n", commandStr[GETITHSOLDTYPE_CMD], typeID);
    return error_free;
}

static errorType OnQuit(void** DS, const char* const command) {
    Quit(DS);
    if (*DS != NULL) {
        printf("quit failed.\n");
        return error;
    };

    isInit = false;
    printf("quit done.\n");
    return error_free;
}

#ifdef __cplusplus
}
#endif