#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

// Estruturas de Dados
struct Data {
    int dia;
    int mes;
    int ano;
};

struct Compromisso {
    int id;
    char nome[50];
    char descricao[150];
    struct Data data;
};

struct listNode {
    struct Compromisso compromisso;
    struct listNode *nextPtr;
};

typedef struct listNode ListNode;
typedef ListNode *ListNodePtr;

// Protótipos das Funções
int idExiste(ListNodePtr currentPtr, int idBusca);
int validarData(int d, int m, int a);
void insert(ListNodePtr *sPtr, struct Compromisso novo);
char deleteNode(ListNodePtr *sPtr, int idBusca);
int isEmpty(ListNodePtr sPtr);
void printList(ListNodePtr currentPtr);
void printMonth(ListNodePtr currentPtr, int mesBusca, int anoBusca);
int compararDatas(struct Data d1, struct Data d2);
int diaDaSemana(int d, int m, int y);
int diasNoMes(int m, int y);

#endif