#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// --- Protótipos das Funções de Validação ---
int idExiste(ListNodePtr currentPtr, int idBusca);
int validarData(int d, int m, int a);
void insert(ListNodePtr *sPtr, struct Compromisso novo);
char deleteNode(ListNodePtr *sPtr, int idBusca);
int isEmpty(ListNodePtr sPtr);
void printList(ListNodePtr currentPtr);
int compararDatas(struct Data d1, struct Data d2);

// --- Implementação das Validações ---

// Verifica se o ID já está na lista
int idExiste(ListNodePtr currentPtr, int idBusca) {
    while (currentPtr != NULL) {
        if (currentPtr->compromisso.id == idBusca) return 1; // ID encontrado
        currentPtr = currentPtr->nextPtr;
    }
    return 0; // ID livre
}

// Verifica se a data é lógica e a partir de 2026
int validarData(int d, int m, int a) {
    if (a < 2026) return 0;
    if (m < 1 || m > 12) return 0;
    if (d < 1 || d > 31) return 0;
    
    // Validação extra para meses com 30 dias e Fevereiro
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) return 0;
    if (m == 2) {
        // Checa ano bissexto simplificado
        int bissexto = (a % 4 == 0 && (a % 100 != 0 || a % 400 == 0));
        if (bissexto && d > 29) return 0;
        if (!bissexto && d > 28) return 0;
    }
    return 1;
}

// --- Funções de Manipulação da Lista ---

int compararDatas(struct Data d1, struct Data d2) {
    if (d1.ano != d2.ano) return d1.ano > d2.ano;
    if (d1.mes != d2.mes) return d1.mes > d2.mes;
    return d1.dia > d2.dia;
}

void insert(ListNodePtr *sPtr, struct Compromisso novo) {
    ListNodePtr newPtr = malloc(sizeof(ListNode));
    ListNodePtr previousPtr = NULL;
    ListNodePtr currentPtr = *sPtr;

    if (newPtr != NULL) {
        newPtr->compromisso = novo;
        newPtr->nextPtr = NULL;

        while (currentPtr != NULL && compararDatas(novo.data, currentPtr->compromisso.data)) {
            previousPtr = currentPtr;
            currentPtr = currentPtr->nextPtr;
        }

        if (previousPtr == NULL) {
            newPtr->nextPtr = *sPtr;
            *sPtr = newPtr;
        } else {
            previousPtr->nextPtr = newPtr;
            newPtr->nextPtr = currentPtr;
        }
    }
}

char deleteNode(ListNodePtr *sPtr, int idBusca) {
    if (isEmpty(*sPtr)) return '\0';
    ListNodePtr previousPtr = NULL;
    ListNodePtr currentPtr = *sPtr;

    while (currentPtr != NULL && currentPtr->compromisso.id != idBusca) {
        previousPtr = currentPtr;
        currentPtr = currentPtr->nextPtr;
    }

    if (currentPtr != NULL) {
        char letra = currentPtr->compromisso.nome[0];
        ListNodePtr tempPtr = currentPtr;
        if (previousPtr == NULL) *sPtr = (*sPtr)->nextPtr;
        else previousPtr->nextPtr = currentPtr->nextPtr;
        free(tempPtr);
        return letra;
    }
    return '\0';
}

int isEmpty(ListNodePtr sPtr) { return sPtr == NULL; }

void printList(ListNodePtr currentPtr) {
    if (isEmpty(currentPtr)) {
        printf("\nAgenda vazia.\n");
    } else {
        printf("\n--- CALENDARIO DE COMPROMISSOS ---\n");
        printf("DATA       | ID  | EVENTO\n");
        printf("-----------|-----|------------------\n");
        while (currentPtr != NULL) {
            printf("%02d/%02d/%04d | %-3d | %s\n", 
                    currentPtr->compromisso.data.dia, currentPtr->compromisso.data.mes, 
                    currentPtr->compromisso.data.ano, currentPtr->compromisso.id, currentPtr->compromisso.nome);
            currentPtr = currentPtr->nextPtr;
        }
        printf("----------------------------------\n\n");
    }
}

int main(void) {
    ListNodePtr startPtr = NULL;
    int choice;

    do {
        printf("MENU:\n1. Inserir Compromisso\n2. Excluir Compromisso\n3. Sair\n? ");
        scanf("%d", &choice);

        if (choice == 1) {
            struct Compromisso c;
            
            // Validação de ID Único
            do {
                printf("ID (unico): ");
                scanf("%d", &c.id);
                if (idExiste(startPtr, c.id)) {
                    printf("Erro: Este ID ja existe! Tente outro.\n");
                }
            } while (idExiste(startPtr, c.id));

            printf("Nome do Evento: ");
            scanf(" %[^\n]", c.nome);

            // Validação de Data (Dia 1-31, Mes 1-12, Ano >= 2026)
            do {
                printf("Data (DD MM AAAA): ");
                scanf("%d %d %d", &c.data.dia, &c.data.mes, &c.data.ano);
                if (!validarData(c.data.dia, c.data.mes, c.data.ano)) {
                    printf("Data invalida! Use Dia(1-31), Mes(1-12) e Ano(>=2026).\n");
                }
            } while (!validarData(c.data.dia, c.data.mes, c.data.ano));

            insert(&startPtr, c);
            printList(startPtr);

        } else if (choice == 2) {
            int idBusca;
            printf("ID para excluir: ");
            scanf("%d", &idBusca);
            char res = deleteNode(&startPtr, idBusca);
            if (res != '\0') printf("Sucesso! Removido evento com inicial '%c'.\n", res);
            else printf("ID nao encontrado.\n");
            printList(startPtr);
        }

    } while (choice != 3);

    return 0;
}