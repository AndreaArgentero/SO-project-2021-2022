#include "includer.h"

int semid_start, *t;
int Bool_Start = 0, Bool_KILL = 0;
time_t start, stop;



void handler_int(int s) {


        SetSem(semid_start, 0, SO_USERS_NUM+SO_NODES_NUM);

        start = time(NULL);
        Bool_Start = 1;
        system("stty echoctl");
        signal(SIGINT, SIG_IGN);

}

void KillAllIPC(int shmid_libro, int shmid_table, int semid_start, int semID_node_user, int msgid_canal, LibroMastro *L){

        int k;

            semctl(semid_start, 0, IPC_RMID);
            semctl(semID_node_user,0, IPC_RMID);
            /*------------------------*/
            for( k = 0; k < SO_NODES_NUM; k++){
                msgid_canal = msgget(t[k], 0);
                msgctl(msgid_canal, IPC_RMID, NULL);
            }
            /*-------------------------------*/
            shmdt(L);
            shmdt(t);

            shmctl(shmid_libro, 0, IPC_RMID);
            shmctl(shmid_table, 0, IPC_RMID); 

}

void KillAllProcess(int *t){
    int i;
    char str[30];

    for(i = 0; i < SO_NODES_NUM; i++){
        sprintf(str, "kill %d", t[i]);
        system(str);
    }

    for(i = (SO_NODES_NUM*3); i < (SO_USERS_NUM+(SO_NODES_NUM*3)); i++){
        sprintf(str, "kill %d", t[i]);
        system(str);
    }

}

void KillMaster(){

    system("ipcrm -a");
    exit(EXIT_FAILURE);

}

void handler_term(int s){

    if(Bool_KILL == 1){
    printf("-TERMINAZIONE FORZATA-\n");
    KillAllProcess(t);
    KillMaster();
    }

    signal(SIGQUIT, SIG_IGN);

}



int main(int argc, char** argv){
    
    LibroMastro *L;
    
    double accum, tempp;
    int shmid_table, shmid;
    int pID, entrate, uscite, bilancio, sum_reward;
    int msgid;
    int semID;
    int i = 0,z = 0,m,k;
    char str[10];
    
    
    
    system("stty -echoctl");
    signal(SIGINT, handler_int);
    signal(SIGQUIT, handler_term);


    semid_start = semget(SEM_START,1,IPC_CREAT|S_IRUSR | S_IWUSR);
    semID = semget(SEM_KEY, 2, IPC_CREAT|S_IRUSR | S_IWUSR);
    shmid = shmget(SHM_KEY, sizeof(LibroMastro), IPC_CREAT|0666);
    shmid_table = shmget(SHM_TABLE, sizeof(int)*((SO_NODES_NUM*3)+(SO_USERS_NUM*2)), IPC_CREAT|0666);


    L = (LibroMastro *)shmat(shmid, NULL, 0);
    t = shmat(shmid_table,NULL,0);
    
    initSemInUse(semid_start,0);
    initSemInUse(semID,0);
    initSemInUse(semID,1);

    Utenti(t, str, semid_start);

    Nodi(t, str, semid_start);

    printf("Premere Ctrl+C per iniziare la simulazione.\n");
    printf("Premere Ctrl+\\ per terminare la simulazione in maniera forzata.\n");
    Bool_KILL = 1;

    do{
        
        if(Bool_Start == 1){

            sleep(1);

            printf("--------------------------------------\n");
            /*printf("Sono passati %.0f secondi dall'inizio della simulazione.\n", (stop - start));*/
            printf("Utente attivi: %d \nNodi attivi: %d\n", SO_USERS_NUM-L->KillCount, SO_NODES_NUM);

            if(SO_NODES_NUM + SO_USERS_NUM <= MAX_PROCESS){
            printf("budget corrente di ogni Utente e Nodo:\n");

                for (i=(SO_NODES_NUM*3); i<(SO_USERS_NUM+(SO_NODES_NUM*3)); i++) {
                entrate = 0;
                uscite = 0;
                for (k=0; k<L->Indice; k++) {
                    for (m=0; m<SO_BLOCK_SIZE; m++) {
                        if (L->Libro[k].Blocco[m].sender == t[i]) {
                            uscite += L->Libro[k].Blocco[m].money + L->Libro[k].Blocco[m].reward;
                        }
                        if (L->Libro[k].Blocco[m].reciver == t[i]) {
                            entrate += L->Libro[k].Blocco[m].money;
                        }
                    }
                }

            /* mi aspetto di avere 100 + (80 + 80 + 80) - (100) = 240*/
            bilancio = SO_BUDGET_INIT + entrate - uscite;
            printf("Utente: %i, Bilancio: %i\n", t[i], bilancio);

            }

                for (i=0; i<SO_NODES_NUM; i++) {
                /* per ogni blocco del libro mastro*/
                sum_reward = 0;
                for (m=0; m<L->Indice; m++) {
                    /* guardo se la transazione di reward corrisponde al pid del nodo in questione*/
                    if (L->Libro[m].Blocco[SO_BLOCK_SIZE-1].reciver == t[i]) {
                        sum_reward += L->Libro[m].Blocco[SO_BLOCK_SIZE-1].money;
                    }
                }
                printf("Nodo: %i, Bilancio: %i\n", t[i], sum_reward);
            }
            }

            stop = time(NULL);
        }

    }while(((stop - start) < SO_SIM_SEC) && L->Indice != SO_REGISTRY_SIZE && L->KillCount != SO_USERS_NUM);

        KillAllProcess(t);

        if((stop - start) >= SO_SIM_SEC){

            printf("--------------------------------------\n");            
            printf("-terminazione per tempo\n");
            printf("--------------------------------------\n");
        }
        else if(L->Indice == SO_REGISTRY_SIZE){

            printf("--------------------------------------\n");            
            printf("-terminazione per LibroMastro\n");
            printf("--------------------------------------\n");
            
        }
        else if(L->KillCount == SO_USERS_NUM){

            printf("--------------------------------------\n");            
            printf("-Terminazione per Morte degli Utenti\n");
            printf("--------------------------------------\n");
           
        }

            printf("-Bilancio Utenti:\n");
            for( m = (SO_NODES_NUM*3); m < (SO_NODES_NUM*3)+SO_USERS_NUM; m++){
                printf("%d = %d\n", t[m], t[m+SO_USERS_NUM]);
            }
            printf("--------------------------------------\n");

            printf("-Bilancio Nodi:\n");
            for( m = 0; m < SO_NODES_NUM; m++){
                printf("%d = %d\n", t[m], t[m+SO_NODES_NUM]);
            }
            printf("--------------------------------------\n");

            printf("-Utenti terminati prematuramente:\n");
                printf("%d\n", L->KillCount);
            printf("--------------------------------------\n");

            printf("-Numero Blocchi Libro Mastro\n");
                printf("%d\n", L->Indice);
            printf("--------------------------------------\n");

            printf("-Numero trasazioni ancora in trasaction pool:\n");
            for( m = 0; m < SO_NODES_NUM; m++){
                 printf("%d = %d\n", t[m], t[m+SO_NODES_NUM+SO_NODES_NUM]);
            }
            printf("--------------------------------------\n");

            KillAllIPC(shmid, shmid_table, semid_start, semID, msgid, L);

    exit(EXIT_SUCCESS);

}