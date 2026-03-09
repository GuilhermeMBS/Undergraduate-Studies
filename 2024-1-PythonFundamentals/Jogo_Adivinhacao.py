"""
Jogo da Adivinhação
Jogo Único (Somente uma Jogada)
Autor: Guilherme Soares
"""


import random
import time


def printMensagemInicial():
    print('\n')
    for elemento in ('*'*60 , 'Jogo da Adivinhação' , 'Pense em um inteiro de 1 a 63' , 
                    'Em seguida, tecle ENTER para continuar... e boa sorte!' , '*'*60):
        print('*'+(elemento).center(60,' ')+'*')


def testeCartela(lista: list) -> int:
    var = input('\nO valor escolhido está nessa cartela <s/n>? ')
    if var == 's':
        return lista[0]
    else:
        return 0


def geraCartelas(potencia_dois: int) -> list:
    cartela = []
    for n in range(potencia_dois,64):
        if n&potencia_dois:
            cartela.append(n)
    return cartela


def printBonito(lista: list , i: int) -> int:
    print(f'\nCartela {i+1}:')
    for x in range(4):
        print(*[f'{e:2d}' for e in lista[x*8:x*8+8]])
 

numero_escolhido = 0
potencias = [1,2,4,8,16,32]
random.shuffle(potencias) #Criar Cartelas em Potências Aleatórias

printMensagemInicial()
comando_jogar = input()

for (ind,pot) in enumerate(potencias):
    cartela = geraCartelas(pot)
    printBonito(cartela,ind)
    numero_escolhido += testeCartela(cartela)

print('\nO valor escolhido foi...' , end = '')
time.sleep(2)
print(f' {numero_escolhido}!\n')