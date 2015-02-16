#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>

class node{
public:
	node(int r = 0, int c = 0, char e = '-');
	node* getConnection(int);
	void setConnection(int, node*);
	bool isConnected(node);
	int getRow();
	void setRow(int);
	int getColumn();
	void setColumn(int);
	char getEntry();
	void setEntry(char);

private:
	int row, col;
	char entry;
	std::vector< node * > ptrList;
	//ptrList layout, where the n is the current node and the numbers represent visually the adjacent nodes are
	//	 0 1
	//	5 n 2
	//   4 3
};//end node class