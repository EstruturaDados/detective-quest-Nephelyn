#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Estrutura da sala (nó da árvore binária)

typedef struct Sala {
    char nome[50];          // Nome da sala
    struct Sala* esquerda;  // Caminho à esquerda
    struct Sala* direita;   // Caminho à direita
} Sala;



// Cria dinamicamente uma sala (nó da árvore)

Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}


// explorarSalas()

void explorarSalas(Sala* atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

       
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais caminhos a seguir. Fim da exploração\n");
            return;
        }

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
                printf("Exploração encerrada.\n");
                return;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
}


// main()

int main() {
   
    Sala* hall           = criarSala("Hall de Entrada");
    Sala* salaEstar      = criarSala("Sala de Estar");
    Sala* salaJantar     = criarSala("Sala de Jantar");
    Sala* cozinha        = criarSala("Cozinha");
    Sala* biblioteca     = criarSala("Biblioteca");
    Sala* escritorio     = criarSala("Escritório");

    // Construção manual da árvore
  

    hall->esquerda = salaEstar;
    hall->direita  = salaJantar;

    salaEstar->esquerda   = cozinha;
    salaEstar->direita    = biblioteca;

    salaJantar->esquerda  = escritorio;
 

    // Início da exploração
    printf("Detective Quest: Exploração da Mansão\n");
    explorarSalas(hall);

    free(cozinha);
    free(biblioteca);
    free(escritorio);
    free(salaEstar);
    free(salaJantar);
    free(hall);

    return 0;
}