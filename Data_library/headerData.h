/* Dati Progetto */
#include "headerLIB.h"

    #define SO_USERS_NUM            (DATA("../DATA/SO_USERS_NUM.txt"))
    #define SO_NODES_NUM            (DATA("../DATA/SO_NODES_NUM.txt"))
    #define SO_BUDGET_INIT          (DATA("../DATA/SO_BUDGET_INIT.txt"))
    #define SO_REWARD               (DATA("../DATA/SO_REWARD.txt"))
    #define SO_MIN_TRANS_GEN_NSEC   (DATA("../DATA/SO_MIN_TRANS_GEN_NSEC.txt"))
    #define SO_MAX_TRANS_GEN_NSEC   (DATA("../DATA/SO_MAX_TRANS_GEN_NSEC.txt"))
    #define SO_RETRY                (DATA("../DATA/SO_RETRY.txt"))
    #define SO_TP_SIZE              (DATA("../DATA/SO_TP_SIZE.txt"))
    #define SO_MIN_TRANS_PROC_NSEC  (DATA("../DATA/SO_MIN_TRANS_PROC_NSEC.txt"))
    #define SO_MAX_TRANS_PROC_NSEC  (DATA("../DATA/SO_MAX_TRANS_PROC_NSEC.txt"))
    #define SO_SIM_SEC              (DATA("../DATA/SO_SIM_SEC.txt"))

    #define SO_BLOCK_SIZE           10
    #define SO_REGISTRY_SIZE        1000
    /*#define SO_HOPS*/
    /*#define SO_FRIENDS_NUM*/

/* Chiavi */
    #define SHM_KEY                 3121922
    #define SHM_TABLE               1111
    #define SHM_KILL                5555
    #define SEM_KEY                 5051941
    #define SEM_START               0000000
    #define CLOCK_MONOTONIC         1

/*Cose Macro*/

    #define MACRO_IN                (pow(5,2)-26)
    #define BILLION                 1000000000L 
    #define MAX_PROCESS             110
