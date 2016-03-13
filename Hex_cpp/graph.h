#include <iostream>
#include <vector>
#include "node.h"

class graph{
public:
	graph();
	bool nodeHasNeighbors(node);
	int countSimilarNeighbors(node, char);
	bool checkBlocked(node, char);
	bool checkFull();
	void setMove(char);
	void aiSetMove(char, int, int);
	void randomlyPopulate(char);
	bool checkVictory(char);
	bool recursivePathW(std::vector<std::vector<bool> > &, node, node);
	bool recursivePathB(std::vector<std::vector<bool> > &, node, node);
	bool aICheckVictory(char, node, bool&);
	bool aIRecursivePathW(std::vector<std::vector<bool> > &, node, node, node, bool&);
	bool aIRecursivePathB(std::vector<std::vector<bool> > &, node, node, node, bool&);
	friend std::ostream& operator<< (std::ostream &out, const graph & g);
	void resetBoard();
	
	char getEntryAt(int, int);
	node getNodeAt(int, int);
	bool getWaitingForPlayer() { return waitingForPlayer; }
	void setWaitingForPlayer(bool wait) { waitingForPlayer = wait; }
	bool getIsThinking() { return isThinking; }
	void setIsThinking(bool thinking) { isThinking = thinking; }
	char getPlayer() { return player; }
	void setPlayer(char p) { player = p; }
	char getWinner() { return winner; }
	void setWinner(char w) { winner; }
private:
	bool waitingForPlayer;
	bool isThinking;
	char winner;
	char player;
	int size;
	int elements;
	std::vector< std::vector<node> > hexBoard;
};//end graph class

class location{
public:
	location(int, int);
	int row;
	int col;
};//end location class