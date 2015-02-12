//C++ code for Assignment 6's AI by Christopher Dilday
//see Readme for details on the UI

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
inline double probability(){ return 1.0*rand() / RAND_MAX; }// probability function returns a random number below 1



class node{
public:
	node(int, int, char );
	node* getConnection(int pos){ return ptrList[pos]; };
	void setConnection(int pos, node *ptr) {ptrList[pos] = ptr; }
	bool isConnected(node);
	int getRow(){ return row; }
	void setRow(int r){ row = r;}
	int getColumn(){ return col; }
	void setColumn(int c){ col = c; }
	char getEntry(){ return entry; }
	void setEntry(char e){ entry = e; }

private:
	int row, col;
	char entry;
	vector< node * > ptrList;
	//ptrList layout, where the n is the current node and the numbers represent visually the adjacent nodes are
	//	 0 1
	//	5 n 2
	//   4 3
};//end node class
//Constructor defaults node information to 0 and - unless it recieves the information from the graph class
node::node(int r = 0, int c = 0, char e = '-')
{
	ptrList.resize(6);
	row = r; 
	col = c;
	entry = e;
	if (row == 0)
		ptrList[0] = NULL, ptrList[1] = NULL;
	if (col == 0)
		ptrList[4] = NULL, ptrList[5] = NULL;
	if (row == 10)
		ptrList[3] = NULL, ptrList[4] = NULL;
	if (col == 10)
		ptrList[1] = NULL, ptrList[2] = NULL;
}//end node constructor
//returns true if there is a connection, false if there isn't
bool node::isConnected(node check){
	for (int i = 0; i < 6; i++)
	{
		node temp = *ptrList[i];
		if (temp.getRow() == check.getRow() && temp.getColumn() == check.getColumn())
			return true;
	}
	return false;
}//end isConnected

//checks if it is near another node. Will be used in Monte Carlo to make the AI less random. Will also return true if it's along the edges

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
	bool recursivePathW(vector<vector<bool> > &, node, node);
	bool recursivePathB(vector<vector<bool> > &, node, node);
	bool aICheckVictory(char, node, bool&);
	bool aIRecursivePathW(vector<vector<bool> > &, node, node, node, bool&);
	bool aIRecursivePathB(vector<vector<bool> > &, node, node, node, bool&);
	friend ostream& operator<< (ostream &out, const graph & g);
	vector< vector<node> > hexBoard;
private:
	int size;
	int elements;
};//end graph class
//graph constructor creates a graph with 11 * 11 nodes that connect to their adjacent nodes
graph::graph(){
	elements = 0;
	hexBoard.resize(11);
	for (int i = 0; i < 11; i++)
		hexBoard[i].resize(11);
	for (int r = 0; r < 11; r++)
	{
		for (int c = 0; c < 11; c++)
		{
			hexBoard[r][c] = node(r, c);
		}
	}//end for loop that creates the nodes within the board
}//end graph constructor, previously make_graph
//checks if the given node has values in its surrounding nodes or is on an edge
bool graph::nodeHasNeighbors(node temp)
{
	int r = temp.getRow();
	int c = temp.getColumn();
	if (r == 0 || r == 10 || c == 0 || c == 10)
		return true;
	else
	{
		if (hexBoard[r - 1][c].getEntry() != '-')
			return true;
		else if (hexBoard[r - 1][c + 1].getEntry() != '-')
			return true;
		else if (hexBoard[r][c + 1].getEntry() != '-')
			return true;
		else if (hexBoard[r + 1][c].getEntry() != '-')
			return true;
		else if (hexBoard[r + 1][c - 1].getEntry() != '-')
			return true;
		else if (hexBoard[r][c - 1].getEntry() != '-')
			return true;
	}
	return false;
}
//Returns how many of its neighbors are similar and favors the player's win direction and blocking
int graph::countSimilarNeighbors(node temp, char player)
{
	int r = temp.getRow();
	int c = temp.getColumn();
	int favor = 0;
	if (r != 0 && hexBoard[r - 1][c].getEntry() == 'B')
	{
		if (player == 'W')
			favor += 3;
		favor++;
	}
	if (r != 10 && hexBoard[r + 1][c].getEntry() == 'B')
	{
		if (player == 'W')
			favor += 3;
		favor++;
	}
	if (c != 0 && hexBoard[r][c - 1].getEntry() == 'W')
	{
		if (player == 'B')
			favor += 3;
		favor++;
	}
	if (c != 10 && hexBoard[r][c + 1].getEntry() == 'W')
	{
		if (player == 'W')
			favor += 3;
		favor++;
	}
	if (r != 0 && c != 10 && player == 'B')
	{
		if (hexBoard[r - 1][c + 1].getEntry() == 'B')
			favor++;
		else if (hexBoard[r - 1][c + 1].getEntry() == 'W')
			favor += 4;
	}
	else if (r != 0 && c != 10 && player == 'W')
	{
		if (hexBoard[r - 1][c + 1].getEntry() == 'W')
			favor++;
		else if (hexBoard[r - 1][c + 1].getEntry() == 'B')
			favor += 4;
	}
	if (r != 10 && c != 0 && player == 'B')
	{
		if (hexBoard[r + 1][c - 1].getEntry() == 'B')
			favor++;
		else if (hexBoard[r + 1][c - 1].getEntry() == 'W')
			favor += 4;
	}
	else if (r != 10 && c != 0 && player == 'W')
	{
		if (hexBoard[r + 1][c - 1].getEntry() == 'W')
			favor++;
		else if (hexBoard[r + 1][c - 1].getEntry() == 'B')
			favor += 4;
	}
	return favor;
}
//checks if the node has any empty entries around it. Returns false if it does.
bool graph::checkBlocked(node temp, char player)
{
	int r = temp.getRow();
	int c = temp.getColumn();
	int numFull = 0;
	if (r != 0 && (hexBoard[r - 1][c].getEntry() == '-' || hexBoard[r - 1][c].getEntry() == player))
		return false;
	if (r != 0 && c != 10 && (hexBoard[r - 1][c + 1].getEntry() == '-' || hexBoard[r - 1][c + 1].getEntry() == player))
		return false;
	if (c != 10 && (hexBoard[r][c + 1].getEntry() == '-' || hexBoard[r][c + 1].getEntry() == player))
		return false;
	if (r != 10 && (hexBoard[r + 1][c].getEntry() == '-' || hexBoard[r + 1][c].getEntry() == player))
		return false;
	if (r != 10 && c != 0 && (hexBoard[r + 1][c - 1].getEntry() == '-' || hexBoard[r + 1][c - 1].getEntry() == player))
		return false;
	if (c != 0 && (hexBoard[r][c - 1].getEntry() == '-' || hexBoard[r][c - 1].getEntry() == player))
		return false;

	return true;
}
//checks if there is anymore space left in the graph, returns true if the graph is full
bool graph::checkFull()
{
	for (int r = 0; r < 11; r++)
	{
		for (int c = 0; c < 11; c++)
		{
			if (hexBoard[r][c].getEntry() == '-')
				return false;
		}
	}
	return true;
}
//Player uses this to set move, prompts the player for their input
void graph::setMove(char player){
	int r, c;
	bool placed = false;
	bool oobFlag; //oobFlag (out of bounds flag) checks to make sure the move is on the board.=
	while (!placed){
		oobFlag = false;
		cout << "Player " << player << ", enter row: ";
		cin >> r;
		cout << "Enter column: ";
		cin >> c;
		if (r > 10 || r < 0 || c > 10 || c < 0)
			oobFlag = true;
		if (!oobFlag && hexBoard[r][c].getEntry() == '-')
		{
			hexBoard[r][c].setEntry(player);
			placed = true;
			elements++;
		}
	}//end check to see if player move is not valid
	if (r != 0 && hexBoard[r - 1][c].getEntry() == player)
	{
		hexBoard[r][c].setConnection(0, &hexBoard[r - 1][c]);
		hexBoard[r - 1][c].setConnection(3, &hexBoard[r][c]);
	}
	if (r != 0 && c != 10 && hexBoard[r - 1][c + 1].getEntry() == player)
	{
		hexBoard[r][c].setConnection(1, &hexBoard[r - 1][c + 1]);
		hexBoard[r - 1][c + 1].setConnection(4, &hexBoard[r][c]);
	}
	if (c != 10 && hexBoard[r][c + 1].getEntry() == player)
	{
		hexBoard[r][c].setConnection(2, &hexBoard[r][c + 1]);
		hexBoard[r][c + 1].setConnection(5, &hexBoard[r][c]);
	}
	if (r != 10 && hexBoard[r + 1][c].getEntry() == player)
	{
		hexBoard[r][c].setConnection(3, &hexBoard[r + 1][c]);
		hexBoard[r + 1][c].setConnection(0, &hexBoard[r][c]);
	}
	if (r != 10 && c != 0 && hexBoard[r + 1][c - 1].getEntry() == player)
	{
		hexBoard[r][c].setConnection(4, &hexBoard[r + 1][c - 1]);
		hexBoard[r + 1][c - 1].setConnection(1, &hexBoard[r][c]);
	}
	if (c != 0 && hexBoard[r][c - 1].getEntry() == player)
	{
		hexBoard[r][c].setConnection(5, &hexBoard[r][c - 1]);
		hexBoard[r][c - 1].setConnection(2, &hexBoard[r][c]);
	}
}//end playerSetMove
//AI uses this to place its move, has no input prompts.
void graph::aiSetMove(char player, int r, int c){
	hexBoard[r][c].setEntry(player);
	elements++;
	if (r != 0 && hexBoard[r - 1][c].getEntry() == player)
	{
		hexBoard[r][c].setConnection(0, &hexBoard[r - 1][c]);
		hexBoard[r - 1][c].setConnection(3, &hexBoard[r][c]);
	}
	if (r != 0 && c != 10 && hexBoard[r - 1][c + 1].getEntry() == player)
	{
		hexBoard[r][c].setConnection(1, &hexBoard[r - 1][c + 1]);
		hexBoard[r - 1][c + 1].setConnection(4, &hexBoard[r][c]);
	}
	if (c != 10 && hexBoard[r][c + 1].getEntry() == player)
	{
		hexBoard[r][c].setConnection(2, &hexBoard[r][c + 1]);
		hexBoard[r][c + 1].setConnection(5, &hexBoard[r][c]);
	}
	if (r != 10 && hexBoard[r + 1][c].getEntry() == player)
	{
		hexBoard[r][c].setConnection(3, &hexBoard[r + 1][c]);
		hexBoard[r + 1][c].setConnection(0, &hexBoard[r][c]);
	}
	if (r != 10 && c != 0 && hexBoard[r + 1][c - 1].getEntry() == player)
	{
		hexBoard[r][c].setConnection(4, &hexBoard[r + 1][c - 1]);
		hexBoard[r + 1][c - 1].setConnection(1, &hexBoard[r][c]);
	}
	if (c != 0 && hexBoard[r][c - 1].getEntry() == player)
	{
		hexBoard[r][c].setConnection(5, &hexBoard[r][c - 1]);
		hexBoard[r][c - 1].setConnection(2, &hexBoard[r][c]);
	}
}//end aISetMove
//randomly populate the graph evenly with W & B
void graph::randomlyPopulate()
{
	int r, c;
	for (int i = 0; i < 121 && !checkFull(); i++)
	{
		r = rand() % 11;
		c = rand() % 11;
		while (hexBoard[r][c].getEntry() == 'W' || hexBoard[r][c].getEntry() == 'B')
		{
			r = rand() % 11;
			c = rand() % 11;
		}
		elements++;
		if (elements >= 90 && i % 5 == 0)
		{
			if (checkVictory('W') || checkVictory('B'))
				break;
		}//breaks the loop if someone's won for more accuracy, only runs it once it's relatively full every 5 moves for optimization
		if (i % 2 == 0)
		{
			hexBoard[r][c].setEntry('W');
			if (r != 0 && hexBoard[r-1][c].getEntry() == 'W')
			{
				hexBoard[r][c].setConnection(0, &hexBoard[r - 1][c]);
				hexBoard[r - 1][c].setConnection(3, &hexBoard[r][c]);
			}
			if (r != 0 && c != 10 && hexBoard[r - 1][c + 1].getEntry() == 'W')
			{
				hexBoard[r][c].setConnection(1, &hexBoard[r - 1][c + 1]);
				hexBoard[r - 1][c + 1].setConnection(4, &hexBoard[r][c]);
			}			
			if (c != 10 && hexBoard[r][c + 1].getEntry() == 'W')
			{
				hexBoard[r][c].setConnection(2, &hexBoard[r][c + 1]);
				hexBoard[r][c + 1].setConnection(5, &hexBoard[r][c]);
			}
			if (r != 10 && hexBoard[r + 1][c].getEntry() == 'W')
			{
				hexBoard[r][c].setConnection(3, &hexBoard[r + 1][c]);
				hexBoard[r + 1][c].setConnection(0, &hexBoard[r][c]);
			}
			if (r != 10 && c != 0 && hexBoard[r + 1][c - 1].getEntry() == 'W')
			{
				hexBoard[r][c].setConnection(4, &hexBoard[r + 1][c - 1]);
				hexBoard[r + 1][c - 1].setConnection(1, &hexBoard[r][c]);
			}
			if (c != 0 && hexBoard[r][c - 1].getEntry() == 'W')
			{
				hexBoard[r][c].setConnection(5, &hexBoard[r][c - 1]);
				hexBoard[r][c - 1].setConnection(2, &hexBoard[r][c]);
			}
		}//if it's an even number, populate and connect the new white node
		else
		{
			hexBoard[r][c].setEntry('B');
			if (r != 0 && c != 0 && hexBoard[r - 1][c].getEntry() == 'B')
			{
				hexBoard[r][c].setConnection(0, &hexBoard[r - 1][c]);
				hexBoard[r - 1][c].setConnection(3, &hexBoard[r][c]);
			}
			if (r != 0 && c != 10 && hexBoard[r - 1][c + 1].getEntry() == 'B')
			{
				hexBoard[r][c].setConnection(1, &hexBoard[r - 1][c + 1]);
				hexBoard[r - 1][c + 1].setConnection(4, &hexBoard[r][c]);
			}
			if (c != 10 && hexBoard[r][c + 1].getEntry() == 'B')
			{
				hexBoard[r][c].setConnection(2, &hexBoard[r][c + 1]);
				hexBoard[r][c + 1].setConnection(5, &hexBoard[r][c]);
			}
			if (r != 10 && hexBoard[r + 1][c].getEntry() == 'B')
			{
				hexBoard[r][c].setConnection(3, &hexBoard[r + 1][c]);
				hexBoard[r + 1][c].setConnection(0, &hexBoard[r][c]);
			}
			if (r != 10 && c != 0 && hexBoard[r + 1][c - 1].getEntry() == 'B')
			{
				hexBoard[r][c].setConnection(4, &hexBoard[r + 1][c - 1]);
				hexBoard[r + 1][c - 1].setConnection(1, &hexBoard[r][c]);
			}
			if (c != 0 && hexBoard[r][c - 1].getEntry() == 'B')
			{
				hexBoard[r][c].setConnection(5, &hexBoard[r][c - 1]);
				hexBoard[r][c - 1].setConnection(2, &hexBoard[r][c]);
			}
		}//if it's an odd number, populate and connect the new black node
	}//end main for loop
}//end randomlyPopulate
//Checks to see if the given char has won, returns true if it has and false if not
bool graph::checkVictory(char player)
{
	// Player W, white, wins west to east
	// Player B, black, wins north to south
	vector< vector<bool> > processed(11, vector <bool>(11, false));
	node start, goal;
	if (player == 'W')
	{
		for (int r = 0; r < 11; r++)
		{
			start = hexBoard[r][0];
			if (start.getEntry() == 'W' && processed[r][0] != true)
			{
				for (int rGoal = 0; rGoal < 11; rGoal++)
				{
					goal = hexBoard[rGoal][10];
					//recusively traverse 'W' path to if it won
					if (goal.getEntry() == 'W')
					{
						if (recursivePathW(processed, start, goal))
						{
							return true;
						}
					}//end if for goal == W
				}//end for for goal nodes
			}//end if for start == W
			processed[r][0] = true;
		}//end for for start nodes
	}//end if plaer W victory check

	if (player == 'B')
	{
		for (int c = 0; c < 11; c++)
		{
			start = hexBoard[0][c];
			if (start.getEntry() == 'B' && processed[0][c] != true)
			{
				for (int cGoal = 0; cGoal < 11; cGoal++)
				{
					goal = hexBoard[10][cGoal];
					//recusively traverse 'B' path to if if it won
					if (goal.getEntry() == 'B' && (recursivePathB(processed, start, goal)))
					{
						return true;
					}//end if for goal == B
				}//end for for goal nodes
			}//end if for start == B
			processed[0][c] = true;
		}//end for for start nodes
	}//end player B victory check
	return false;
}//end checkVictory
//recursively traverses graph to see if W player has made an E/W path
bool graph::recursivePathW(vector<vector<bool> > &processed, node start, node goal)
{
	int r = start.getRow();
	int c = start.getColumn();
	processed[r][c] = true;
	for (int i = 0; i < 6; i++)
	{
		if (c == 10 ||(r == goal.getRow() && c == goal.getColumn())) //Base case, hit it's goal
			return true;
		if (start.getConnection(i) != NULL && processed[(*start.getConnection(i)).getRow()][(*start.getConnection(i)).getColumn()] == false)//how it exits the recursion
		{
			if (recursivePathW(processed, *start.getConnection(i), goal))//exit  case when it hits the base case
				return true;
		}
	}
	return false; //secondary base case when it hits a dead-end node
}
//recursively traverses graph to see if B player has made a N/S path
bool graph::recursivePathB(vector<vector<bool> > &processed, node start, node goal)
{
	int r = start.getRow();
	int c = start.getColumn();
	processed[r][c] = true;
	for (int i = 0; i < 6; i++)
	{
		if (r == 10 || (r == goal.getRow() && c == goal.getColumn())) //Base case, hit it's goal
			return true;
		if (start.getConnection(i) != NULL && processed[(*start.getConnection(i)).getRow()][(*start.getConnection(i)).getColumn()] == false)//how it exits the recursion
		{
			if (recursivePathB(processed, *start.getConnection(i), goal))//exit case when it hits the base case
				return true;
		}
	}
	return false; //secondary base case when it hits a dead-end node
}
//Same as checkVictory but takes a check node the checks if it was used in the winning path
bool graph::aICheckVictory(char player, node check, bool &checkUsed)
{
	// Player W, white, wins west to east
	// Player B, black, wins north to south
	vector< vector<bool> > processed(11, vector <bool>(11, false));
	node start, goal;
	if (player == 'W')
	{
		for (int r = 0; r < 11; r++)
		{
			start = hexBoard[r][0];
			if (start.getEntry() == 'W' && processed[r][0] != true)
			{
				for (int rGoal = 0; rGoal < 11; rGoal++)
				{
					goal = hexBoard[rGoal][10];
					//recusively traverse 'W' path to if it won
					if (goal.getEntry() == 'W')
					{
						if (aIRecursivePathW(processed, start, goal, check, checkUsed))
						{
							return true;
						}
					}//end if for goal == W
				}//end for for goal nodes
			}//end if for start == W
			processed[r][0] = true;
		}//end for for start nodes
	}//end if plaer W victory check

	if (player == 'B')
	{
		for (int c = 0; c < 11; c++)
		{
			start = hexBoard[0][c];
			if (start.getEntry() == 'B' && processed[0][c] != true)
			{
				for (int cGoal = 0; cGoal < 11; cGoal++)
				{
					goal = hexBoard[10][cGoal];
					//recusively traverse 'B' path to if if it won
					if (goal.getEntry() == 'B' && (aIRecursivePathB(processed, start, goal, check, checkUsed)))
					{
						return true;
					}//end if for goal == B
				}//end for for goal nodes
			}//end if for start == B
			processed[0][c] = true;
		}//end for for start nodes
	}//end player B victory check
	return false;
}//end checkVictory
//recursively traverses graph to see if W player has made an E/W path. Checks if the check node is used in the winning path
bool graph::aIRecursivePathW(vector<vector<bool> > &processed, node start, node goal, node check, bool &checkUsed)
{
	int r = start.getRow();
	int c = start.getColumn();
	processed[r][c] = true;
	for (int i = 0; i < 6; i++)
	{
		if (c == 10 || (r == goal.getRow() && c == goal.getColumn())) //Base case, hit it's goal
		{
			if (!checkUsed && r == check.getRow() && c == check.getColumn())
				checkUsed = true;
			return true;
		}
		if (start.getConnection(i) != NULL && processed[(*start.getConnection(i)).getRow()][(*start.getConnection(i)).getColumn()] == false)//how it exits the recursion
		{
			if (aIRecursivePathW(processed, *start.getConnection(i), goal, check, checkUsed))//exit  case when it hits the base case
			{
				if (!checkUsed && r == check.getRow() && c == check.getColumn())
					checkUsed = true;
				return true;
			}
		}
	}
	return false; //secondary base case when it hits a dead-end node
}
//recursively traverses graph to see if B player has made a N/S path. Checks if the check node is used in the winning path
bool graph::aIRecursivePathB(vector<vector<bool> > &processed, node start, node goal, node check, bool &checkUsed)
{
	int r = start.getRow();
	int c = start.getColumn();
	processed[r][c] = true;
	for (int i = 0; i < 6; i++)
	{
		if (r == 10 || (r == goal.getRow() && c == goal.getColumn())) //Base case, hit it's goal
		{
			if (!checkUsed && r == check.getRow() && c == check.getColumn())
				checkUsed = true;
			return true;
		}
		if (start.getConnection(i) != NULL && processed[(*start.getConnection(i)).getRow()][(*start.getConnection(i)).getColumn()] == false)//how it exits the recursion
		{
			if (aIRecursivePathB(processed, *start.getConnection(i), goal, check, checkUsed))//exit case when it hits the base case
			{
				if ( !checkUsed && r == check.getRow() && c == check.getColumn())
					checkUsed = true;
				return true;
			}
		}
	}
	return false; //secondary base case when it hits a dead-end node
}
//Overloaded <<, prints out the graph
ostream& operator<< (ostream &out, const graph &g)
{
	vector< vector<node> > temp = g.hexBoard;
	cout << " ";
	for (int i = 0; i < 11; i++)
		cout << i << " ";
	cout << endl;
	for (int r = 0; r < 11; r++)
	{
		for (int s = 0; s < r; s++){
			if (s != 9)
				cout << " ";
		}
		cout << r << " ";
		for (int c = 0; c < 11; c++)
		{
			cout << temp[r][c].getEntry() << " ";
		}
		cout << endl;
	}
	cout << endl;
	return out;
}//end overloaded << function

//runs each next possible move in the graph and then randomly populates it, then checks to see who won.
//Repeats 100 times and keeps track of which node won the most, then gives that node's row and column 
//value back to the playAIGame function to make that move
void monteCarloAI(graph realBoard, char player, int& row, int& column)
{
	graph board = realBoard;
	graph tempBoard = board;
	bool checkUsed = false;
	bool bypass = false;
	int favor = 0, tempWins = 0, tempFavor = 0;
	for (int r = 0; r < 11; r++)
	{
		for (int c = 0; c < 11; c++)
		{
			if (!bypass && !board.checkBlocked(board.hexBoard[r][c], player) && board.hexBoard[r][c].getEntry() == '-')
			{
				tempBoard = board;
				for (int i = 0; i < 1000; i++)
				{
					tempBoard.aiSetMove(player, r, c);
					tempBoard.randomlyPopulate();
					if (tempBoard.aICheckVictory(player, tempBoard.hexBoard[r][c], checkUsed))
					{
						tempWins++;
						if (checkUsed)
							tempFavor++;
					}//keeps track of wins, and if it was used in the win add to its favor
					tempBoard = board;
					checkUsed = false;
				}// keeps track of how many times it wins with that move
				tempFavor += tempWins;
				tempFavor += (3 * tempBoard.countSimilarNeighbors(tempBoard.hexBoard[r][c], player));
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
				cout << "Favor for " << r << " " << c << ": " << tempFavor << endl;
				tempWins = 0; tempFavor = 0;
				board.hexBoard[r][c].setEntry('-');
			}//end if the chosen 
		}//end column traversing
	}//end row traversing 
}//end monteCarloAI
//plays the game between 2 players
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
		monteCarloAI(board, aIplayer, r, c);
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
	//system("PAUSE"); //<- this is for Visual Studio, ignore it
	return 0;
}//end main function