#include <stdio.h>
#include "ABBFunctions.h"


void freeTree(Nodo* p) {
    if (p == NULL) return;
    freeTree(p->esq); // Percorre a esquerda primeiro
    freeTree(p->dir); // Percorre a direita
    free(p); // Após liberar os filhos, libera o nó
}


int checkABB(Nodo* p) { // 1 = TRUE e 0 = FALSE
    if (p == NULL) return 1; // Se a árvore for vazia (também é ABB)

    if (p->esq != NULL) { // Verificar se é NULL pra acessar sua chave
        if (p->esq->chave < p->chave) { // Confere se o nó da esquerda é menor que o da direita
            int vare = checkABB(p->esq); // Se for menor, percorre o lado esquerdo
            if (!vare) return 0; // Se quebrou a regra da ABB, vai voltar todos os ramos com 0
        }
        else return 0; // Se o nó da esquerda é maior que o nó (quebra a regra)
    }

    if (p->dir != NULL) { // Análogo para o lado direito
        if (p->dir->chave > p->chave) {
            int vard = checkABB(p->dir);
            if (!vard) return 0;
        }
        else return 0;
    }

    return 1; // Se chegar numa folha, não quebrou a regra
}


int checkAVL(Nodo* p, int* height) { // 1 = TRUE e 0 = FALSE
    if (p == NULL) { 
        *height = -1; // Convenção de altura para o NULL na AVL (pois folha é 0)
        return 1; // Árvore vazia também é AVL
    }

    int hl, hr; // Guardar alturas da esquerda e da direita
    if (!checkAVL(p->esq, &hl)) return 0; // Percorre o lado esquerdo
    if (!checkAVL(p->dir, &hr)) return 0; // Percorre o lado direito

    int balance = hl - hr; // Confere fator de balanceamento
    *height = 1 + (hl > hr ? hl : hr); // Altura é sempre 1 + máx(hl,hd), para que folha tenha altura 0

    return (balance >= -1 && balance <= 1); // Confere se o balanceamento é -1, 0 ou 1
}


int main() {
    Nodo* raiz = NULL; // Usado na Questão 2
    int nodes[] = {10, 5, 20, 1, 7, 15, 21, -1, -1, 6, 8, 11, -1, -1, 22, -1, -1, -1, -1, -1, -1, -1, -1}; // Fila da Questão 2
    int size = sizeof(nodes) / sizeof(nodes[0]); // Tamanho do array "nodes"
    int h; // Variável global para alocar altura no checkAVL

    // QUESTÃO 1

    // Letra A
    Nodo* n1 = cria(1, NULL, NULL);
    Nodo* n6 = cria(6, NULL, NULL);
    Nodo* n8 = cria(8, NULL, NULL);
    Nodo* n11 = cria(11, NULL, NULL);
    Nodo* n22 = cria(22, NULL, NULL);
    Nodo* n7 = cria(7, n6, n8);
    Nodo* n15 = cria(15, n11, NULL);
    Nodo* n21 = cria(21, NULL, n22);
    Nodo* n5 = cria(5, n1, n7);
    Nodo* n20 = cria(20, n15, n21);
    Nodo* n10 = cria(10, n5, n20);

    // Letra D(A)
    printf("LETRA A\n\nPre-ordem:\n");
    exibe_preordem(n10);
    printf("\nOrdem simetrica:\n");
    exibe_simetrica(n10);

    int q3_arvore1 = checkABB(n10); // Questão 3(A)
    int q4_arvore1 = q3_arvore1 ? checkAVL(n10, &h) : 0; // Questão 4(A)

    // Letra B
    n5->esq = NULL; // Tira n1 da árvore
    free(n1); // Libera sua memória

    // Letra D(B)
    printf("\n\nLETRA B\n\nPre-ordem:\n");
    exibe_preordem(n10);
    printf("\nOrdem simetrica:\n");
    exibe_simetrica(n10);

    int q3_arvore2 = checkABB(n10); // Questão 3(B)
    int q4_arvore2 = q3_arvore2 ? checkAVL(n10, &h) : 0; // Questão 4(B)

    // Letra C
    n1 = cria(1, NULL, NULL); // Cria o n1
    n5->esq = n1; // Devolve n1 pra árvore (reconstruir árvore da letra A)
    n22->chave = 17; // Troca o valor do nó 22

    // Letra D(C)
    printf("\n\nLETRA C\n\nPre-ordem:\n");
    exibe_preordem(n10);
    printf("\nOrdem simetrica:\n");
    exibe_simetrica(n10);

    int q3_arvore3 = checkABB(n10); // Questão 3(C)
    int q4_arvore3 = q3_arvore3 ? checkAVL(n10, &h) : 0; // Questão 4(C)

    // QUESTÃO 2
    for (int i = 0; i < size; i++) { // Percorre o vetor de inserção
        if (nodes[i] == -1) continue; // -1 = NULL
        raiz = insere(raiz, nodes[i]); // Insere na árvore
    }

    // Conferindo Árvore da Questão 2
    printf("\n\nQUESTAO 2\n\nPre-ordem:\n");
    exibe_preordem(raiz);
    printf("\nOrdem simetrica:\n");
    exibe_simetrica(raiz);

    // QUESTÃO 3
    printf("\n\nTESTE ARVORE ABB\n");
    (q3_arvore1 == 1) ? printf("Arvore 1: ABB\n") : printf("Arvore 1: Nao ABB\n");
    (q3_arvore2 == 1) ? printf("Arvore 2: ABB\n") : printf("Arvore 2: Nao ABB\n");
    (q3_arvore3 == 1) ? printf("Arvore 3: ABB\n") : printf("Arvore 3: Nao ABB\n");

    // QUESTÃO 4
    printf("\nTESTE ARVORE AVL\n");
    (q4_arvore1 == 1) ? printf("Arvore 1: AVL\n") : printf("Arvore 1: Nao AVL\n");
    (q4_arvore2 == 1) ? printf("Arvore 2: AVL\n") : printf("Arvore 2: Nao AVL\n");
    (q4_arvore3 == 1) ? printf("Arvore 3: AVL\n") : printf("Arvore 3: Nao AVL\n");

    // Limpando Árvores
    freeTree(raiz); // Libera memória da top down
    freeTree(n10); // Libera memória da bottom up

    return 0;
}