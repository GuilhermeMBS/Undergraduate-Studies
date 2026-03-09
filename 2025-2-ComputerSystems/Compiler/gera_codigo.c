/*
    Guilherme Soares 3WC
    João Pedro 3WC
*/

#include <stdio.h>
#include <stdlib.h>
#include "gera_codigo.h"

// Definindo os valores dos registradores
#define EAX 0
#define EDI 7
#define RBP 5


/*
    Array para armazenar os enderecos de cada funcao
*/
unsigned char* end_funcs[25];

/*
    Comandos padrao para toda funcao
*/
static void insere_comeco_func(unsigned char code[], int* pos) {
    // Os 4 primeiros são para o push e mov, os demais são para o sub
    unsigned char instrucoes[] = {0x55, 0x48, 0x89, 0xe5, 0x48, 0x83, 0xec, 0x20};
    int tam = sizeof(instrucoes)/sizeof(instrucoes[0]);

    for(int i = 0; i < tam; i++) code[(*pos)++] = instrucoes[i];
}

static void insere_fim_func(unsigned char code[], int* pos) {
    unsigned char instrucoes[] = {0xc9, 0xc3};
    int tam = sizeof(instrucoes)/sizeof(instrucoes[0]);

    for(int i = 0; i < tam; i++) code[(*pos)++] = instrucoes[i];
}


/*
    Funcoes para mover valores entre registradores e/ou memoria
*/
static void move_constante_reg(unsigned char code[], int* pos, int valor, unsigned char reg) {
    /*
        Usando o registrador 0xb8 + reg temporariamente
        0xB8 -> %eax
    */
    code[(*pos)++] = 0xb8 + reg;
    for (int i = 0; i < 4; i++) {
        code[(*pos)++] = valor & 0xFF;
        valor = valor >> 8;
    }
}

static void move_reg_pilha(unsigned char code[], int* pos, unsigned char reg, unsigned char num_var) {
    code[(*pos)++] = 0x89; // Reg -> Pilha
    code[(*pos)++] = (0b01 << 6) | (reg << 3) | RBP;
    num_var++; num_var *= -4;
    code[(*pos)++] = num_var;
}

static void move_pilha_reg(unsigned char code[], int* pos, unsigned char reg, unsigned char num_var) {
    code[(*pos)++] = 0x8b; // Mem (ou pilha) -> reg
    code[(*pos)++] = (0b01 << 6) | (reg << 3) | RBP;
    num_var++; num_var *= -4;
    code[(*pos)++] = num_var;
}

static void move_reg_reg(unsigned char code[], int* pos, unsigned char reg_fonte, unsigned char reg_dest) {
    code[(*pos)++] = 0x89;
    code[(*pos)++] = (0b11 << 6) | (reg_fonte << 3) | reg_dest;
}

/*
    Funcoes para adcionar chamada de outras funcoes
    "call (ind) (c1)(num1)"
        c1 -> 'p', 'v', '$';
*/
static void adiciona_call(unsigned char code[], int* pos, int ind, char c1, int num1) {
    // Salvar o parametro (pushq %rdi)
    code[(*pos)++] = 0x57;

    // Passar o novo parametro
    if (c1 == '$') move_constante_reg(code, pos, num1, EDI);
    else if (c1 == 'v') move_pilha_reg(code, pos, EDI, num1);

    int offset, pos_offset;

    // call func(ind)
    code[(*pos)++] = 0xe8;
    pos_offset = *pos;
    *pos = *pos + 4;
    offset = (int) (end_funcs[ind] - &code[(*pos)]);
    for (int i = 0; i < 4; i++) {
        code[pos_offset++] = offset & 0xFF;
        offset = offset >> 8;
    }

    // Recuperar o parametro da funcao atual (popq %rdi)
    code[(*pos)++] = 0x5F;
}

/*
    Funcao para adicionar o comando de soma, subtracao ou multiplicacao
    dos valores
*/
static void insere_operacao(unsigned char code[], int* pos, char op, char c2, int valor) {
    unsigned char is_one_byte = (valor == (valor & 0xFF)) ? 1 : 0;

    switch (op) {
        case '*':
            if (c2 == '$') code[(*pos)++] = (is_one_byte) ? 0x6b : 0x69;
            else {
                code[(*pos)++] = 0x0F;
                code[(*pos)++] = 0xAF;
            }
            code[(*pos)] = ((c2 == 'v') ? (0b01 << 6) : (0b11 << 6)) | (EAX << 3);
            switch (c2) {
                case '$':
                    code[(*pos)++] |= EAX;
                    for (int i = 0; i < ((is_one_byte) ? 1 : 4); i++) {
                        code[(*pos)++] = valor & 0xFF;
                        valor = valor >> 8;
                    }

                    break;
                case 'v':
                    code[(*pos)++] |= RBP;
                    valor = (valor + 1) * (-4);
                    code[(*pos)++] = valor & 0xFF;

                    break;
                default:
                    code[(*pos)++] |= EDI;
                    break;
            }
            
            break;
        // Soma ou subtracao
        default:
            switch (c2) {
                case '$':
                    code[(*pos)++] = (is_one_byte) ? 0x83 : 0x81;
                    code[(*pos)++] = (0b11 << 6) | ((op == '+') ? (0 << 3) : (0b101 << 3)) | EAX;
                    for (int i = 0; i < ((is_one_byte) ? 1 : 4); i++) {
                        code[(*pos)++] = valor & 0xFF;
                        valor = valor >> 8;
                    }

                    break;
                case 'p':
                    code[(*pos)++] = (op == '+') ? 0x01 : 0x29;
                    code[(*pos)++] = (0b11 << 6) | (EDI << 3) | EAX;

                    break;
                default:
                    code[(*pos)++] = (op == '+') ? 0x03 : 0x2b;
                    code[(*pos)++] = (0b01 << 6) | (EAX << 3) | RBP;
                    
                    valor = ((valor + 1) * (-4)) & 0xFF;// Ofset do %rbp
                    code[(*pos)++] = valor; 

                    break;
            }

            break;
    }
}

/*
    "c1(ans) = (t1)"
        c1 -> 'p', 'v';
        t1 -> 'p', 'v', 'c';
*/
static void adiciona_expressao(FILE* f, unsigned char code[], int* pos, char c1) {
    int ans, num1, num2;
    char t1, t2, op;
    fscanf(f, "%d = %c", &ans, &t1);

    if (t1 == 'c') {
        /* 
            Chamada de funcao: "(c1)(ans) = call (num1) (t2)(num2)"
                t2 -> 'p', 'v', '$';
        */
        fscanf(f, "all %d %c%d\n", &num1, &t2, &num2);
        adiciona_call(code, pos, num1, t2, num2);

        if (c1 == 'p') move_reg_reg(code, pos, EAX, EDI);
        else move_reg_pilha(code, pos, EAX, ans);
    } else {
        /*
            Operacao aritmetica: "(c1)(ans) = (t1)(num1) (op) (t2)(num2)"
                t1 -> 'p', 'v', '$';
                t2 -> 'p', 'v', '$';
                op -> '+', '-', '*';
        */
        fscanf(f, "%d %c %c%d\n", &num1, &op, &t2, &num2);
        
        if (t1 == '$') move_constante_reg(code, pos, num1, EAX);
        else if (t1 == 'p') move_reg_reg(code, pos, EDI, EAX);
        else move_pilha_reg(code, pos, EAX, num1);

        insere_operacao(code, pos, op, t2, num2);

        if (c1 == 'v') move_reg_pilha(code, pos, EAX, ans);
        else move_reg_reg(code, pos, EAX, EDI);
    }
}

/*
    Funcoes para adicionar os valores de retorno
    "ret (c1)(num1)"
        c1 -> 'p', 'v', '$';
*/
static void adiciona_ret(unsigned char code[], int* pos, char c1, int num1) {
    switch (c1) {
        // Retorna o valor da constante
        case '$': 
            move_constante_reg(code, pos, num1, EAX);
            break;
        // Retorna o valor de alguma variavel
        case 'v': 
            move_pilha_reg(code, pos, EAX, (unsigned char) num1);
            break;
        // Retorna o parametro da funcao
        default:
            move_reg_reg(code, pos, EDI, EAX);
            break;
    }
    insere_fim_func(code, pos);
}

/*
    "zret (c1)(num1) (c2)(num2)"
        c1 -> 'p', 'v', '$';
        c2 -> 'p', 'v', '$';
*/
static void adiciona_ret_cond(unsigned char code[], int* pos, char c1, int num1, char c2, int num2) {
    // zret c1num1 c2num2
    int pos_offset;

    if (c1 == '$') move_constante_reg(code, pos, num1, EAX);
    
    // cmpl $0, (reg ou pilha)
    code[(*pos)++] = 0x83;
    code[(*pos)] = (0b11 << 6) | (0b111 << 3);

    if (c1 == '$')  code[(*pos)++] |= EAX;
    else if (c1 == 'p')  code[(*pos)++] |= EDI;
    else  {
        code[(*pos)] = (code[(*pos)] | RBP) & 0x7F;
        (*pos)++;
        num1 = (num1 + 1) * (-4);
        code[(*pos)++] = num1 & 0xFF;
    }

    code[(*pos)++] = 0;

    // jne (proxima instrucao)
    code[(*pos)++] = 0x75;
    pos_offset = (*pos)++;

    // Retorno (se o valor avaliado for zero)        
    adiciona_ret(code, pos, c2, num2);

    // Calcula os offsets do jump
    if (c2 == '$') code[pos_offset] += 0x07;
    else if (c2 == 'p') code[pos_offset] += 0x04;
    else code[pos_offset] += 0x05;
    // insere_fim_func(code, pos);
}

/*
    A funcao que gera o codigo de maquina
*/
void gera_codigo (FILE *f, unsigned char code[], funcp* entry) {
    char letra;
    int qtd_func = 0;
    int pos = 0;

    for (int i = 0; i < 25; i++) end_funcs[i] = 0;

    // Vars que vao ser usadas
    char c1, c2;
    int num1, num2;
    while ((letra = fgetc(f)) != EOF) {
        switch (letra) {
            // Eh o inicio de uma funcao
            case 'f': 
                fscanf(f, "unction\n");
                end_funcs[qtd_func] = &code[pos];
                *entry = (funcp)end_funcs[qtd_func++];
                
                insere_comeco_func(code, &pos);

                break;
            // Terminou a funcao
            case 'e':
                fscanf(f, "nd\n");
                break;
            // Valor de retorno da funcao
            case 'r':
                fscanf(f, "et %c%d\n", &c1, &num1);

                adiciona_ret(code, &pos, c1, num1);

                break;
            case 'z':
                // Retorno condicional
                fscanf(f, "ret %c%d %c%d\n", &c1, &num1, &c2, &num2);

                adiciona_ret_cond(code, &pos, c1, num1, c2, num2);
                
                break;
            case 'v':
                // Expressao algebrica
                adiciona_expressao(f, code, &pos, 'v');
                break;
            case 'c':
                // Chamou uma funcao
                int ind;
                fscanf(f, "all %d %c%d\n", &ind, &c1, &num1);
                adiciona_call(code, &pos, ind, c1, num1);
                break;
            default:
                // Comecou com 'p'
                adiciona_expressao(f, code, &pos, 'p');
                break;
        }
    }
}