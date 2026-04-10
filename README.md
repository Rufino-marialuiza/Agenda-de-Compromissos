# 📅 Agenda de Compromissos
Este projeto é um sistema de gerenciamento de compromissos desenvolvido como parte dos requisitos avaliativos da disciplina de Linguagem de Programação 1 na Universidade Estadual da Paraíba (UEPB).

O objetivo principal é oferecer uma ferramenta de linha de comando (CLI) que permita organizar tarefas, prazos e eventos de forma estruturada.

# 🚀 Funcionalidades
O sistema permite gerenciar o ciclo de vida completo de um compromisso:

Cadastro de Compromissos: Registro de novas tarefas com detalhes como data e descrição.

Listagem Ordenada: Visualização de todos os compromissos agendados.

Busca e Consulta: Localização de compromissos específicos.

Edição e Remoção: Atualização de informações ou exclusão de registros antigos.

Persistência de Dados: (Caso o projeto salve em arquivo) Armazenamento das informações para consulta posterior.

# 🛠️ Tecnologias Utilizadas
Linguagem C: Linguagem base para toda a lógica de programação.

Modularização: Organização do código em múltiplos arquivos (main.c, biblioteca.c e biblioteca.h) para melhor manutenção e legibilidade.

Estrutura de Dados: Uso de listas (ou vetores dinâmicos) para manipulação dos dados em memória.

# 📁 Estrutura do Projeto
main.c: Ponto de entrada do programa. Contém o menu principal e a orquestração das funções.

biblioteca.h: Arquivo de cabeçalho contendo as definições de estruturas (structs) e os protótipos das funções.

biblioteca.c: Implementação detalhada de toda a lógica de manipulação da agenda.
