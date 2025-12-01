#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOME 50
#define MAX_PISTA 100
#define HASH_SIZE 101

//Estruturas
typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

typedef struct NoPista {
    char pista[MAX_PISTA];
    struct NoPista* esquerda;
    struct NoPista* direita;
} NoPista;

typedef struct HashNode {
    char chave[MAX_PISTA];       // texto da pista
    char suspeito[MAX_NOME];     // nome do suspeito associado
    struct HashNode* proximo;
} HashNode;

HashNode* tabelaHash[HASH_SIZE];

// Protótipos
Sala* criarSala(const char* nome, const char* pista);
void explorarSalas(Sala* atual, NoPista** arvorePistas);
NoPista* inserirPista(NoPista* raiz, const char* pista);
int existePista(NoPista* raiz, const char* pista);
void exibirPistas(NoPista* raiz);
unsigned long hash_djb2(const char* str);
void inserirNaHash(const char* pista, const char* suspeito);
const char* encontrarSuspeito(const char* pista);
int contarPistasQueApontam(NoPista* raiz, const char* acusado);
int verificarSuspeitoFinal(NoPista* arvorePistas, const char* acusado);
void liberarPistas(NoPista* raiz);
void liberarSalas(Sala* raiz);
void liberarHash(void);



//criarSala()

Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) { fprintf(stderr, "Erro de memória\n"); exit(1); }
    strncpy(nova->nome, nome, MAX_NOME-1); nova->nome[MAX_NOME-1] = '\0';
    if (pista) { strncpy(nova->pista, pista, MAX_PISTA-1); nova->pista[MAX_PISTA-1] = '\0'; }
    else nova->pista[0] = '\0';
    nova->esquerda = nova->direita = NULL;
    return nova;
}

//inserirPista()

NoPista* inserirPista(NoPista* raiz, const char* pista) {
    if (!pista || pista[0]=='\0') return raiz;
    if (raiz == NULL) {
        NoPista* n = (NoPista*) malloc(sizeof(NoPista));
        strncpy(n->pista, pista, MAX_PISTA-1); n->pista[MAX_PISTA-1] = '\0';
        n->esquerda = n->direita = NULL;
        return n;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0) return raiz; // já coletada
    if (cmp < 0) raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

//existePista()

int existePista(NoPista* raiz, const char* pista) {
    if (!raiz) return 0;
    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0) return 1;
    return (cmp < 0) ? existePista(raiz->esquerda, pista) : existePista(raiz->direita, pista);
}

//exibirPistas()

void exibirPistas(NoPista* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

//hash_djb2()
unsigned long hash_djb2(const char* str) {
    unsigned long hash = 5381;
    unsigned char c;
    while ((c = (unsigned char)*str++)) hash = ((hash << 5) + hash) + c;
    return hash;
}

//inserirNaHash()

void inserirNaHash(const char* pista, const char* suspeito) {
    unsigned long h = hash_djb2(pista) % HASH_SIZE;
    HashNode* cur = tabelaHash[h];
    while (cur) {
        if (strcmp(cur->chave, pista) == 0) {
            strncpy(cur->suspeito, suspeito, MAX_NOME-1); cur->suspeito[MAX_NOME-1] = '\0';
            return;
        }
        cur = cur->proximo;
    }
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strncpy(novo->chave, pista, MAX_PISTA-1); novo->chave[MAX_PISTA-1] = '\0';
    strncpy(novo->suspeito, suspeito, MAX_NOME-1); novo->suspeito[MAX_NOME-1] = '\0';
    novo->proximo = tabelaHash[h];
    tabelaHash[h] = novo;
}

//encontrarSuspeito()

const char* encontrarSuspeito(const char* pista) {
    unsigned long h = hash_djb2(pista) % HASH_SIZE;
    HashNode* cur = tabelaHash[h];
    while (cur) {
        if (strcmp(cur->chave, pista) == 0) return cur->suspeito;
        cur = cur->proximo;
    }
    return NULL;
}

//explorarSalas()

void explorarSalas(Sala* atual, NoPista** arvorePistas) {
    char opcao;
    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf(">> Pista encontrada: \"%s\"\n", atual->pista);
            const char* suspeito = encontrarSuspeito(atual->pista);
            if (suspeito) {
                printf("   (Esta pista está associada ao suspeito: %s)\n", suspeito);
            } else {
                printf("   (Nenhum suspeito associado a esta pista)\n");
            }

            if (!existePista(*arvorePistas, atual->pista)) {
                *arvorePistas = inserirPista(*arvorePistas, atual->pista);
                printf("   Pista adicionada ao caderno.\n");
            } else {
                printf("   Pista já presente no caderno (não duplicada).\n");
            }
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        printf("\nEscolha um caminho:\n");
        if (atual->esquerda) printf("  (e) Ir para a esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita)  printf("  (d) Ir para a direita  -> %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);
        int ch; while ((ch = getchar()) != '\n' && ch != EOF); // limpa buffer

        if (opcao=='e' || opcao=='E') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Não há caminho à esquerda!\n");
        } else if (opcao=='d' || opcao=='D') {
            if (atual->direita) atual = atual->direita;
            else printf("Não há caminho à direita!\n");
        } else if (opcao=='s' || opcao=='S') {
            printf("Encerrando exploração...\n");
            return;
        } else {
            printf("Opção inválida! Use 'e', 'd' ou 's'.\n");
        }
    }
}

//contarPistasQueApontam()

int contarPistasQueApontam(NoPista* raiz, const char* acusado) {
    if (!raiz) return 0;
    int cnt = 0;
    cnt += contarPistasQueApontam(raiz->esquerda, acusado);
    const char* s = encontrarSuspeito(raiz->pista);
    if (s && strcmp(s, acusado) == 0) cnt++;
    cnt += contarPistasQueApontam(raiz->direita, acusado);
    return cnt;
}

// verificarSuspeitoFinal()

int verificarSuspeitoFinal(NoPista* arvorePistas, const char* acusado) {
    int total = contarPistasQueApontam(arvorePistas, acusado);
    printf("\nPistas coletadas que apontam para '%s': %d\n", acusado, total);
    return (total >= 2) ? 1 : 0;
}

void liberarPistas(NoPista* raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

void liberarSalas(Sala* raiz) {
    if (!raiz) return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);
}

void liberarHash(void) {
    for (int i=0;i<HASH_SIZE;i++) {
        HashNode* cur = tabelaHash[i];
        while (cur) {
            HashNode* t = cur;
            cur = cur->proximo;
            free(t);
        }
        tabelaHash[i] = NULL;
    }
}

//main
int main(void) {
    /* Montagem das salas (mapa fixo) */
    Sala* hall = criarSala("Hall de Entrada", "Pegadas úmidas no chão");
    Sala* salaEstar = criarSala("Sala de Estar", "Uma luva caída");
    Sala* salaJantar = criarSala("Sala de Jantar", "Cadeira caída ao chão");
    Sala* cozinha = criarSala("Cozinha", "Faca fora do lugar");
    Sala* biblioteca = criarSala("Biblioteca", "Livro aberto em página rasgada");
    Sala* escritorio = criarSala("Escritório", "Mesa revirada");

    hall->esquerda = salaEstar; hall->direita = salaJantar;
    salaEstar->esquerda = cozinha; salaEstar->direita = biblioteca;
    salaJantar->esquerda = escritorio;

    /* Inicializa hash */
    for (int i=0;i<HASH_SIZE;i++) tabelaHash[i] = NULL;


inserirNaHash("Pegadas úmidas no chão", "Jardineiro");
inserirNaHash("Uma luva caída", "Mordomo");
inserirNaHash("Cadeira caída ao chão", "Dama");
inserirNaHash("Faca fora do lugar", "Cozinheiro");
inserirNaHash("Livro aberto em página rasgada", "Professor");
inserirNaHash("Mesa revirada", "Camareira");

    NoPista* arvorePistas = NULL;

    printf("\n=== Detective Quest: Julgamento Final (Exploração) ===\n");
    explorarSalas(hall, &arvorePistas);

    printf("\n=== PISTAS COLETADAS (A–Z) ===\n");
    if (arvorePistas == NULL) printf("Nenhuma pista coletada.\n");
    else exibirPistas(arvorePistas);

    /* Fase de acusação */
    char acusado[MAX_NOME];
    printf("\nInforme o nome do suspeito que deseja acusar ");
    if (fgets(acusado, sizeof(acusado), stdin) == NULL) acusado[0]='\0';
    size_t len = strlen(acusado);
    if (len>0 && acusado[len-1]=='\n') acusado[len-1]='\0';

    if (strlen(acusado) == 0) {
        printf("Nenhum suspeito informado. Encerrando.\n");
    } else {
        if (verificarSuspeitoFinal(arvorePistas, acusado)) {
            printf("\nACUSAÇÃO SUSTENTADA: existem pelo menos 2 pistas apontando para %s.\n", acusado);
        } else {
            printf("\nACUSAÇÃO NÃO SUSTENTADA: menos de 2 pistas apontam para %s.\n", acusado);
        }
    }

    /* Limpeza */
    liberarPistas(arvorePistas);
    liberarHash();
    liberarSalas(hall);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
