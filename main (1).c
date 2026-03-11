#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // <-- Adicionamos a biblioteca de tempo aqui!

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

// --- Protótipos das Funções ---
int idExiste(ListNodePtr currentPtr, int idBusca);
int validarData(int d, int m, int a);
void insert(ListNodePtr *sPtr, struct Compromisso novo);
char deleteNode(ListNodePtr *sPtr, int idBusca);
int isEmpty(ListNodePtr sPtr);
void printList(ListNodePtr currentPtr);
void printMonth(ListNodePtr currentPtr, int mesBusca, int anoBusca);
int compararDatas(struct Data d1, struct Data d2);

// Funções para desenhar o calendário
int diaDaSemana(int d, int m, int y);
int diasNoMes(int m, int y);

// --- Implementação das Validações ---

int idExiste(ListNodePtr currentPtr, int idBusca) {
    while (currentPtr != NULL) {
        if (currentPtr->compromisso.id == idBusca) return 1;
        currentPtr = currentPtr->nextPtr;
    }
    return 0;
}

int validarData(int d, int m, int a) {
    if (a < 2026) return 0;
    if (m < 1 || m > 12) return 0;
    if (d < 1 || d > 31) return 0;
    
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) return 0;
    if (m == 2) {
        int bissexto = (a % 4 == 0 && (a % 100 != 0 || a % 400 == 0));
        if (bissexto && d > 29) return 0;
        if (!bissexto && d > 28) return 0;
    }
    return 1;
}

// --- Funções Auxiliares para o Calendário Visual ---

// Novo jeito limpo de pegar o dia da semana usando <time.h>
int diaDaSemana(int d, int m, int y) {
    struct tm tempo = {0}; // Zera a estrutura para não ter lixo de memória
    tempo.tm_mday = d;
    tempo.tm_mon = m - 1;     // time.h conta meses de 0 a 11
    tempo.tm_year = y - 1900; // time.h conta os anos a partir de 1900

    mktime(&tempo); // Função nativa do C que calcula o calendário

    return tempo.tm_wday; // Retorna 0 (Dom), 1 (Seg)... até 6 (Sáb)
}

int diasNoMes(int m, int y) {
    if (m == 2) {
        return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ? 29 : 28;
    }
    if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
    return 31;
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

// --- Impressão Geral de Todos os Meses com Eventos ---
void printList(ListNodePtr currentPtr) {
    if (isEmpty(currentPtr)) {
        printf("\nAgenda vazia.\n\n");
        return;
    }

    char *nomeMeses[] = {"", "JANEIRO", "FEVEREIRO", "MARCO", "ABRIL", "MAIO", "JUNHO", 
                         "JULHO", "AGOSTO", "SETEMBRO", "OUTUBRO", "NOVEMBRO", "DEZEMBRO"};

    ListNodePtr temp = currentPtr;

    while (temp != NULL) {
        int anoAtual = temp->compromisso.data.ano;
        int mesAtual = temp->compromisso.data.mes;

        printf("\n=============================\n");
        printf("        %s %d\n", nomeMeses[mesAtual], anoAtual);
        printf("=============================\n");
        printf("Dom Seg Ter Qua Qui Sex Sab\n");

        int diasTotais = diasNoMes(mesAtual, anoAtual);
        int diaSemanaInicio = diaDaSemana(1, mesAtual, anoAtual);

        for (int i = 0; i < diaSemanaInicio; i++) {
            printf("    ");
        }

        for (int d = 1; d <= diasTotais; d++) {
            int temEvento = 0;
            ListNodePtr check = temp;
            
            while (check != NULL && check->compromisso.data.ano == anoAtual && check->compromisso.data.mes == mesAtual) {
                if (check->compromisso.data.dia == d) {
                    temEvento = 1;
                    break;
                }
                check = check->nextPtr;
            }

            if (temEvento) {
                printf("[%2d]", d); 
            } else {
                printf(" %2d ", d);
            }

            if ((d + diaSemanaInicio) % 7 == 0) {
                printf("\n");
            }
        }
        printf("\n-----------------------------\n");
        printf("Eventos deste mes:\n");

        while (temp != NULL && temp->compromisso.data.ano == anoAtual && temp->compromisso.data.mes == mesAtual) {
            printf("Dia %02d - [ID: %3d] %s\n", temp->compromisso.data.dia, temp->compromisso.id, temp->compromisso.nome);
            temp = temp->nextPtr;
        }
    }
    printf("=============================\n\n");
}

// --- Impressão de um Mês Específico ---
void printMonth(ListNodePtr currentPtr, int mesBusca, int anoBusca) {
    char *nomeMeses[] = {"", "JANEIRO", "FEVEREIRO", "MARCO", "ABRIL", "MAIO", "JUNHO", 
                         "JULHO", "AGOSTO", "SETEMBRO", "OUTUBRO", "NOVEMBRO", "DEZEMBRO"};

    printf("\n=============================\n");
    printf("        %s %d\n", nomeMeses[mesBusca], anoBusca);
    printf("=============================\n");
    printf("Dom Seg Ter Qua Qui Sex Sab\n");

    int diasTotais = diasNoMes(mesBusca, anoBusca);
    int diaSemanaInicio = diaDaSemana(1, mesBusca, anoBusca);

    for (int i = 0; i < diaSemanaInicio; i++) {
        printf("    ");
    }

    for (int d = 1; d <= diasTotais; d++) {
        int temEvento = 0;
        ListNodePtr check = currentPtr;
        
        while (check != NULL) {
            if (check->compromisso.data.ano == anoBusca && 
                check->compromisso.data.mes == mesBusca && 
                check->compromisso.data.dia == d) {
                temEvento = 1;
                break;
            }
            check = check->nextPtr;
        }

        if (temEvento) {
            printf("[%2d]", d); 
        } else {
            printf(" %2d ", d);
        }

        if ((d + diaSemanaInicio) % 7 == 0) {
            printf("\n");
        }
    }
    
    printf("\n-----------------------------\n");
    printf("Eventos deste mes:\n");

    int encontrouEventos = 0;
    ListNodePtr temp = currentPtr;
    
    while (temp != NULL) {
        if (temp->compromisso.data.ano == anoBusca && temp->compromisso.data.mes == mesBusca) {
            printf("Dia %02d - [ID: %3d] %s\n", temp->compromisso.data.dia, temp->compromisso.id, temp->compromisso.nome);
            encontrouEventos = 1;
        }
        temp = temp->nextPtr;
    }

    if (!encontrouEventos) {
        printf("Nenhum evento programado para este mes.\n");
    }
    
    printf("=============================\n\n");
}

int main(void) {
    ListNodePtr startPtr = NULL;
    int choice;

    do {
        printf("MENU:\n");
        printf("1. Inserir Compromisso\n");
        printf("2. Excluir Compromisso\n");
        printf("3. Listar por Mes\n");
        printf("4. Sair\n? ");
        scanf("%d", &choice);

        if (choice == 1) {
            struct Compromisso c;
            
            do {
                printf("ID (unico): ");
                scanf("%d", &c.id);
                if (idExiste(startPtr, c.id)) {
                    printf("Erro: Este ID ja existe! Tente outro.\n");
                }
            } while (idExiste(startPtr, c.id));

            printf("Nome do Evento: ");
            scanf(" %[^\n]", c.nome);

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

        } else if (choice == 3) {
            int mBusca, aBusca;
            printf("Digite o mes e ano desejados (MM AAAA): ");
            scanf("%d %d", &mBusca, &aBusca);
            
            if (mBusca < 1 || mBusca > 12 || aBusca < 2026) {
                printf("Data invalida para busca.\n\n");
            } else {
                printMonth(startPtr, mBusca, aBusca);
            }
        }

    } while (choice != 4);

    return 0;
}