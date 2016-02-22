#include <iostream>
#include <vector>
#include "node.h"

//Constructor defaults node information to 0 and - unless it recieves the information from the graph class
node::node(int r, int c, char e)
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

//getter and setter methods
node* node::getConnection(int pos){ return ptrList[pos]; };
void node::setConnection(int pos, node* ptr) { ptrList[pos] = ptr; }
int node::getRow(){ return row; }
void node::setRow(int r){ row = r; }
int node::getColumn(){ return col; }
void node::setColumn(int c){ col = c; }
char node::getEntry(){ return entry; }
void node::setEntry(char e){ entry = e; }

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
