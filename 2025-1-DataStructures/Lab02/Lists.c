#include <stdio.h>
#include <stdlib.h>

#define SIZE_IMP 10
#define SIZE_PAR 10

typedef struct numlist Nlist;

struct numlist
{
	int num;
	struct numlist* nxt;
};


Nlist* insereNum(int n, Nlist* lst) {
    Nlist* novo = (Nlist*)malloc(sizeof(Nlist));

    if (novo != NULL) { //Caso o malloc tenha dado certo, montar a estrutura
        novo->num = n;
        novo->nxt = NULL;
        
        if (lst != NULL) { //Caso a lista não seja vazia
            Nlist* p; //Ponteiro para andar até o final da lista
            for (p = lst; p->nxt != NULL; p = p->nxt); //Andar até o final (um while mais enxuto)
            p->nxt = novo; //Importante o ponteiro ter sido inicializado antes do for loop para usá-lo aqui
        }
        
        else return novo; //Caso a lista seja vazia, retorna o novo elemento como a head
    }

    return lst; //Caso o malloc tenha dado errado, retorna NULL para verificar dar exit na main. Caso contrário, retorna a head, como de praxe
}


void montaLista(int size, int* arr, Nlist** lst) //Função simples e auto explicativa
{
	for (int i = 0; i < size; i++) {
        *lst = insereNum(arr[i], *lst); //A head será sempre atualizada para a head retornada da função insereNum (inserindo sempre os elementos em ordem do vetor)
        if (*lst == NULL) {
            printf("Erro de memoria em %d\n", arr[i]);
            exit(1);
        }
    };
}


void imprimeLista(Nlist* lst)
{
	if (lst != NULL) { //Caso base
		printf("%d ", lst->num);
		imprimeLista(lst->nxt); //Chama recursivamente com o ponteiro do próximo
	}

	else printf("\n");
}


void imprimeVetor(int* v, int size) {
    if (size > 0) { //Caso base: tamanho a ser impresso = 0
        printf("%d ", v[0]);
        imprimeVetor(v + 1, size - 1);  //Recursividade iterando o ponteiro para o próximo local da memória e atualizando o tamanho do vetor
    }
}


void eliminaNum(Nlist** lst) //
{
	Nlist* p = *lst; //Cria um ponteiro temporário para guardar o endereço a ser liberado
	*lst = (*lst)->nxt; //Atualiza a lista
	free(p); //Libera a memória do endereço guardado
}


void intercalaLista(Nlist** pares, Nlist** impares, Nlist** intercalado)
{
    if (*pares == NULL || (*impares != NULL && (*impares)->num < (*pares)->num)) { //Se os pares acabarem OU se o ímpar for menor que o par (e ainda houver ímpar, para podermos acessar a memória com seu valor)
        *intercalado = insereNum((*impares)->num, *intercalado); //Adiciona o menor ímpar (head)
        if (*intercalado == NULL) { //Conferindo o malloc
            printf("Erro de memoria em ao alocar impar\n");
            exit(1);
        }
        eliminaNum(impares); //Retira e libera o ímpar usado (já atualiza a head)
    } 
    
    else { //Caso haja par e ele seja maior que o ímpar (negação da anterior). Não precisa conferir igualdade, pois par != ímpar sempre. Análogo ao processo anterior
        *intercalado = insereNum((*pares)->num, *intercalado);
        if (*intercalado == NULL) {
            printf("Erro de memoria ao alocar par\n");
            exit(1);
        }
        eliminaNum(pares);
    }
}


void intercalaVetor(int* pares, int* impares, Nlist** intercalado)
{
    int i = 0, j = 0; //Two pointers -> ambos começando na head (dos pares e dos ímpares, respectivamente)
     
    while (i + j < (SIZE_IMP+SIZE_PAR)) { //Enquanto não forem todos os elementos das duas listas
        if (j == SIZE_IMP || (i < SIZE_PAR && pares[i] < impares[j])) { //Caso o ponteiro dos ímpares chegue no final OU o par seja menor que o ímpar (e o par não pode ter chegado no final, para não acessar valores indesejados)
            *intercalado = insereNum(pares[i], *intercalado); //Adiciona o menor par na lista encadeada
            if (*intercalado == NULL) { //Conferência de memória do malloc
                printf("Erro de memoria ao alocar par\n");
                exit(1);
            }
            pares[i++] = -1; //Atualiza o valor (eliminar do vetor) e anda com o ponteiro
        }
        else { //Caso os pares tenham acabado ou o ímpar seja maior (negação da afirmação anterior). Processo análogo ao de cima
            *intercalado = insereNum(impares[j], *intercalado);
            if (*intercalado == NULL) {
                printf("Erro de memoria ao alocar impar\n");
                exit(1);
            }
            impares[j++] = -1;
        }
        
        printf("\nApos interacao %d:", i + j);
		printf("\nVetor dos pares: "); imprimeVetor(pares, SIZE_PAR);
		printf("\nVetor dos impares: "); imprimeVetor(impares, SIZE_IMP);
		printf("\nNova lista encadeada intercalada: "); imprimeLista(*intercalado);
    }
}


int main(void)
{
	//Inicializações
	int npares[SIZE_PAR] = { 10, 20, 30, 34, 36, 38, 50, 60, 62, 70 };
	int nimpares[SIZE_IMP] = { 9, 11, 13, 33, 35, 41, 43, 55, 61, 71 };
	
	Nlist* pares = NULL;
	Nlist* impares = NULL;
	Nlist* intercalado = NULL;
	
	//Montagem das listas
	montaLista(SIZE_PAR, npares, &pares); montaLista(SIZE_IMP, nimpares, &impares);

    //Listas encadeadas dos pares e ímpares iniciais
	printf("Lista encadeada dos pares: "); imprimeLista(pares); 
	printf("Lista encadeada dos impares: "); imprimeLista(impares); 
	
	//Intercalagem
	for (int i = 0; i < (SIZE_PAR + SIZE_IMP); i++) { //Enquanto não forem todos os elementos de ambos os vetores
		intercalaLista(&pares, &impares, &intercalado); //Compara e faz a intercalagem
		printf("\nApos interacao %d:\n", i + 1); //Sequência de impressões
		printf("Lista encadeada dos pares: "); imprimeLista(pares);
		printf("Lista encadeada dos impares: "); imprimeLista(impares);
		printf("Lista encadeada intercalada: "); imprimeLista(intercalado);
	}

    //Limpando lista
    for (int i = 0; i < (SIZE_PAR + SIZE_IMP); i++) eliminaNum(&intercalado);
    printf("\nLista encadeada intercalada apos limpeza: "); imprimeLista(intercalado); //Conferindo se a lista está zerada

    //Refazendo com vetores
    intercalaVetor(npares, nimpares, &intercalado);

    //Liberando memoria alocada
    for (int i = 0; i < (SIZE_PAR + SIZE_IMP); i++) eliminaNum(&intercalado);
    
	return 0;
}