// Scapegoat-tree.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cmath>
#include <iostream>
#include <vector>

class node {
public:
	int key;
	node *left;
	node *right;

	node()
	{
		this->key = 0;
		this->right = NULL;
		this->left = NULL;
	}

	node(int k)
	{
		this->key = k;
		this->right = NULL;
		this->left = NULL;
	}
};

class Scape_goat_tree
{
private:
	node *root;
	float a;
	float s;
	float maxS;

public:
	Scape_goat_tree()
	{
		this->root = NULL;
		this->a = 0.5;
		this->s = 0;
		this->maxS = 0;
	}

	int size(node *Node)
	{
		if (Node == NULL)
			return 0;
		else {
			int n = 1;
			n += size(Node->left);
			n += size(Node->right);
			return n;
		}
	}

	int ha()
	{
		return (int)(log(this->s) / log(1 / this->a));
	}

	node *findScapeGoat(std::vector<node *> *stack)
	{
		int i = stack->size() - 1;
		node *n = stack->at(i);
		int depth = 0;
		while (i != 0)
		{
			depth++;
			float totalSize = this->size(stack->at(i - 1));

			if (depth > (log(totalSize) / log(1 / this->a)))
				return stack->at(i - 1);
			n = stack->at(i - 1);
			i--;
		}
	}

	void add(int key)
	{
		node *newNode = new node(key);
		int depth = addWithDepth(newNode);
	}

	void rebuild(node *Node, std::vector<node *> *stack)
	{
		int p = 0;
		for (int i = 0; i < stack->size(); i++)
		{
			if (stack->at(i) == Node)
			{
				p = i;
				break;
			}
		}
		int nodeSize = size(Node);
		node **arr = new node*[nodeSize];
		packIntoArray(Node, arr, 0);

		if (p == 0)
		{
			this->root = buildBalanced(arr, 0, nodeSize);
		}
		else if (stack->at(p - 1)->right == Node)
		{
			stack->at(p - 1)->right = buildBalanced(arr, 0, nodeSize);
		}
		else
		{
			stack->at(p - 1)->left = buildBalanced(arr, 0, nodeSize);
		}
		this->maxS = this->s;
	}

	int packIntoArray(node *Node, node *arr[], int i)
	{
		if (Node == NULL)
			return i;

		i = packIntoArray(Node->left, arr, i);
		arr[i++] = Node;
		return packIntoArray(Node->right, arr, i);
	}

	node *buildBalanced(node *arr[], int i, int nodeSize)
	{
		if (nodeSize == 0)
			return NULL;

		int mid = nodeSize / 2;
		arr[i + mid]->left = buildBalanced(arr, i, mid);

		arr[i + mid]->right = buildBalanced(arr, i + mid + 1, nodeSize - mid - 1);

		return arr[i + mid];
	}

	int addWithDepth(node * Node)
	{
		std::vector<node *> stack;
		node *tmp = this->root;
		if (tmp == NULL)
		{
			this->root = Node;
			this->s++;
			return 0;
		}
		bool finish = false;
		int depth = 0;
		do
		{
			stack.push_back(tmp);
			if (Node->key < tmp->key)
			{
				if (tmp->left == NULL)
				{
					tmp->left = Node;
					finish = true;
				}
				else {
					tmp = tmp->left;
				}
			}
			else if (Node->key > tmp->key)
			{
				if (tmp->right == NULL)
				{
					tmp->right = Node;
					finish = true;
				}
				else {
					tmp = tmp->right;
				}
			}
			else
				return -1;
			depth++;
		} while (!finish);
		this->s++;
		if (this->maxS < this->s)
			this->maxS = this->s;
		stack.push_back(Node);

		if (depth > this->ha())
		{
			node *p = this->findScapeGoat(&stack);
			rebuild(p, &stack);
		}

		return depth;
	}

	node *search(node *root, int key)
	{
		node *found = NULL;
		while ((root != NULL) && found == NULL)
		{
			int rkey = root->key;
			if (key < rkey)
				root = root->left;
			else if (key > rkey)
				root = root->right;
			else
			{
				found = root;
				break;
			}
			found = search(root, key);
		}
		return found;
	}

	void deleteCase1(node *Node, std::vector<node *> *stack)
	{
		int i = stack->size() - 1;

		if (i > 0)
		{
			if (stack->at(i - 1)->left == Node)
				stack->at(i - 1)->left = NULL;
			if (stack->at(i - 1)->right == Node)
				stack->at(i - 1)->right = NULL;

			delete Node;
			return;
		}

		if (i == 0)
		{
			this->root = NULL;

			delete Node;
			return;
		}
	}

	void deleteCase2(node *Node, std::vector<node *> *stack)
	{
		int i = stack->size() - 1;

		if (Node->left == NULL)
		{
			if (i > 0)
			{
				if (stack->at(i - 1)->left == Node)
					stack->at(i - 1)->left = Node->right;
				if (stack->at(i - 1)->right == Node)
					stack->at(i - 1)->right = Node->right;

				delete Node;
				return;
			}
			if (i == 0)
			{
				this->root = Node->right;

				delete Node;
				return;
			}
		}
		else {
			if (i > 0)
			{
				if (stack->at(i - 1)->left == Node)
					stack->at(i - 1)->left = Node->left;
				if (stack->at(i - 1)->right == Node)
					stack->at(i - 1)->right = Node->left;

				delete Node;
				return;
			}
			if (i == 0)
			{
				this->root = Node->left;

				delete Node;
				return;
			}
		}
	}

	void deleteCase3(node *Node, std::vector<node *> *stack)
	{
		int i = stack->size() - 1;

		node *min = Node->right;
		stack->push_back(min);
		while (min->left != NULL)
		{
			min = min->left;
			stack->push_back(min);
		}

		int m = stack->size() - 1;

		node *tmp = Node->left;
		Node->left = min->left;
		min->left = tmp;

		tmp = Node->right;
		Node->right = min->right;
		min->right = tmp;

		if (i > 0)
		{
			if (stack->at(i - 1)->left == Node)
				stack->at(i - 1)->left = min;
			if (stack->at(i - 1)->right == Node)
				stack->at(i - 1)->right = min;
		}
		if (m > 0)
		{
			if (stack->at(m - 1)->left == min)
				stack->at(m - 1)->left = Node;
			if (stack->at(m - 1)->right == min)
				stack->at(m - 1)->right = Node;
		}

		if (min->right == min)
		{
			min->right = Node;
		}
		
		if (Node == this->root)
			this->root = min;

		tmp = stack->at(i);
		stack->at(i) = stack->at(m);
		stack->at(m) = tmp;

		if (Node->left == NULL && Node->right == NULL)
		{
			this->deleteCase1(Node, stack);
		}
		else if (Node->left == NULL || Node->right == NULL)
		{
			this->deleteCase2(Node, stack);
		}
	}

	void deleteNode(int key)
	{
		node *tmp = this->root;

		if (tmp == NULL)
			return;

		bool find = false;
		std::vector<node *> stack;
		do
		{
			stack.push_back(tmp);
			if (tmp->key < key)
			{
				tmp = tmp->right;
			}
			else if (tmp->key > key)
			{
				tmp = tmp->left;
			}
			else if (tmp->key == key)
			{
				find = true;
			}
		} while (tmp != NULL && find == false);

		if (tmp->left == NULL && tmp->right == NULL)
		{
			this->deleteCase1(tmp, &stack);
		}
		else if (tmp->left == NULL || tmp->right == NULL)
		{
			this->deleteCase2(tmp, &stack);
		}
		else if (tmp->left != NULL && tmp->right != NULL)
		{
			this->deleteCase3(tmp, &stack);
		}

		this->s--;

		if (this->s < this->a * maxS)
			rebuild(this->root, &stack);
	}

	void print()
	{
		std::cout << std::endl;
		print_Tree(this->root, 0);
		std::cout << std::endl;
	}

	void print_Tree(node *p, int level)
	{
		if (p)
		{
			print_Tree(p->right, level + 1);
			for (int i = 0; i< level; i++) std::cout << "   ";
			std::cout << p->key << std::endl;
			print_Tree(p->left, level + 1);
		}
	}
};


int main()
{
	Scape_goat_tree sgt;
	int max;

	std::cin >> max;

	for (int i = 0; i < max; i++)
	{
		int key;
		std::cin >> key;
		sgt.add(key);
	}

	sgt.print();

	sgt.deleteNode(4);

	sgt.print();

	system("pause");

    return 0;
}

