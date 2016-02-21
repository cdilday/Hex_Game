//Code by Christopher Dilday

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <thread>
#include "graph.h"

using namespace std;
inline double probability(){ return 1.0*rand() / RAND_MAX; }// probability function returns a random number below 1

//checks if it is near another node. Will be used in Monte Carlo to make the AI less random. Will also return true if it's along the edges

//runs each next possible move in the graph and then randomly populates it, then checks to see who won.
//Repeats 100 times and keeps track of which node won the most, then gives that node's row and column 
//value back to the playAIGame function to make that move
void monteCarloAI(graph realBoard, char player, int& row, int& column, bool& noChance)
{
	graph board = realBoard;
	graph tempBoard = board;
	bool checkUsed = false;
	bool bypass = false;
	bool noWins = true;
	int favor = 0, tempWins = 0, tempFavor = 0;
	//loops to traverse every piece on the board
	for (int r = 0; r < 11; r++)
	{
		for (int c = 0; c < 11; c++)
		{
			if (!bypass && !board.checkBlocked(board.hexBoard[r][c], player) && board.hexBoard[r][c].getEntry() == '-')
			{
				tempBoard = board;
				//loop that simulates the board moves
				for (int i = 0; i < 1000; i++)
				{
					tempBoard.aiSetMove(player, r, c);
					tempBoard.randomlyPopulate();
					if (tempBoard.aICheckVictory(player, tempBoard.hexBoard[r][c], checkUsed))
					{
						tempWins++;
						if (checkUsed)
							tempFavor++;
						noWins = false;
					}//keeps track of wins, and if it was used in the win add to its favor
					tempBoard = board;
					checkUsed = false;
				}// keeps track of how many times it wins with that move
				tempFavor += tempWins;
				if (tempWins == 1000)
				{
					bypass = true;
					row = r;
					column = c;
				}//this ignores favor and makes the move if it won 100% of the time
				if (!bypass && tempFavor >= favor)
				{
					row = r;
					column = c;
					favor = tempFavor;
				}// replaces the coordinates if the move won more often
				cout << "Wins for " << r << " " << c << ": " << tempFavor << endl;
				tempWins = 0; tempFavor = 0;
				board.hexBoard[r][c].setEntry('-');
			}//end if the chosen 
		}//end column traversing
	}//end row traversing 

	//no Wins means that the AI simulated all moves and couldn't see any possible way to win
	noChance = noWins;
}//end monteCarloAI

//plays the game between 2 Human Players
void playGame(graph board){
	cout << "Starting a game of Hex" << endl;
	cout << board;
	char player = 'W';

	for (int turn = 1; turn <= 121; turn++)
	{
		if (player == 'W' && turn !=1)
			player = 'B';
		else
			player = 'W';
		board.setMove(player);
		cout << board;
		if (board.checkVictory(player))
		{
			cout << player << " Player Wins" << endl;
			break;
		}

	}
}
//plays the game between 1 player and an AI
void playAIGame(graph board){
	cout << "Starting a game of Hex with an AI opponent" << endl;
	cout << "Pick W or B as your color (W goes first): ";
	char player, aIplayer;
	cin >> player;
	if (player == 'W')
		aIplayer = 'B';
	else
		aIplayer = 'W';
	cout << board;
	int r, c;

	for (int turn = 1; turn <= 121; turn++)
	{
		if (player == 'W'){
			board.setMove(player);
			turn++;
			cout << board;
			if (board.checkVictory(player))
			{
				cout << player << " Player Wins" << endl;
				break;
			}
		}

		bool noChance = true;
		thread first(monteCarloAI, board, aIplayer, ref(r), ref(c), ref(noChance));

		first.join();
		
		
		if (noChance)
		{
			cout << aIplayer << " player gives up, as they see no good moves left!"<< endl;
			break;
		}

		if (aIplayer == 'W')
		{
			board.aiSetMove(aIplayer, r, c);
			turn++;
		}
		else
			board.aiSetMove(aIplayer, r, c);
		cout << board;
		if (board.checkVictory(aIplayer))
		{
			cout << aIplayer << " Player Wins" << endl;
			break;
		}
		if (player == 'B')
		{
			board.setMove(player);
			cout << board;
			if (board.checkVictory(player))
			{
				cout << player << " Player Wins" << endl;
				break;
			}
		}//end if player is B move
	}//end main for loop
}//end playAIGame

//Returns true if the given player won, false if the given player lost. Places randomly on board
bool playPureAIGame(graph board, char player){
	int r, c;

	for (int turn = 1; turn <= 121; turn++)
	{
		r = rand() % 11;
		c = rand() % 11;
		while (board.hexBoard[r][c].getEntry() == 'W' || board.hexBoard[r][c].getEntry() == 'B')
		{
			r = rand() % 11;
			c = rand() % 11;
		}
		board.aiSetMove('W', r, c);
		turn++;
		if (board.checkVictory('W'))
		{
			break;
		}
		r = rand() % 11;
		c = rand() % 11;
		while (board.hexBoard[r][c].getEntry() == 'W' || board.hexBoard[r][c].getEntry() == 'B')
		{
			r = rand() % 11;
			c = rand() % 11;
		}
		board.aiSetMove('B', r, c);
		if (board.checkVictory('B'))
		{
			break;
		}

	}
	if (board.checkVictory(player))
		return true;
	return false;
}


/* Ideal printout of board:
  0 1 2 3 4 5 6 7 8 9 10
 0 - - - - - - - - - - -
  1 - - - - - - - - - - -
   2 - - - - - - - - - - -
    3 - - - - - - - - - - -
     4 - - - - - - - - - - -
      5 - - - - - - - - - - -
       6 - - - - - - - - - - -
        7 - - - - - - - - - - -
         8 - - - - - - - - - - -
          9 - - - - - - - - - - -
          10 - - - - - - - - - - -*/

int main(int argc, char** argv)
{
	srand(time(0));
	graph test;
	playAIGame(test);
	system("PAUSE");
	return 0;
}//end main function