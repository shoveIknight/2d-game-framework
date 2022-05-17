#include "Node.h"
void Node::addData(Entity* data)
{
	entity = data;
	key = data->getHitboxRect();
}


void Node::setLeftNode(Node* nodePtr)
{
	left = nodePtr;
	if (left == nullptr && nodePtr == nullptr)
	{
		height = 1;
		subsize = 1;
	}
	else if (right != nullptr && nodePtr == nullptr)
	{
		subsize = right->getSubSize() + 1;
		height = right->getHeight() + 1;
	}
	else if (right == nullptr && nodePtr != nullptr)
	{
		subsize = nodePtr->getSubSize() + 1;
		height = nodePtr->getHeight() + 1;
	}
	else
	{
		if (nodePtr->getHeight() > right->getHeight())
		{
			height = nodePtr->getHeight() + 1;
		}
		else
		{
			height = right->getHeight() + 1;
		}
		subsize = right->getSubSize() + left->getSubSize() + 1;
	}

}

void Node::setRightNode(Node* nodePtr)
{
	right = nodePtr;
	if (left == nullptr && nodePtr == nullptr)
	{
		height = 1;
		subsize = 1;
	}
	else if (left != nullptr && nodePtr == nullptr)
	{
		subsize = left->getSubSize() + 1;
		height = left->getHeight() + 1;
	}
	else if (left == nullptr && nodePtr != nullptr)
	{
		subsize = nodePtr->getSubSize() + 1;
		height = nodePtr->getHeight() + 1;
	}
	else if (left != nullptr && nodePtr != nullptr)
	{
		if (nodePtr->getHeight() > left->getHeight())
		{
			height = nodePtr->getHeight() + 1;
		}
		else
		{
			height = left->getHeight() + 1;
		}
		subsize = right->getSubSize() + left->getSubSize() + 1;
	}
}

Entity* Node::getEntityPointer()
{
	return entity;
}

size_t Node::getSubSize() const
{
	return subsize;
}
size_t Node::getHeight() const
{
	return height;
}

Rectangle Node::getKey() const
{
	return key;
}

Node*& Node::getLeftNode()
{
	return left;
}

Node*& Node::getRightNode()
{
	return right;
}

bool operator<(const Node& lhs, const Node& rhs)
{
	if (lhs.getKey().x < rhs.getKey().x)
	{
		return true;
	}
	return false;
}

bool operator>(const Node& lhs, const Node& rhs)
{
	Rectangle key1 = lhs.getKey();
	if (key1.x + key1.w >= rhs.getKey().x)
	{
		return true;
	}
	return false;
}