#include <stdio.h>
#include "biblioteca.h"

int main(void) {
    NoListaPtr inicioPtr = NULL;
    int escolha;

    do {
        printf("MENU:\n");
        printf("1. Inserir Compromisso\n");
        printf("2. Excluir Compromisso\n");
        printf("3. Listar por Mes\n");
        printf("4. Modificar Compromisso\n");
        printf("5. Sair\n? ");
        scanf("%d", &escolha);

        if (escolha == 1) {
            struct Compromisso c;
            
            do {
                printf("ID (unico): ");
                scanf("%d", &c.id);
                if (idExiste(inicioPtr, c.id)) {
                    printf("Erro: Este ID ja existe! Tente outro.\n");
                }
            } while (idExiste(inicioPtr, c.id));

            printf("Nome do Evento: ");
            scanf(" %[^\n]", c.nome);
            
            printf("Descricao do Evento: ");
            scanf(" %[^\n]", c.descricao);

            do {
                printf("Data (DD MM AAAA): ");
                scanf("%d %d %d", &c.data.dia, &c.data.mes, &c.data.ano);
                if (!validarData(c.data.dia, c.data.mes, c.data.ano)) {
                    printf("Data invalida! Use Dia(1-31), Mes(1-12) e Ano(>=2026).\n");
                }
            } while (!validarData(c.data.dia, c.data.mes, c.data.ano));

            inserir(&inicioPtr, c);
            imprimirLista(inicioPtr);

        } else if (escolha == 2) {
            int idBusca;
            printf("ID para excluir: ");
            scanf("%d", &idBusca);
            char res = excluirNo(&inicioPtr, idBusca);
            if (res != '\0') printf("Sucesso! Removido evento com inicial '%c'.\n", res);
            else printf("ID nao encontrado.\n");
            imprimirLista(inicioPtr);

        } else if (escolha == 3) {
            int mBusca, aBusca;
            printf("Digite o mes e ano desejados (MM AAAA): ");
            scanf("%d %d", &mBusca, &aBusca);
            
            if (mBusca < 1 || mBusca > 12 || aBusca < 2026) {
                printf("Data invalida para busca.\n\n");
            } else {
                imprimirMes(inicioPtr, mBusca, aBusca);
            }
        } else if(escolha == 4){
            modificarCompromisso(&inicioPtr);
            imprimirLista(inicioPtr);
        }

    } while (escolha != 5);

    return 0;
}
