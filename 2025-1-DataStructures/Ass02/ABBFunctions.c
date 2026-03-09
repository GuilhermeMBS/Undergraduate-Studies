#include "ABBFunctions.h"

Nodo* insere(Nodo* p, int x) {
    Nodo* aux;
    if (p == NULL) {
        aux = (Nodo*)malloc(sizeof(Nodo));
        aux->chave = x;
        aux->esq = aux->dir = NULL;
        return aux;
    }
    if (p->chave > x)
        p->esq = insere(p->esq, x);
    else if (p->chave < x)
        p->dir = insere(p->dir, x);
    return p;
}

Nodo* cria(int c, Nodo* sae, Nodo* sad) {
    Nodo* p = (Nodo*)malloc(sizeof(Nodo));
    p->chave = c;
    p->esq = sae;
    p->dir = sad;
    return p;
}

int busca(Nodo* p, int x) {
    if (p == NULL) return FALSE;
    if (p->chave == x) return TRUE;
    return busca(p->esq, x) || busca(p->dir, x);
}

void exibe_preordem(Nodo* p) {
    if (p == NULL) return;
    printf("ptr_no=%p, chave=%d esq=%p dir=%p\n", p, p->chave, p->esq, p->dir);
    exibe_preordem(p->esq);
    exibe_preordem(p->dir);
}

void exibe_simetrica(Nodo* p) {
    if (p == NULL) return;
    exibe_simetrica(p->esq);
    printf("ptr_no=%p, chave=%d esq=%p dir=%p\n", p, p->chave, p->esq, p->dir);
    exibe_simetrica(p->dir);
}

int deletamin(Nodo** pt) {
    int min;
    Nodo* aux;
    if ((*pt)->esq == NULL) {
        min = (*pt)->chave;
        aux = (*pt);
        (*pt) = (*pt)->dir;
        free(aux);
        return min;
    }
    return deletamin(&((*pt)->esq));
}

void deleta(int x, Nodo** pt) {
    Nodo* aux;
    if ((*pt) != NULL)
        if (x < (*pt)->chave)
            deleta(x, &((*pt)->esq));
        else if (x > (*pt)->chave)
            deleta(x, &((*pt)->dir));
        else {
            if ((*pt)->esq == NULL) {
                aux = (*pt);
                (*pt) = (*pt)->dir;
                free(aux);
            }
            else if ((*pt)->dir == NULL) {
                aux = (*pt);
                (*pt) = (*pt)->esq;
                free(aux);
            }
            else {
                (*pt)->chave = deletamin(&((*pt)->dir));
            }
        }
}