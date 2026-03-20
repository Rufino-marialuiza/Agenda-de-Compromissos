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

struct noLista {
    struct Compromisso compromisso;
    struct noLista *proximoPtr;
};

typedef struct noLista NoLista;
typedef NoLista *NoListaPtr;

// Protótipos das Funções
int idExiste(NoListaPtr atualPtr, int idBusca);
int validarData(int d, int m, int a);
void inserir(NoListaPtr *sPtr, struct Compromisso novo);
char excluirNo(NoListaPtr *sPtr, int idBusca);
int estaVazia(NoListaPtr sPtr);
void imprimirLista(NoListaPtr atualPtr);
void imprimirMes(NoListaPtr atualPtr, int mesBusca, int anoBusca);
int compararDatas(struct Data d1, struct Data d2);
int diaDaSemana(int d, int m, int y);
int diasNoMes(int m, int y);
void modificarCompromisso(NoListaPtr *sPtr);

#endif
