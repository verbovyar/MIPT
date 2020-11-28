#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

const int RIGHT_ROTATE = -2;
const int LEFT_ROTATE = 2;

struct Node {
	int key = 0;
	Node* parent = nullptr;
	Node* left = nullptr;
	Node* right = nullptr;
	unsigned int height = 0;
};

struct AVLTree
{
	Node* root = nullptr;
	size_t size = 0;
};

inline
int GetHeight(Node* node);
inline
void SetHeight(Node* node);

AVLTree* Construct(AVLTree* temp);
void DeleteAVLTree(AVLTree* tree);

void SmallLeft(AVLTree* tree, Node* node);
void SmallRight(AVLTree* tree, Node* node);
void BigLeft(AVLTree* tree, Node* node);
void BigRight(AVLTree* tree, Node* node);
bool Find(AVLTree* tree, int key);
Node* FindSubtree(Node* subtree_root, int key);

int Balance(Node* node);
void FixUp(AVLTree* tree, Node* node);

void Insert(AVLTree* tree, int key);

inline
int GetHeight(Node* node) {
	return node ? node->height : 0;
}

inline
void SetHeight(Node* node) {
	if (node != nullptr)
	{
		node->height = fmax(GetHeight(node->left), GetHeight(node->right)) + 1;
	}
}

AVLTree* Construct(AVLTree* temp)
{
	AVLTree* tree = (AVLTree*)calloc(1, sizeof(AVLTree));
	tree->root = (Node*)calloc(1, sizeof(Node));
	tree->size = 0;

	return tree;
}

void DeleteAVLTree(AVLTree* tree)
{
	tree->size = 0;
	free(tree->root);
}

void SmallLeft(AVLTree* tree, Node* node)
{
	if (node != nullptr)
	{
		Node* temp = node->right;
		node->right = temp->left;
		if (temp->left != nullptr)
		{
			temp->left->parent = node;
		}

		temp->parent = node->parent;
		if (node->parent == nullptr)
		{
			tree->root = temp;
		}
		else if (node == node->parent->left)
		{
			node->parent->left = temp;
		}
		else
		{
			node->parent->right = temp;
		}

		temp->left = node;
		node->parent = temp;

		SetHeight(node);
		SetHeight(node->parent);
	}
}

void SmallRight(AVLTree* tree, Node* node)
{
	if (node != nullptr)
	{
		Node* temp = node->left;
		node->left = temp->right;
		if (temp->right != nullptr)
		{
			temp->right->parent = node;
		}

		temp->parent = node->parent;
		if (node->parent == nullptr)
		{
			tree->root = temp;
		}
		else if (node == node->parent->right)
		{
			node->parent->right = temp;
		}
		else
		{
			node->parent->left = temp;
		}

		temp->right = node;
		node->parent = temp;

		SetHeight(node);
		SetHeight(node->parent);
	}
}

void BigLeft(AVLTree* tree, Node* node)
{
	if (node != nullptr)
	{
		SmallRight(tree, node->right);
		SmallLeft(tree, node);
	}
}

void BigRight(AVLTree* tree, Node* node)
{
	if (node != nullptr)
	{
		SmallLeft(tree, node->left);
		SmallRight(tree, node);
	}
}

int Balance(Node* node)
{
	return GetHeight(node->right) - GetHeight(node->left);
}

void FixUp(AVLTree* tree, Node* node)
{
	while (node != nullptr)
	{
		if (Balance(node) == LEFT_ROTATE)
		{
			if (Balance(node->right) < 0)
			{
				BigLeft(tree, node);
			}
			else
			{
				SmallLeft(tree, node);
			}
		}

		if (Balance(node) == RIGHT_ROTATE)
		{
			if (Balance(node->left) > 0)
			{
				BigRight(tree, node);
			}
			else
			{
				SmallRight(tree, node);
			}
		}

		SetHeight(node);
		node = node->parent;
	}
}

void Insert(AVLTree* tree, int key) {
	Node* new_parent = nullptr;
	Node* new_leaf = tree->root;
	Node* node = (Node*)calloc(1, sizeof(Node));
	node->parent = new_parent;
	node->key = key;

	if (tree->size == 0)
	{
		tree->root = node;
	}
	else
	{
		while (new_leaf)
		{
			new_parent = new_leaf;

			if (key < new_leaf->key)
			{
				new_leaf = new_leaf->left;
			}
			else
			{
				new_leaf = new_leaf->right;
			}
		}

		node->parent = new_parent;
		if (!new_parent)
		{
			tree->root = node;
		}
		else if (node->key < new_parent->key)
		{
			new_parent->left = node;
		}
		else
		{
			new_parent->right = node;
		}

		FixUp(tree, node);
	}

	tree->size++;
}

Node* FindSubtree(Node* subtree_root, int key)
{
	if (subtree_root == nullptr)
	{
		return nullptr;
	}

	if (key < subtree_root->key)
	{
		return FindSubtree(subtree_root->left, key);
	}

	if (subtree_root->key < key)
	{
		return FindSubtree(subtree_root->right, key);
	}

	return subtree_root;
}

bool Find(AVLTree* tree, int key)
{
	if (FindSubtree(tree->root, key) != nullptr)
	{
		return true;
	}

	return false;
}

void GetChild(AVLTree* tree, Node* node, Node* child)
{
	if (node->parent == nullptr)
	{
		tree->root = child;
	}
	else if (node->parent->left == node)
	{
		node->parent->left = child;
	}
	else
	{
		node->parent->right = child;
	}

	if (child != nullptr)
	{
		child->parent = node->parent;
	}
}

void Erase(AVLTree* tree, Node* node)
{
	if (node == nullptr)
	{
		return;
	}

	Node* new_node = nullptr;

	if (node->left == nullptr)
	{
		new_node = node->right;
		GetChild(tree, node, node->right);
		FixUp(tree, new_node);

		return;
	}

	if (node->right == nullptr)
	{
		new_node = node->left;
		GetChild(tree, node, node->left);
		FixUp(tree, new_node);

		return;
	}

	new_node = node->right;
	while (new_node->left != nullptr)
	{
		new_node = new_node->left;
	}

	if (new_node->parent != node)
	{
		GetChild(tree, new_node, new_node->right);
		new_node->right = node->right;
		new_node->right->parent = new_node;
	}
	else {
		new_node->right->parent = new_node;
	}

	GetChild(tree, node, new_node);
	new_node->left = node->left;
	new_node->left->parent = new_node;
	
	--tree->size;

	FixUp(tree, new_node);
}

void Erase(AVLTree* tree, int key)
{
	Erase(tree, FindSubtree(tree->root, key));
}

void graphvizBST(AVLTree* bst);

int main() 
{
	AVLTree* tree = Construct(nullptr);

	for (int i = 0; i < 10; ++i)
	{
		Insert(tree, i);
	}

	Erase(tree, 3);
	Erase(tree, 7);
	Erase(tree, 4);

	if (Find(tree, 2))
	{
		printf("YES\n");
	}
	else
	{
		printf("NO\n");
	}

	if (Find(tree, 7))
	{
		printf("YES\n");
	}
	else
	{
		printf("NO\n");
	}

	graphvizBST(tree);

	DeleteAVLTree(tree);

	return 0;
}

void LoadNodes(FILE* file, Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->parent != nullptr)
    {
        fprintf(file, "\"%p\" [label=\"{{par: %p|ptr: %p}|{str: %d}|{H: %d}|{l:%p|r:%p}}\"]\n ", node, node->parent, node,
            ((int*)(node->key)), node->height, node->left, node->right); // <-- label printing
    }
    else
    {
        //printf("parent is nullptr\n");
        fprintf(file, "\"%p\" [label=\"{{par: %s}|{str: %d}|{H: %d}}\"]\n ", node, "root",
            ((int*)(node->key)), GetHeight(node)); // <-- label printing
    }
    
    if (node->left != nullptr)
    {
        fprintf(file, "edge[color=orange]\n\"%p\":sw->\"%p\";\n", node, node->left);  // link parent and left child
        LoadNodes(file, node->left);
    }
    if (node->right != nullptr)
    {
        fprintf(file, "edge[color=red]\n\"%p\":se->\"%p\";\n", node , node->right); //link parent and right child
        LoadNodes(file, node->right);
    }
}


void graphvizBST(AVLTree* bst)
{
    if (bst->root == nullptr)
    {
        return;
    }
    FILE* file = fopen("graphPlot.txt", "w");

    fprintf(file, "digraph G{\n");
    fprintf(file, "node [shape=\"record\", style=\"solid\", color=\"green\"];\n");

    LoadNodes(file, bst->root);

    fprintf(file, "}");

    fclose(file);

    system("dot -Tjpg  graphPlot.txt > dump.jpg");
}