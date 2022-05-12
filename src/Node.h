#pragma once
#include "Entity.h"

class Node
{
public:
	void addData(Entity*);
	void setLeftNode(Node* node);
	void setRightNode(Node* node);
	Entity* getEntityPointer();
	size_t getSubSize() const;
	size_t getHeight() const;
	Rectangle getKey() const;
	Node*& getLeftNode();
	Node*& getRightNode();
	friend bool operator<(const Node& lhs, const Node& rhs);
	friend bool operator>(const Node& lhs, const Node& rhs);
public:

	Node* left = nullptr, * right = nullptr;
private:
	CollisionShape* key;
	Entity *entity;
	
	size_t subsize = 1, height = 1;

};




