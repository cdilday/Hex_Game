#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <thread>
#include "graph.h"
#include <SFML/Graphics.hpp>


// probability function returns a random number below 1
inline double probability();


//runs each next possible move in the graph and then randomly populates it, then checks to see who won.
//Repeats 100 times and keeps track of which node won the most, then gives that node's row and column 
//value back to the playAIGame function to make that move
void monteCarloAI(graph realBoard, char player, int& row, int& column, bool& noChance);

void checkTopHalf(graph * realBoard, char player, int& rowTop, int& columnTop, bool& noChance, int& winningFavor);

void checkBottomHalf(graph * realBoard, char player, int& rowBottom, int& columnBottom, bool& noChance, int& winningFavor);

void threadedMonteCarloAI(graph * realBoard, char player, int& row, int& column, bool& noChance);

 //plays the game between 2 Human Players
void playGame(graph board);

//plays the game between 1 player and an AI
void playAIGame(graph* board);

 //Returns true if the given player won, false if the given player lost. Places randomly on board
bool playPureAIGame(graph board, char player);
