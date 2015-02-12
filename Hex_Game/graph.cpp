#include <iostream>
#include <vector>
#include "graph.h"

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
		std::cout << "Player " << player << ", enter row: ";
		std::cin >> r;
		std::cout << "Enter column: ";
		std::cin >> c;
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
			if (r != 0 && hexBoard[r - 1][c].getEntry() == 'W')
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
	std::vector<std::vector<bool> > processed(11, std::vector <bool>(11, false));
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
bool graph::recursivePathW(std::vector<std::vector<bool> > &processed, node start, node goal)
{
	int r = start.getRow();
	int c = start.getColumn();
	processed[r][c] = true;
	for (int i = 0; i < 6; i++)
	{
		if (c == 10 || (r == goal.getRow() && c == goal.getColumn())) //Base case, hit it's goal
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
bool graph::recursivePathB(std::vector<std::vector<bool> > &processed, node start, node goal)
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
	std::vector< std::vector<bool> > processed(11, std::vector <bool>(11, false));
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
bool graph::aIRecursivePathW(std::vector<std::vector<bool> > &processed, node start, node goal, node check, bool &checkUsed)
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
bool graph::aIRecursivePathB(std::vector<std::vector<bool> > &processed, node start, node goal, node check, bool &checkUsed)
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
				if (!checkUsed && r == check.getRow() && c == check.getColumn())
					checkUsed = true;
				return true;
			}
		}
	}
	return false; //secondary base case when it hits a dead-end node
}
//Overloaded <<, prints out the graph
std::ostream& operator<< (std::ostream &out, const graph &g)
{
	std::vector< std::vector<node> > temp = g.hexBoard;
	std::cout << " ";
	for (int i = 0; i < 11; i++)
		std::cout << i << " ";
	std::cout << std::endl;
	for (int r = 0; r < 11; r++)
	{
		for (int s = 0; s < r; s++){
			if (s != 9)
				std::cout << " ";
		}
		std::cout << r << " ";
		for (int c = 0; c < 11; c++)
		{
			std::cout << temp[r][c].getEntry() << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return out;
}//end overloaded << function