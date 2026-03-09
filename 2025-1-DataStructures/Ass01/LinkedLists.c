#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct patient {
    int ind;
    int color; // Vermelha = 1, Amarela = 2, Verde = 3
    struct patient* next;
};
typedef struct patient Patient;


Patient* insert(Patient* lst, int pos, int clr) {
    Patient* new_node = (Patient*)malloc(sizeof(Patient));
    
    if (new_node != NULL) { // Teste de alocação de memória
        // Inicialização do conteúdo do novo paciente
        new_node->ind = pos;
        new_node->color = clr;
        new_node->next = NULL;

        // Caso a inserção seja na head da lista
        if (lst == NULL || clr < lst->color) {
            new_node->next = lst;
            return new_node;
        }

        Patient* curr = lst;
        while (curr->next != NULL && curr->next->color <= clr) curr = curr->next; // Andando na lista até encontrar uma cor "maior" que a do novo paciente
        new_node->next = curr->next;  // Inserir o paciente
        curr->next = new_node;
    }
    else exit(1);
    return lst; // Retornar a head
}


void removePatient(Patient** lst, int ord) {
    Patient* curr = *lst;
    Patient* prev = NULL;

    while (curr != NULL) {
        if (curr->ind == ord) {
            (prev == NULL) ? (*lst = curr->next) : (prev->next = curr->next); // Remover ou head ou no meio
            free(curr);
            return;
        }
        prev = curr; curr = curr->next; // Anda com os ponteiros
    }
}


void printList(Patient* lst) {
    printf("\nLista Atualizada:\n");
    while (lst != NULL) {
        char* pcolor; 
        if (lst->color == 1) pcolor = "Vermelha";
        else if (lst->color == 2) pcolor = "Amarela";
        else pcolor = "Verde";
        printf("%d - %s\n", lst->ind, pcolor);
        lst = lst->next;
    }
}


int main(void) {
    // Inicializações
    Patient* PList = NULL;
    char cntrl;
    int pos;
    char ord[64];

    // Abertura do arquivo com verificação de erro
    FILE* fp = fopen("dados_trabalho1.txt", "r");
    if (fp == NULL) {
        printf("File oppening error");
        exit(2);
    }

    // Leitura do arquivo
    while (fscanf(fp, " %c-%d-%[^\n]", &cntrl, &pos, ord) == 3) {
        if (cntrl == 'c') { // Inserir paciente
            int clr;
            // Conversão das cores para números
            if (strcmp(ord, "vermelha") == 0) clr = 1;
            else if (strcmp(ord, "amarela") == 0) clr = 2;
            else clr = 3; // Caso seja verde

            PList = insert(PList, pos, clr);
        }
        else removePatient(&PList, pos); // Caso seja um "a"

        printList(PList);
    }

    // Free em todas as alocações dinâmicas
    while (PList != NULL) {
        Patient* tmp = PList;
        PList = PList->next;
        free(tmp);
    }

    fclose(fp); // Fechamento do arquivo
    return 0;
}
