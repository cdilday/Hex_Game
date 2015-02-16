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
	void randomlyPopulate();
	bool checkVictory(char);
	bool recursivePathW(std::vector<std::vector<bool> > &, node, node);
	bool recursivePathB(std::vector<std::vector<bool> > &, node, node);
	bool aICheckVictory(char, node, bool&);
	bool aIRecursivePathW(std::vector<std::vector<bool> > &, node, node, node, bool&);
	bool aIRecursivePathB(std::vector<std::vector<bool> > &, node, node, node, bool&);
	friend std::ostream& operator<< (std::ostream &out, const graph & g);
	std::vector< std::vector<node> > hexBoard;
private:
	int size;
	int elements;
};//end graph class

class location{
public:
	location(int, int);
	int row;
	int col;
};//end location class