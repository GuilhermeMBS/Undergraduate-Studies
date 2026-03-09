"""
Tradicional Jogo "Pedra, Papel e Tesoura"
"""


#Random Module
import random

#Base Info
options_list = ["rock" , "paper" , "scissors"]
player_points = 0
computer_points = 0
username = input("Insert Username: ")

#Play Function
def play_function(user_input):

    #Global Variables:
    global computer_points
    global player_points

    #Computer Choice
    computer_choice = random.randint(0,2)

    #User didn't type rock, paper or scissors 
    if user_input not in options_list:
        print("Sorry, try again.")
        return
    
    #Match
    #Player Wins
    if computer_choice == options_list.index(user_input) - 1:
        print("Computer played" , options_list[computer_choice].upper() , "\n" "YOU WON")
        player_points += 1
    
    #Tie
    elif computer_choice == options_list.index(user_input):
        print("Computer played" , options_list[computer_choice].upper() , "\n" "IT'S A TIE!")
    
    #Computer Wins
    else:
        print("Computer played" , options_list[computer_choice].upper() , "\n" "YOU LOST")
        computer_points += 1

#Game Setup
while True:

    #Main Menu
    user_choice = input("Play / Quit / Score: ").lower()

    #Play Game
    if user_choice == "play":
        play_function(user_input = input("Insert rock, paper or scissors: "))

    #Score Board
    elif user_choice == "score":
        print(username + ":", player_points, "\n" "Computer:", computer_points)

    #End Game
    elif user_choice == "quit":
        print("Thanks for playing! Here are the Final Results:" "\n",username + ":", player_points, "\n" " Computer:", computer_points)
        break

    #User didn't type play, score or quit
    else:
        print("Type any of the options, please.")