// Scapegoat-tree.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cmath>
#include <iostream>

class node {
public:
	int key;
	node *left;
	node *right;
	node *parent;

	node()
	{
		this->key = 0;
		this->right = NULL;
		this->left = NULL;
		this->parent = NULL;
	}

	node(int k)
	{
		this->key = k;
		this->right = NULL;
		this->left = NULL;
		this->parent = NULL;
	}
};

class Scape_goat_tree
{
private:
	node *root;
	float a;
	float s;
	int maxSize;

public:
	Scape_goat_tree()
	{
		this->root = NULL;
		this->a = 0.5;
		this->s = 0;
		this->maxSize = 0;
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

	node *findScapeGoat(node *Node)
	{
		node *n = Node;
		int depth = 0;
		while (n->parent != NULL)
		{
			depth++;
			float totalSize = this->size(n->parent);

			if (depth > (log(totalSize) / log(1 / this->a)))
				return n->parent;
			n = n->parent;
		}
	}

	void add(int key)
	{
		node *newNode = new node(key);
		int depth = addWithDepth(newNode);
		if (depth > this->ha())
		{
			node *p = this->findScapeGoat(newNode);
			rebuild(p);
		}
	}

	void rebuild(node *Node)
	{
		int nodeSize = size(Node);
		node *par = Node->parent;
		node **arr = new node*[nodeSize];
		packIntoArray(Node, arr, 0);

		if (par == NULL)
		{
			this->root = buildBalanced(arr, 0, nodeSize);
			this->root->parent = NULL;
		}
		else if (par->right == Node)
		{
			par->right = buildBalanced(arr, 0, nodeSize);
			par->right->parent = par;
		}
		else
		{
			par->left = buildBalanced(arr, 0, nodeSize);
			par->left->parent = par;
		}
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
		if (arr[i + mid]->left != NULL)
			arr[i + mid]->left->parent = arr[i + mid];

		arr[i + mid]->right = buildBalanced(arr, i + mid + 1, nodeSize - mid - 1);
		if (arr[i + mid]->right != NULL)
			arr[i + mid]->right->parent = arr[i + mid];

		return arr[i + mid];
	}

	int addWithDepth(node * Node)
	{
		node *tmp = this->root;
		if (tmp == NULL)
		{
			this->root = Node;
			this->s++;
			this->maxSize++;
			return 0;
		}
		bool finish = false;
		int depth = 0;
		do
		{
			if (Node->key < tmp->key)
			{
				if (tmp->left == NULL)
				{
					tmp->left = Node;
					Node->parent = tmp;
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
					Node->parent = tmp;
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
		this->maxSize++;

		return depth;
	}

	node *search(node *root, int key)
	{
		if (root == NULL || root->key == key)
		{
			return root;
		}
		else if (key <= root->left->key) {
			return search(root->left, key);
		}
		else {
			return search(root->right, key);
		}
	}

	void print()
	{
		print_Tree(this->root, 0);
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

	system("pause");

    return 0;
}

