#include "includer.h"

void Utenti( int *t, char str[], int semid_start){

    int pid, i;

        for(i=(SO_NODES_NUM*3); i<(SO_USERS_NUM+(SO_NODES_NUM*3)); i++){
        switch(pid = fork()){
        case 0: /*figlio*/
            sprintf(str, "%d", i);
            reserveSem(semid_start, 0);
            execlp("./Utente", "./Utente", str);
            break;
        default: /*padre*/
                /* metto il pid dell'utente figlio nella tabella dei pid */
                t[i] = pid;                             /* pid dell'utente */
                t[i+SO_USERS_NUM] = 0;                  /* 0 budget*/
            break;
        }

    }
}

void Nodi(int *t, char str[], int semid_start){

int pid, i;
        for( i = 0; i < SO_NODES_NUM; i++){
        switch(pid = fork()){
        case 0: /*figlio*/
            sprintf(str, "%d", i);
            reserveSem(semid_start, 0);
            execlp("./Nodo", "./Nodo", str);
            break;
            default: /*padre*/
                /* metto il pid del nodo figlio nella tabella dei pid */
                t[i] = pid;                             /* pid del nodo */
                t[i+SO_NODES_NUM] = 0;                  /* 0 soldi nel budget */
                t[i+SO_NODES_NUM+SO_NODES_NUM] = 0;     /* 0 transazioni nella transaction pool */
                break;
        }
    }

}