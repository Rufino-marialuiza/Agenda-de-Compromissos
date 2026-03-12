#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "biblioteca.h"

int idExiste(ListNodePtr currentPtr, int idBusca) { //procura se o id digitado ja existe na struct
    while (currentPtr != NULL) {
        if (currentPtr->compromisso.id == idBusca) return 1;
        currentPtr = currentPtr->nextPtr;
    }
    return 0;
}

int validarData(int d, int m, int a) { //valida data inserida pelo usuario
    if (a < 2026) return 0;//n da para marcar compromisso no ano passado
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

int diaDaSemana(int d, int m, int y) {//Algoritmo de Tomohito Sakamoto simplificado
    struct tm tempo = {0};//para identificar em que dia da semana inicia o mes
    tempo.tm_mday = d;//usa a biblioteca time.h
    tempo.tm_mon = m - 1;//pois conta meses de 0 a 11
    tempo.tm_year = y - 1900;//conta anos a partir de 1900, por isso a conta
    mktime(&tempo);//converte essa data para o tempo de sistema e normaliza a estrutura(completa info que faltam)
    return tempo.tm_wday;//int que identifica o dia da semana
}

int diasNoMes(int m, int y) {// quantos dias tem o mes
    if (m == 2) {
        return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ? 29 : 28;
    }
    if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
    return 31;
}

int compararDatas(struct Data d1, struct Data d2) {//compara data anterior com a data atual digitada pelo usuario
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

int isEmpty(ListNodePtr sPtr) { return sPtr == NULL; }//verifica se a lista ja teve algum compromisso digitado

void printList(ListNodePtr currentPtr) {//printa todos os meses com os compromissos
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
        printf("\n=============================\n        %s %d\n=============================\n", nomeMeses[mesAtual], anoAtual);
        printf("Dom Seg Ter Qua Qui Sex Sab\n");

        int diasTotais = diasNoMes(mesAtual, anoAtual);
        int diaSemanaInicio = diaDaSemana(1, mesAtual, anoAtual);

        for (int i = 0; i < diaSemanaInicio; i++) printf("    ");//espaço dos dias que n inicia a semana

        for (int d = 1; d <= diasTotais; d++) {
            int temEvento = 0;
            ListNodePtr check = temp;
            while (check != NULL && check->compromisso.data.ano == anoAtual && check->compromisso.data.mes == mesAtual) {
                if (check->compromisso.data.dia == d) { temEvento = 1; break; }
                check = check->nextPtr;
            }
            if (temEvento) printf("[%2d]", d); else printf(" %2d ", d);//destaca evento com []
            if ((d + diaSemanaInicio) % 7 == 0) printf("\n");//finaliza a semana
        }
        printf("\n-----------------------------\nEventos deste mes:\n");
        while (temp != NULL && temp->compromisso.data.ano == anoAtual && temp->compromisso.data.mes == mesAtual) {
            printf("Dia %02d - [ID: %3d] %s\n", temp->compromisso.data.dia, temp->compromisso.id, temp->compromisso.nome);
            temp = temp->nextPtr;
        }
    }
    printf("=============================\n\n");
}

void printMonth(ListNodePtr currentPtr, int mesBusca, int anoBusca) {//printa o mês informado
    char *nomeMeses[] = {"", "JANEIRO", "FEVEREIRO", "MARCO", "ABRIL", "MAIO", "JUNHO", 
                         "JULHO", "AGOSTO", "SETEMBRO", "OUTUBRO", "NOVEMBRO", "DEZEMBRO"};

    printf("\n=============================\n        %s %d\n=============================\n", nomeMeses[mesBusca], anoBusca);
    printf("Dom Seg Ter Qua Qui Sex Sab\n");

    int diasTotais = diasNoMes(mesBusca, anoBusca);
    int diaSemanaInicio = diaDaSemana(1, mesBusca, anoBusca);

    for (int i = 0; i < diaSemanaInicio; i++) printf("    ");

    for (int d = 1; d <= diasTotais; d++) {
        int temEvento = 0;
        ListNodePtr check = currentPtr;
        while (check != NULL) {
            if (check->compromisso.data.ano == anoBusca && check->compromisso.data.mes == mesBusca && check->compromisso.data.dia == d) {
                temEvento = 1; break;
            }
            check = check->nextPtr;
        }
        if (temEvento) printf("[%2d]", d); else printf(" %2d ", d);
        if ((d + diaSemanaInicio) % 7 == 0) printf("\n");
    }
    printf("\n-----------------------------\nEventos deste mes:\n");
    int encontrou = 0;
    ListNodePtr temp = currentPtr;
    while (temp != NULL) {
        if (temp->compromisso.data.ano == anoBusca && temp->compromisso.data.mes == mesBusca) {
            printf("Dia %02d - [ID: %3d] %s\n", temp->compromisso.data.dia, temp->compromisso.id, temp->compromisso.nome);
            encontrou = 1;
        }
        temp = temp->nextPtr;
    }
    if (!encontrou) printf("Nenhum evento programado.\n");
    printf("=============================\n\n");
}