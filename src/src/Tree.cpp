#include "Tree.h"

void Tree::addNode(Entity*& entity)
{
	if (root == nullptr)
	{
		Node* newNode = new Node;
		newNode->addData(entity);
		root = newNode;
	}
	else
	{
		root = addNodeHelper(root, entity);
	}

}


Node* Tree::addNodeHelper(Node*& node, Entity*& entity)
{
	Node tmpNode;
	tmpNode.addData(entity);
	if (tmpNode < *node)
	{
		if (node->left == nullptr)
		{
			Node* newNode = new Node;
			newNode->addData(entity);
			node->setLeftNode(newNode);
		}
		else
		{
			node->setLeftNode(addNodeHelper(node->left, entity));
		}

	}
	if (!isBalanced(node))
	{
		node = balanceHelper(node);
	}

	if (tmpNode > *node)
	{
		if (node->right == nullptr)
		{
			Node* newNode = new Node;
			newNode->addData(entity);
			node->setRightNode(newNode);
		}
		else
		{
			node->setRightNode(addNodeHelper(node->right, entity));
		}

	}
	if (!isBalanced(node))
	{
		node = balanceHelper(node);
	}

	return node;
}


Node* Tree::rotateLeft(Node* node)
{
	if (!isBalanced(node->left))
	{
		node->setLeftNode(balanceHelper(node->left));
	}
	if (!isBalanced(node->right))
	{
		node->setRightNode(balanceHelper(node->right));
	}
	int tmpi = balanceFactor(node->right);
	if (balanceFactor(node->right) != 1 && balanceFactor(node->right) != 0)
	{
		node->setRightNode(rotateRight(node->right));
	}
	Node* tmp = node->right;
	node->setRightNode(tmp->left);
	tmp->setLeftNode(node);
	return tmp;
}

Node* Tree::rotateRight(Node* node)
{
	if (!isBalanced(node->left))
	{
		node->setLeftNode(balanceHelper(node->left));
	}
	if (!isBalanced(node->right))
	{
		node->setRightNode(balanceHelper(node->right));
	}
	if (balanceFactor(node->left) != -1 && balanceFactor(node->left) != 0)
	{
		node->setLeftNode(rotateLeft(node->left));
	}
	Node* tmp = node->left;
	node->setLeftNode(tmp->right);
	tmp->setRightNode(node);
	return tmp;
}

void Tree::balanceTree()
{
	while (!isBalanced(root))
	{
		if (!isBalanced(root->left))
		{
			root->setLeftNode(balanceHelper(root->left));
		}
		if (!isBalanced(root->right))
		{
			root->setRightNode(balanceHelper(root->right));
		}
		if (balanceFactor(root) > 1)
		{
			root = rotateLeft(root);
		}
		else if (balanceFactor(root) < -1)
		{
			root = rotateRight(root);
		}
	}
}

Node* Tree::getRoot()
{
	return root;
}

Node* Tree::balanceHelper(Node* node)
{
	if (!isBalanced(node))
	{
		if (!isBalanced(node->left))
		{
			node->setLeftNode(balanceHelper(node->left));
		}
		if (!isBalanced(node->right))
		{
			node->setRightNode(balanceHelper(node->right));
		}
		if (balanceFactor(node) > 1)
		{
			node = rotateLeft(node);
		}
		else if (balanceFactor(node) < -1)
		{
			node = rotateRight(node);
		}
	}

	return node;
}

bool Tree::isBalanced(Node* node)
{
	if (node == nullptr)
	{
		return true;
	}
	if (balanceFactor(node) == 1 || balanceFactor(node) == 0 || balanceFactor(node) == -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Tree::balanceFactor(Node* node)
{
	if (node->left != nullptr && node->right != nullptr)
	{
		return node->right->getHeight() - node->left->getHeight();
	}
	else if (node->left == nullptr && node->right != nullptr)
	{
		return node->right->getHeight();
	}
	else if (node->left != nullptr && node->right == nullptr)
	{
		return -1 * node->left->getHeight();
	}
	else
	{
		return 0;
	}
}