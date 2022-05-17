#pragma once
#include "Node.h"

class Tree
{
public:
	void addNode(Entity*&);
	void balanceTree();
	Node* getRoot();
	

private:
	Node* addNodeHelper(Node*& , Entity*& );
	int balanceFactor(Node* );
	bool isBalanced(Node*);
	Node* balanceHelper(Node*);
	Node* rotateLeft(Node* node);
	Node* rotateRight(Node* node);
private:
	Node* root;
};



