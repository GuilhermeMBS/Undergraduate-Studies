"""
Calculadora de IMC (20/03/2024)

Guilherme Soares - PUC Rio - 33A

Modelagem e Programação - Exercício para Casa 01

"""


#Função Cálculo de IMC
def imc_calculator():
    
    #Variáveis
    global weight
    global height
    global imc

    #Cálculo de Altura
    while height<=0 or height > 2.5:
        height = float(input("\nInsira sua altura em metros (entre 0 e 2.5): "))

    #Cálculo de Peso
    while weight<=0 or weight > 300:
        weight = float(input("\nInsira seu peso em kg (entre 0 e 300): "))

    #Cálculo do IMC
    imc = weight/(height**2)
    
    return imc

#Função Cadastro Usuário
def profile_register():

    #Variáveis
    global register 

    #Cadastro
    profiles[user_name] = None
    register = "1"
    print("\n Seu cadastro foi realizado com sucesso!")

#Função Registro de Valores
def progress():

    profiles[user_name] = f"IMC: {imc:.2f}"


#Informações Base
profiles = {} #Poderia ser Biblioteca com SQLite para economizar memória
register = "" #Caso de convidado: register = 1 ; caso de cadastrado: register = 2
user_name = input("\n\n\n\n\n\n\nBem Vindo ao IMC APP. Por favor, insira seu nome: ").upper()

#Identificação de Usuário
if user_name in profiles:#Usuário Cadastrado

    register = "1"
    print(f"Bem vindo, {user_name}!")

else: #Novo Usuário

    print("\nVejo que você é um novo usuário. Deseja cadastrar-se? \n1) Sim 2) Não\n")
    register = input("Escolha uma opção: ")

    if register == "1":
        profile_register()

#Interface      
while True:

    print("\n1) CALCULADORA \n2) PROGRESSO \n3) CONTA \n4) SAIR\n")
    user_option = input("Escolha uma opção: ")

    #Calculadora
    if user_option == "1":

        #Calculadora + Resultado
        height = weight = imc = 0
        imc_calculator()
        print(f"\n{user_name}, para o peso {weight:.2f}kg e a altura {height:.2f}m, seu IMC é igual a {imc:.2f}.\n")

        #Salvar Resultado
        save = input("Deseja salvar o progresso? \n1) Sim 2) Não \n\n Escolha uma opção: ")

        #Salvamento para Convidado
        if save == "1" and register == "2":
            register = input("Desculpe. Para salvar o resultado, deve realizar um cadastro. Gostaria de realizá-lo agora? \n\n 1) Sim 2) Não: \n\n Escolha uma opção: ")
            if register == "1":
                profile_register()
                progress()

        #Salvamento para Cadastrado
        elif save == "1" and register == "1":
            progress()
    
    #Progresso
    elif user_option == "2":

        #Para Convidados
        if register == "2":
            print("\nPara acessar o progresso, deve fazer um cadastro. Deseja realizá-lo agora? \n1) Sim \n2) Não\n")

            #Cadastro do Convidado
            register = input("Escolha uma opção: ")
            if register == "1":
                profile_register()

        #Para Cadastrados
        elif register == "1":
            print(profiles)

    #Conta
    elif user_option == "3":
        print(f"\n {user_name}")

    #Sair    
    elif user_option == "4":
        print(f"Obrigado pelo uso, {user_name}! Volte sempre!\n")
        break