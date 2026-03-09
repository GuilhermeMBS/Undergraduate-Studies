#include <stdio.h>
#include <stdlib.h>


struct nodo {
    int chave;
    struct nodo* esq;
    struct nodo* dir;
    struct nodo* pai;
};
typedef struct nodo Nodo;


Nodo* insere(Nodo* p, int x) {
    Nodo* aux;
    if (p == NULL) {
        aux = (Nodo*)malloc(sizeof(Nodo));
        aux->chave = x;
        aux->esq = aux->dir = NULL;
        aux->pai = NULL;
        return aux;
    }
    if (p->chave > x)
        p->esq = insere(p->esq, x);
    else if (p->chave < x)
        p->dir = insere(p->dir, x);
    return p;
}


void trocaSub(Nodo* p) {
    if (p == NULL) return;
    Nodo* tmp = p->dir;
    p->dir = p->esq; p->esq = tmp;
    trocaSub(p->esq);
    trocaSub(p->dir);
    return;
}


void incluiPai(Nodo* p) {
    if (p == NULL) return;
    if (p->dir != NULL) p->dir->pai = p;
    if (p->esq != NULL) p->esq->pai = p;
    incluiPai(p->esq);
    incluiPai(p->dir);
    return;
}


void freeTree(Nodo* p) {
    if (p == NULL) return;
    freeTree(p->esq);
    freeTree(p->dir);
    free(p);
}


void exibe_preordem(Nodo* p) {
    if (p == NULL) return;
    printf("ptr_no=%p, chave=%d, esq=%p, dir=%p, pai=%p\n", p, p->chave, p->esq, p->dir, p->pai);
    exibe_preordem(p->esq);
    exibe_preordem(p->dir);
}


int main() {
    Nodo* raiz = NULL;
    int nodes[] = {10, 5, 20, 1, 7, 15, 21, -1, -1, 6, 11, -1, -1, 22, -1, -1, -1, -1, -1, -1};
    int size = sizeof(nodes) / sizeof(nodes[0]);

    for (int i = 0; i < size; i++) {
        if (nodes[i] == -1) continue;
        raiz = insere(raiz, nodes[i]);
    }

    printf("ARVORE ABB ORIGINAL EM PRE-ORDEM:\n\n");
    exibe_preordem(raiz);

    trocaSub(raiz);
    printf("\n\nARVORE ABB TROCADA EM PRE-ORDEM\n\n");
    exibe_preordem(raiz);

    incluiPai(raiz);
    printf("\n\nARVORE ABB TROCADA COM PAI EM PRE-ORDEM:\n\n");
    exibe_preordem(raiz);

    freeTree(raiz); 
  
    return 0;
}