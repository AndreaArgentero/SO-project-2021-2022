#include "includer.h"


int BlockI = 0, NT = 0;
int shmid, shmid_table, *t;
LibroMastro *Memory;

List newNode(List l, Transazione_Utente_Utente string){
    ListNode *Elem = (ListNode *)malloc(sizeof(ListNode));
        if(Elem != NULL){
            Elem->data.money = string.money;
            Elem->data.reciver = string.reciver;
            Elem->data.reward = string.reward;
            Elem->data.sender = string.sender;
            Elem->data.timestamp = string.timestamp;
            Elem->next = NULL;

            if(l.head == NULL){
                    l.head = l.tail = Elem;
                    NT++;
            }
            else{
                l.tail->next=Elem;
                l.tail = Elem;
                NT++;
            }
        }

    return l;
}

List rmNode(List l, Transazione_Utente_Utente *string){

    if(l.head!=NULL){
        ListNode *t = l.head;
        *string = l.head->data;
        l.head = l.head->next;
        free(t);
    }

    return l;
}

Transazione_Utente_Utente tr_set(Transazione_Utente_Utente tr, const float TS, const int SD, const int RC, const int MN, const float RW){

    tr.timestamp = TS;
    tr.sender = SD;
    tr.reciver = RC;
    tr.money = MN;
    tr.reward = RW;

    return tr;
}

Block cpinBlock(Block B, List l){
    B.Blocco[BlockI].timestamp = l.head->data.timestamp;
    B.Blocco[BlockI].sender = l.head->data.sender;
    B.Blocco[BlockI].reciver = l.head->data.reciver;
    B.Blocco[BlockI].money = l.head->data.money;
    B.Blocco[BlockI].reward = l.head->data.reward;

    BlockI++;

    return B;
}

Block cpinLastBlock(Block B, int reward_blocco, struct timespec STAMP){

        B.Blocco[SO_BLOCK_SIZE-1].timestamp =  takeTime(STAMP);
        B.Blocco[SO_BLOCK_SIZE-1].sender = MACRO_IN;
        B.Blocco[SO_BLOCK_SIZE-1].reciver = getpid();
        B.Blocco[SO_BLOCK_SIZE-1].money = reward_blocco;
        B.Blocco[SO_BLOCK_SIZE-1].reward = 0;
    
    BlockI = 0;
    return B;
}

LibroMastro fillLibroMastro(LibroMastro L, LibroMastro *Memory, Block B){

    int j;

 if(Memory->Indice < SO_REGISTRY_SIZE){
    for(j = 0; j < SO_BLOCK_SIZE; j++){
    Memory->Libro[Memory->Indice].Blocco[j].timestamp = B.Blocco[j].timestamp;
    Memory->Libro[Memory->Indice].Blocco[j].sender = B.Blocco[j].sender;
    Memory->Libro[Memory->Indice].Blocco[j].reciver = B.Blocco[j].reciver;
    Memory->Libro[Memory->Indice].Blocco[j].money = B.Blocco[j].money;
    Memory->Libro[Memory->Indice].Blocco[j].reward = B.Blocco[j].reward;
    }

    Memory->Indice++;
    }

    return L;
}

int main(int argc, char** argv){

    struct msqid_ds controllocoda;
    Transazione_Utente_Utente tr;
    List l = L_EMPTYLIST_CL;
    Block B;
    LibroMastro L;

    int msgid, semID, n = 0, Node, reward_blocco = 0;
    time_t tm;
    int j, TransactionPool = 0, flag = 1, lb = 0;
    long msg_to_rec = 0;

    struct timespec tim, tim2, STAMP;
    messaggio msg;

    srand((unsigned) time(&tm)*getpid());

    tim.tv_sec = 0;
    tim.tv_nsec = randomValue(SO_MAX_TRANS_PROC_NSEC, SO_MIN_TRANS_PROC_NSEC);

    shmid_table = shmget(SHM_TABLE, sizeof(int)*((SO_NODES_NUM*3)+(SO_USERS_NUM*2)), 0);
    shmid = shmget(SHM_KEY, sizeof(LibroMastro), 0);
    semID = semget(SEM_KEY, 2, S_IRUSR | S_IWUSR);

    Memory = (LibroMastro *)shmat(shmid, NULL, 0);
    Memory->Indice = 0;
    t = shmat(shmid_table, NULL, 0);

    Node = atoi(argv[1]);        
    t[Node+SO_NODES_NUM] = 0;
    t[Node+SO_NODES_NUM+SO_NODES_NUM] = 0;


    if((msgid = msgget(getpid(), IPC_CREAT| 0777))<0){
        printf("error nodi\n");
    }     

    releaseSem(semID,0);

    while(L.Indice < SO_REGISTRY_SIZE){
        srand((unsigned) time(&tm)*getpid()*tim.tv_nsec);
        tim.tv_nsec = randomValue(SO_MAX_TRANS_PROC_NSEC, SO_MIN_TRANS_PROC_NSEC);

        reserveSem(semID, 1);
            if((msgrcv(msgid, (void *)&msg, sizeof(msg), msg_to_rec, 0))<0){
                printf("messaggio non ricevuto\n");
            }
            else{
                tr = tr_set(tr, msg.Timestamp, msg.sender, msg.reciver, msg.transfer, msg.reward);
                l = newNode(l,tr);
                TransactionPool++;
                }
            releaseSem(semID,1);

        if(TransactionPool == SO_BLOCK_SIZE){

            if( clock_gettime( CLOCK_MONOTONIC, &STAMP) == -1 ) {
            perror( "clock gettime" );
            exit( EXIT_FAILURE );
            }

            for(j = 0; j < SO_BLOCK_SIZE-1; j++){
                B = cpinBlock(B, l);
                reward_blocco = reward_blocco + B.Blocco->reward;
                l = rmNode(l , &tr);
                TransactionPool--;
            }
            

            B = cpinLastBlock(B, reward_blocco, STAMP);
            L = fillLibroMastro(L, Memory, B);

            t[Node+SO_NODES_NUM+SO_NODES_NUM] = TransactionPool;
            t[Node+SO_NODES_NUM] = t[Node+SO_NODES_NUM] + reward_blocco;
            reward_blocco = 0;

            nanosleep(&tim, NULL);
            }

        }

       

}