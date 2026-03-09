"""
Guilherme Soares
Project - Hangman Game
"""

#Word's Choice
secret_word = input("\nInsert the word to be guessed: ").upper()

#Word Listed
words_list = list(secret_word)

#User Attempts
guessed_list = []
for i in words_list:
    if i == " ":
        guessed_list.append(i)
    else:
        guessed_list.append("_")

#Guessing
attempts = 0
letters_guessed = []

#Game
while attempts < 3:

    #Letter Guess
    letter = input("\nInsert your letter guess: ").upper()

    #More Than One Letter
    if len(letter) != 1:

        #Correct Word Guess
        if letter == secret_word:
            guessed_list = words_list
            pass

        #Wrong Word Guess
        elif len(letter) == len(secret_word):
            print("\nYou guessed the word wrong!")
            attempts = 3

        #Missclick   
        else:
            print("\nWrite one letter only!")
            pass
    
    #Check The Letter
    else:
        
        #Repeated letter
        if letter in guessed_list or letter in letters_guessed:
            print("\nYou already guessed this letter! Try again!")
            pass

        #Letter not in the word
        elif letter not in words_list:
            letters_guessed.append(letter)
            attempts += 1

        #Letter in the word
        else:
            for i in range(len(words_list)):
                if words_list[i] == letter:
                    guessed_list[i] = letter
        a = "".join(guessed_list)
        b = "/".join(letters_guessed)
        print(f"\nWord: {a} / Missed Attempts: {attempts} - {b}\n")

    #Check if the word is guessed
    if words_list == guessed_list:
        print("\nCongratulations! You guessed the word! \n")
        break

#Loser Message
if attempts == 3:
    c = "".join(words_list)
    print(f"\nYou Lost! The word is: {c}\n")