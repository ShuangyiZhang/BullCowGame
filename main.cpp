/* This is the console executable, that makes use of the BullCow class
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
*/
#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

struct playAgain {
	bool again;
	bool sameWord;
};

int main();

// function prototypes as outside a class
void PrintHelp();
void PrintIntro1();
void PrintIntro2();
int32 GetValidLength();
void PlayGame();
FText GetValidGuess();
playAgain AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; // instantiate a new game, which we re-use across plays

// the entry point for our application
int main()
{
	PrintHelp();
	playAgain bPlayAgain;
	bPlayAgain.again = false;
	bPlayAgain.sameWord = false;
	do  {
		PrintIntro1();
		BCGame.Reset();
		if (!bPlayAgain.sameWord) BCGame.ResetHiddenWord(GetValidLength());
		PrintIntro2();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	} while (bPlayAgain.again);
	

	return 0; // exit the application
}

void PrintHelp() {
	std::cout << "================ How to play Bulls and Cows ================\n";
	std::cout << "Your task is to guess a hidden ISOGRAM of given length! An isogram is a word without repeating letters.\n";
	std::cout << "You have limited number of tries. For each guess, you will know the number of BULLs and COWs.\n";
	std::cout << "# of bulls = # of correct letters in the correct places.\n";
	std::cout << "# of cows = # of correct letters in the wrong place.\n\n";
	std::cout << "For example, if the the hidden isogram is planet and your guess is pliant,\n";
	std::cout << "Bulls = 3, because p, l and t are in the correct places.\n";
	std::cout << "Cows = 2, because a and n are correct letters, but in the wrong places.\n";
	std::cout << "================ How to play Bulls and Cows ================\n\n\n";

}
void PrintIntro1() 
{
	std::cout << "Welcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "          }   {         ___ " << std::endl;
	std::cout << "          (o o)        (o o) " << std::endl;
	std::cout << "   /-------\\ /          \\ /-------\\ " << std::endl;
	std::cout << "  / | BULL |O            O| COW  | \\ " << std::endl;
	std::cout << " *  |-,--- |              |------|  * " << std::endl;
	std::cout << "    ^      ^              ^      ^ " << std::endl;
	
	return;
}

void PrintIntro2() {
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;
	return;
}

// plays a single game to completion
void PlayGame()
{
	
	int32 MaxTries = BCGame.GetMaxTries();
	
	// loop asking for guesses while the game
	// is NOT won and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) {
		FText Guess = GetValidGuess();
				
		// submit valid guess to the game, and receive counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
	}

	PrintGameSummary();
	return;
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries();
		std::cout << ". Enter your guess: ";
		std::getline(std::cin,Guess);
		
		// check status and give feedback
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status) {
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word witout repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors
	return Guess;
}

playAgain AskToPlayAgain()
{
	playAgain playagain;
	std::cout << "Do you want to play again(y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);
	if ((Response[0] == 'y') || (Response[0] == 'Y')) {
		playagain.again = true;
		//if game is lost, ask user whether to change hidden word
		if (!BCGame.IsGameWon()) {
			Response = "";
			std::cout << "Do you want play again with the same hidden word(y/n)? ";
			std::getline(std::cin, Response);
			if ((Response[0] == 'y') || (Response[0] == 'Y')) {
				playagain.sameWord = true;
			}
			else {
				playagain.sameWord = false;
			}
		}//already won the game; change hidden word
		else {
			playagain.sameWord = false;
		}
	} else {
		playagain.again = false;
	}
	return playagain;
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WIN!\n";
	}
	else
	{
		//std::cout << "The hidden word is: " << BCGame.GetHiddenWord()<<"\n";
		std::cout << "Better luck next time!\n";
	}
}

int32 GetValidLength() {
	std::cout << "What length of word do you want to choose?\n";
	std::cout<<"Please choose an integer from 3 to 7.\n";
	std::cout << "Length of Word = ";
	int32 input=3;
	std::cin>>input ;
	std::cout << "\n";
	std::getchar();
	while (input < 3 || input > 7 ) {
		std::cout << "Please choose an integer from 3 to 7.\n";
		std::cout << "Length of Word = ";
		std::cin >> input;
		std::cout << "\n";
	}

	return input;
}