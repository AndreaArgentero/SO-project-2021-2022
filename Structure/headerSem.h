#include "headerLIB.h"

union semun {
   int val; /* val for SETVAL */
   struct semid_ds *buf; /* Buffer for IPC_STAT and IPC_SET */
   unsigned short *array; /* Buffer for GETALL and SETALL */
#if defined(__linux__)
   struct seminfo *__buf; /* Buffer for IPC_INFO and SEM_INFO*/
#endif
};

int initSemAvailable(int semId, int semNum) {
    union semun arg;
    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}

int initSemAQUANTOCAZZOTIPARE(int semId, int semNum, int Val){
    union semun arg;
    arg.val = Val;
    return semctl(semId, semNum, SETVAL, arg);
}

int initSemInUse(int semId, int semNum) {
    union semun arg;
    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}

int reserveSem(int semId, int semNum) {
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = -1;
    sops.sem_flg = 0;

    return semop(semId, &sops, 1);
}

int releaseSem(int semId, int semNum) {
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = 1;
    sops.sem_flg = 0;
    return semop(semId, &sops, 1);
}

int SetSem(int semId, int semNum, int Num) {
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = +Num;
    sops.sem_flg = 0;

    return semop(semId, &sops, 1);
}