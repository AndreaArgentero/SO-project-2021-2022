#include "headerLIB.h"
#include "headerData.h"

#define L_EMPTYLIST_CL {NULL, NULL}

typedef struct{
    float timestamp;
    int sender;
    int reciver;
    int money;
    float reward;
}Transazione_Utente_Utente;

typedef struct
{
    Transazione_Utente_Utente Blocco[SO_BLOCK_SIZE];

}Block;

typedef struct
{
    Block Libro[SO_REGISTRY_SIZE];
    int Indice;
    int KillCount;

}LibroMastro;



typedef struct Node{
    Transazione_Utente_Utente data;
    struct Node *next;
}ListNode;

typedef struct
{
    ListNode *head;
    ListNode *tail;

} List;
