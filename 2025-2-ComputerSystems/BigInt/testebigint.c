/* Guilherme Soares 3WC */
/* João Pedro 3WC */

#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

#define NUM_BYTES 128/8

typedef __int128_t I128;
typedef __uint128_t UI128;

// Function used to test the value signed to a BigInt
void print_big(BigInt valor) {
    for (int i = 0; i < NUM_BYTES; i++) {
        printf("%02X ", valor[i]);
    }
    printf("\n");
}


void dump(void* c, int n) {
    unsigned char* p = c;
    while (n--) {
        printf("0x%02X - ", *p);
        p++;
    }
    printf("\n");
}

// Function to get the value from a BigInt and return it to an integer (with 128 bits)
I128 atribui_BigInt(BigInt num) {
    I128 ans = 0;
    for (int i = NUM_BYTES; i >= 0; i--) ans = num[i] + (ans << 8); // This is considering the little-endian format
    return ans;
}


int bigInt_igual_long(BigInt a, I128 b) {
    return atribui_BigInt(a) == b;
}

// Function to verify big_comp2 is working as intended 
int testa_big_comp2(BigInt res, BigInt a) {
    I128 comp2 = atribui_BigInt(a);
    comp2 *= -1;

    big_comp2(res, a);
    
    return bigInt_igual_long(res, comp2); // Returns 1 if the output from big_comp2 is the same as the opposite of a. Returns 0 otherwise
}

// Generic function to verify if given a function, its output is correct
int testa_big_1_functions(void (*funcao)(BigInt, long), BigInt res, long n) {
    I128 num = n;
    funcao(res, n);
    return bigInt_igual_long(res, num);
}

// Generic function to calculate the expected value for a certain expression with 2 BigInt
I128 operacao_2_BigInt(BigInt a, BigInt b, char n) {
    I128 num1 = atribui_BigInt(a); I128 num2 = atribui_BigInt(b);
    I128 res;
    switch (n) {
        case '+':
            res = num1 + num2;
            break;
        case '-':
            res = num1 - num2;
            break;
        default:
            res = num1 * num2;
            break;
    }
    return res;
}

// Generic function to check if a BigInt function (that has 2 BigInt as input) iw working as intended
int testa_big_2_functions(void (*funcao)(BigInt, BigInt, BigInt), I128 (*funcao2)(BigInt, BigInt, char), BigInt res, BigInt a, BigInt b, char operacao) {
    I128 num = funcao2(a, b, operacao);
    funcao(res, a, b);
    return bigInt_igual_long(res, num);
}

// Function to calculate the expected result from a shift
I128 shifts_BigInt(BigInt a, int n, char operacao) {
    I128 num = 0;
    UI128 tmp = atribui_BigInt(a);
    switch (operacao) {
        case '<':
            num = tmp << n;
            break;
        case '>':
            tmp = tmp >> n;
            num = tmp;
            break;
        default:
            num = tmp;
            num = num >> n;
            break;
    }
    return num;
}

// Generic function to verify if the shift functions from BigInt are working as intended
int testa_big_shift_functions(void (*funcao)(BigInt, BigInt, int), I128 (*funcao2)(BigInt, int, char) , BigInt res, BigInt a, int n, char operacao) {
    I128 num = funcao2(a, n, operacao);
    funcao(res, a, n);
    return bigInt_igual_long(res, num);
}

int main() {
    // Array to keep track of all values that will be tested (must have at least 1 number)
    long valores[] = {0, 1, -1, 10, 16, 0xF9821, 0xACBE0A, -489456, -999999};
    int tam = sizeof(valores) / sizeof(valores[0]); // Amount of values to be tested

    // Array of amount to be shifted in the functions
    int valores_shifts[] = {0, 1, 6, 8, 15, 32, 127, 89};
    int tam_shifts = sizeof(valores_shifts) / sizeof(valores_shifts[0]); // How many shifts there are to test
    
    BigInt numeros[tam]; // Array to keep all the BigInt from the numbers typed above
    
    int resultado = 0; // Amount of tests that have been successful
    printf("Teste da atribuicao de valores:\n\t");
    for (int i = 0; i < tam; i++) { 
        if (!testa_big_1_functions(big_val, numeros[i], valores[i])) {
            printf("Erro ao alocar o valor:\n\t");
            dump(&(valores[i]), sizeof(valores[i]));
            printf("Valor retornado:\n\t");
            print_big(numeros[i]);
            exit(1);
        } else resultado++;
    }
    printf("Resultado: %d / %d\n\n", resultado, tam);

    BigInt ans;
    resultado = 0;
    printf("Teste do complemento a dois:\n\t");
    for (int i = 0; i < tam; i++) {
        if (!testa_big_comp2(ans, numeros[i])) {
            printf("Erro ao calcular o complemento a 2 do valor:\n\t");
            print_big(numeros[i]);
            printf("Valor retornado:\n\t");
            print_big(ans);
            exit(1);
        } else resultado++;
    }
    printf("Resultado: %d / %d\n\n", resultado, tam);

    resultado = 0;
    printf("Teste da soma de 2 BigInt:\n\t");
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (!testa_big_2_functions(big_sum, operacao_2_BigInt, ans, numeros[i], numeros[j], '+')) {
                printf("Erro ao somar os valores:\n\t");
                print_big(numeros[i]);
                printf("e\n\t");
                print_big(numeros[j]);
                printf("Valor retornado:\n\t");
                print_big(ans);
                exit(1);
            } else resultado++;
        }
    }
    printf("Resultado: %d / %d\n\n", resultado, tam*tam);

    resultado = 0;
    printf("Teste da subtracao de 2 BigInt:\n\t");
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (!testa_big_2_functions(big_sub, operacao_2_BigInt, ans, numeros[i], numeros[j], '-')) {
                printf("Erro ao subtrair os valores:\n\t");
                print_big(numeros[i]);
                printf("e\n\t");
                print_big(numeros[j]);
                printf("Valor retornado:\n\t");
                print_big(ans);
                exit(1);
            } else resultado++;
        }
    }
    printf("Resultado: %d / %d\n\n", resultado, tam*tam);

    resultado = 0;
    printf("Teste da multiplicacao de 2 BigInt:\n\t");
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (!testa_big_2_functions(big_mul, operacao_2_BigInt, ans, numeros[i], numeros[j], '*')) {
                printf("Erro ao multiplicar os valores:\n\t");
                print_big(numeros[i]);
                printf("e\n\t");
                print_big(numeros[j]);
                printf("Valor retornado:\n\t");
                print_big(ans);
                exit(1);
            } else resultado++;
        }
    }
    printf("Resultado: %d / %d\n\n", resultado, tam*tam);

    resultado = 0;
    printf("Teste do shift a esquerda:\n\t");
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam_shifts; j++) {
            if (!testa_big_shift_functions(big_shl, shifts_BigInt, ans, numeros[i], valores_shifts[j], '<')) {
                printf("Erro ao shiftar para esquerda o valor:\n\t");
                print_big(numeros[i]);
                printf("%d vezes\n", valores_shifts[j]);
                printf("Valor retornado:\n\t");
                print_big(ans);
                exit(1);
            } else resultado++;
        }
    }
    printf("Resultado: %d / %d\n\n", resultado, tam*tam_shifts);

    resultado = 0;
    printf("Teste do shif a direita (logico):\n\t");
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam_shifts; j++) {
            if (!testa_big_shift_functions(big_shr, shifts_BigInt, ans, numeros[i], valores_shifts[j], '>')) {
                printf("Erro ao shiftar para direita (logico) o valor:\n\t");
                print_big(numeros[i]);
                printf("%d vezes\n", valores_shifts[j]);
                printf("Valor retornado:\n\t");
                print_big(ans);
                exit(1);
            } else resultado++;
        }
    }
    printf("Resultado: %d / %d\n\n", resultado, tam*tam_shifts);

    resultado = 0;
    printf("Teste do shif a direita (aritmetico):\n\t");
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam_shifts; j++) {
            if (!testa_big_shift_functions(big_sar, shifts_BigInt, ans, numeros[i], valores_shifts[j], 'a')) {
                printf("Erro ao shiftar para direita (aritmetico) o valor:\n\t");
                print_big(numeros[i]);
                printf("%d vezes\n", valores_shifts[j]);
                printf("Valor retornado:\n\t");
                print_big(ans);
                exit(1);
            } else resultado++;
        }
    }
    printf("Resultado: %d / %d\n\n", resultado, tam*tam_shifts);

    return 0;
}