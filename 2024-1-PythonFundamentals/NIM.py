from random import randint


def exibeApresentacaoJogo() -> str:
    linhas = [' ','Jogo NIM','Vence quem retirar o último bastão.','Boa sorte!!!',' ']
    print('',*[f'*** {el.center(50)} ***' for el in linhas],'', sep = '\n')
    print('-'*13,'<Pressione ENTER para começar>','-'*13, end = '')


def geraListaBastoes() -> list:
    lista_bastoes = [chr(9608)*randint(1,7) for i in range(randint(3,6))]
    return lista_bastoes


def exibeBastoes(lista: list) -> str:
    for (ind,val) in enumerate(lista):
        print('\n'+f'Grupo {ind+1}:     ', *[i for i in val])


def umaJogada(lista: list , grupo: int , quantidade: int) -> bool:
    lista[grupo-1] = chr(9608)*(len(lista[grupo-1])-quantidade)
    return sum([i == '' for i in lista]) == len(lista)


def calculaSomaNim(columns: list) -> int:
    return eval('^'.join([str(len(x)) for x in columns]))
    

def estrategiaComputador(lista_bastoes: list) -> list:
    for k in range(len(lista_bastoes)-1):
        var = calculaSomaNim(lista_bastoes.pop(k))
        if len(lista_bastoes[k]) - var >= 0:
            lista_bastoes[k] -= var*chr(9608)
            break


exibeApresentacaoJogo()
done = input()
tabuleiro = geraListaBastoes()
jogador = True

while not done:
    jogador = not jogador
    exibeBastoes(tabuleiro)
    print('\n'+f'TURNO DO JOGADOR {jogador+1}')
    grupo = int(input('-> De qual grupo deseja retirar os bastões? '))
    quantidade = int(input('-> Quantos bastões deseja retirar? '))
    done = umaJogada(tabuleiro,grupo,quantidade)

print('\n'+'*'*37,f'\n* PARABÉNS, JOGADOR {jogador+1}, VOCÊ GANHOU! *\n'+'*'*37,'\n')



         
#Colocar número de barras do lado
#Tirar possibilidades erradas