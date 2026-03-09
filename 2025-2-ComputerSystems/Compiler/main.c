#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "gera_codigo.h"

#define forn(i,s,n) for(int i = s; i < n; i++)

int main(int argc, char *argv[]) {
    FILE *fp;
    funcp funcLBS;
    unsigned char* code;
    // unsigned char code[300];
    int res;

    // Aloca memória executável (RWX) usando VirtualAlloc
    code = (unsigned char*) VirtualAlloc(
        NULL,                // endereço sugerido (NULL = qualquer)
        300,                 // tamanho em bytes
        MEM_COMMIT | MEM_RESERVE, 
        PAGE_EXECUTE_READWRITE // permite leitura, escrita e execução
    );

    if (code == NULL) {
        printf("Erro em VirtualAlloc\n");
        return 1;
    }

    forn(i, 0, 300) code[i] = 0;

    /* Abre o arquivo para leitura */
    char* nome_arq = "prog1.txt";
    fp = fopen(nome_arq, "r");
    /* Gera o codigo */
    gera_codigo(fp, code, &funcLBS);
    if (funcLBS == NULL) {
        printf("Erro na geracao\n");
    }
    // forn(i,0, 300) printf("0x%02X\t", code[i]);
    printf("\n");
    /* Chama a função gerada */
    int val = 10;
    res = (*funcLBS)(val);
    printf("%d\n", res);
    return 0;
}