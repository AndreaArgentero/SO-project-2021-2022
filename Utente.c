#include "includer.h"

int *t, shmid, shmid_table;
LibroMastro *L;

int main(int argc, char** argv){
    
    
    int Random, Try = 0, Trasfer;
    time_t tm;
    
    struct timespec tim, tim2, STAMP;
    messaggio msg;

    int semID, msgsend;
    int msgid;
    int lb1 = 0, lb2, u = 0, User = 0, budget_da_lm = 0, budget_ra_rm = 0;
    float Reward;

    srand((unsigned) time(&tm)*getpid());
        
    semID = semget(SEM_KEY, 2, S_IRUSR | S_IWUSR);
    shmid = shmget(SHM_KEY, sizeof(LibroMastro), 0);
    shmid_table = shmget(SHM_TABLE, sizeof(int)*((SO_NODES_NUM*3)+(SO_USERS_NUM*2)), 0);

    L = (LibroMastro *)shmat(shmid, NULL, 0);
    t = shmat(shmid_table,NULL,0);

    tim.tv_sec = 0;
    tim.tv_nsec = randomValue(SO_MAX_TRANS_GEN_NSEC, SO_MIN_TRANS_GEN_NSEC);


            User = atoi(argv[1]);   
            t[User+SO_USERS_NUM] = SO_BUDGET_INIT;

    while(Try < SO_RETRY){

        srand((unsigned) time(&tm)*getpid()*tim.tv_nsec);
        tim.tv_nsec = randomValue(SO_MAX_TRANS_GEN_NSEC, SO_MIN_TRANS_GEN_NSEC);

        if(t[User+SO_USERS_NUM] > 1){
            Try = 0;
            reserveSem(semID,0);
            msgid = msgget(t[rand() % SO_NODES_NUM],0777);
            while(msgid < 0){
                msgid = msgget(t[rand() % SO_NODES_NUM],0777);
            }

                    if( clock_gettime( CLOCK_MONOTONIC, &STAMP) == -1 ) {
                        perror( "clock gettime" );
                        exit( EXIT_FAILURE );
                    }

                    msg.Timestamp = takeTime(STAMP);
                    msg.sender = getpid();

                    do
                    {
                        Random = t[randomValue((SO_NODES_NUM*3)+SO_USERS_NUM, SO_NODES_NUM*3)];
                    }while(Random == getpid());
                    msg.reciver = Random;

                    Trasfer = randomTransfer(t[User+SO_USERS_NUM], 2);

                    Reward = ((float)Trasfer/(float)100)*SO_REWARD;
                    if(Reward < 1){Reward = 1;}
                    if((Reward - (int)Reward) >= 0.5){
                        Reward = (int)Reward + 1;
                    }
                    else{
                        Reward = (int)Reward;
                    }

                    msg.transfer = Trasfer - (int)Reward;
                    msg.reward = Reward;

                    
                    msgsend=msgsnd(msgid, &msg,sizeof(msg),IPC_NOWAIT);

                    while (msgsend < 0)
                    {
                        msgsend=msgsnd(msgid, &msg,sizeof(msg),IPC_NOWAIT);
                        if(msgsend < 0){
                            Try++;
                        }
                        if(Try >= SO_RETRY){
                            L->KillCount++;
                            printf("processo utente %d è morto", getpid());
                            exit(EXIT_FAILURE);
                        }
                    }
                    if(msgsend == 0){
                        t[User+SO_USERS_NUM] = t[User+SO_USERS_NUM] - Trasfer;
                    }
                    Try=0;

                    releaseSem(semID,1);

                releaseSem(semID,0);

                for(lb1 ; lb1 < L->Indice; lb1++){
                    for( lb2 = 0; lb2 < SO_BLOCK_SIZE; lb2++){
                        if(L->Libro[lb1].Blocco[lb2].reciver == getpid()){
                            t[User+SO_USERS_NUM] = t[User+SO_USERS_NUM] + L->Libro[lb1].Blocco[lb2].money;
                        }
                    }
                }

                nanosleep(&tim, NULL);
        }
        else{
            Try++;
            if(Try >= SO_RETRY){
                L->KillCount++;
                printf("processo utente %d è morto", getpid());
                exit(EXIT_FAILURE);
            }

                for(lb1 ; lb1 < L->Indice; lb1++){
                    for( lb2 = 0; lb2 < SO_BLOCK_SIZE; lb2++){
                        if(L->Libro[lb1].Blocco[lb2].reciver == getpid()){
                            t[User+SO_USERS_NUM] = t[User+SO_USERS_NUM] + L->Libro[lb1].Blocco[lb2].money;
                        }
                    }
                }

            nanosleep(&tim, NULL);
        }


        /**/
    }

    exit(EXIT_FAILURE);
    /*dati->MatrixUsers[atoi(argv[1])][2] = 1;*/
        
}