#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura da sala (árvore binária)s

typedef struct Sala {
    char nome[50];
    char pista[100];          // Pista encontrada neste cômodo
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

//Estrutura da árvore de pistas (BST)

typedef struct NoPista {
    char pista[100];
    struct NoPista* esquerda;
    struct NoPista* direita;
} NoPista;


//criarSala()
    
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        nova->pista[0] = '\0'; // sala sem pista

    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}


// inserirPista()
  

NoPista* inserirPista(NoPista* raiz, const char* pista) {
    if (pista[0] == '\0')
        return raiz;  // sala sem pista → nada é inserido

    if (raiz == NULL) {
        NoPista* novo = (NoPista*) malloc(sizeof(NoPista));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else
        raiz->direita  = inserirPista(raiz->direita, pista);

    return raiz;
}


// exibirPistas()

void exibirPistas(NoPista* raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}


// explorarSalasComPistas()

void explorarSalasComPistas(Sala* atual, NoPista** arvorePistas) {
    char opcao;

    while (atual != NULL) {
        printf("Você está em: %s\n", atual->nome);

        // Exibe pista do cômodo (se houver)
        if (strlen(atual->pista) > 0) {
            printf(">> Pista encontrada: \"%s\"\n", atual->pista);

            // insere na árvore de pistas
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        // Opções de navegação
        printf("Escolha um caminho:\n");
        if (atual->esquerda) printf("  (e) Ir para a esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita)  printf("  (d) Ir para a direita  -> %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        switch (opcao) {
            case 'e':
                if (atual->esquerda)
                    atual = atual->esquerda;
                else
                    printf("Não há caminho à esquerda!\n");
                break;

            case 'd':
                if (atual->direita)
                    atual = atual->direita;
                else
                    printf("Não há caminho à direita!\n");
                break;

            case 's':
                printf("Encerrando exploração...\n");
                return;

            default:
                printf("Opção inválida!\n");
        }
    }
}


// main()

int main() {

    // Criação das salas com pistas associadas
    Sala* hall         = criarSala("Hall de Entrada",       "Pegadas úmidas no chão");
    Sala* salaEstar    = criarSala("Sala de Estar",         "Uma luva caída");
    Sala* salaJantar   = criarSala("Sala de Jantar",        "Cadeira caída ao chão");
    Sala* cozinha      = criarSala("Cozinha",               "Faca fora do lugar");
    Sala* biblioteca   = criarSala("Biblioteca",            "Livro aberto em página rasgada");
    Sala* escritorio   = criarSala("Escritório",            "Mesa revirada");

    // Conexões da árvore da mansão
    hall->esquerda = salaEstar;
    hall->direita  = salaJantar;

    salaEstar->esquerda = cozinha;
    salaEstar->direita  = biblioteca;

    salaJantar->esquerda = escritorio;

    // Árvore de pistas (BST inicialmente vazia)
    NoPista* arvorePistas = NULL;

    printf("\n=== Detective Quest: Coleta de Pistas ===\n\n");

    // Exploração da mansão
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibe pistas coletadas
    printf("PISTAS COLETADAS (A–Z)\n");
    exibirPistas(arvorePistas);

    return 0;
}
