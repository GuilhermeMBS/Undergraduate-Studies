#ifndef ABBFUNCTIONS_H
#define ABBFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

struct nodo {
    int chave;
    struct nodo* esq;
    struct nodo* dir;
};
typedef struct nodo Nodo;

// Fun��es da �rvore
Nodo* insere(Nodo* p, int x);
Nodo* cria(int c, Nodo* sae, Nodo* sad);
int busca(Nodo* p, int x);
void exibe_preordem(Nodo* p);
void exibe_simetrica(Nodo* p);
void deleta(int x, Nodo** pt);
int deletamin(Nodo** pt);

#endif