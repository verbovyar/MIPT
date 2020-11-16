#include "avl.h"

inline
int getHeight(Node* node) {
	return node ? node->height : 0;
}

inline
void setHeight(Node* node) {
	if(node != NULL)
	node->height = fmax(getHeight(node->left), getHeight(node->right)) + 1;
}

AVL* newAVL()
{
	AVL* tree = (AVL*)calloc(1, sizeof(AVL));
	tree->root = (Node*)calloc(1, sizeof(Node));
	tree->size = 0;

	return tree;
}

void deleteAVL(AVL* tree)
{
	tree->size = 0;
	free(tree->root);
}

void smallLeft(AVL* tree, Node* node)
{
	Node* temp = node->right;
	node->right = temp->left;
	if (temp->left != NULL)
	{
		temp->left->parent = node;
	}

	temp->parent = node->parent;
	if (node->parent == NULL)
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

	setHeight(node);
	setHeight(node->parent);
}

void smallRight(AVL* tree, Node* node)
{
	Node* temp = node->left;
	node->left = temp->right;
	if (temp->right != NULL)
	{
		temp->right->parent = node;
	}

	temp->parent = node->parent;
	if (node->parent == NULL)
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

	setHeight(node);
	setHeight(node->parent);
}

void bigLeft(AVL* tree, Node* node)
{
	smallRight(tree, node->right);
	smallLeft(tree, node);
}

void bigRight(AVL* tree, Node* node)
{
	smallLeft(tree, node->left);
	smallRight(tree, node);
}

int balance(Node* node)
{
	return getHeight(node->right) - getHeight(node->left);
}

void fixUp(AVL* tree, Node* node)
{
	while (node)
	{
		if (balance(node) == 2)
		{
			if (balance(node->right) < 0)
			{
				bigLeft(tree, node);
			}
			else
			{
				smallLeft(tree, node);
			}
		}

		if (balance(node) == -2)
		{
			if (balance(node->left) > 0)
			{
				bigRight(tree, node);
			}
			else
			{
				smallRight(tree, node);
			}
		}

		setHeight(node);
		node = node->parent;
	}
}

void insert(AVL* tree, Map* key) {
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
			if (strcmp(key->str1, new_leaf->key->str1) < 0) //str
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
		else if (strcmp(node->key->str1, new_parent->key->str1) < 0) //str
		{
			new_parent->left = node;
		}
		else
		{
			new_parent->right = node;
		}

		fixUp(tree, node);
	}

	tree->size++;
}

Node* FindSubtree(Node* subtree_root, char* str) 
{
	if(subtree_root == NULL) 
	{
		return NULL;
	}

	if(strcmp(str, subtree_root->key->str1) < 0) 
	{
		return FindSubtree(subtree_root->left, str);
	}

	if(strcmp(subtree_root->key->str1, str) < 0) 
	{
		return FindSubtree(subtree_root->right, str);
	}

	return subtree_root;
}

Node* Find(AVL* tree, char* key) 
{
	return FindSubtree(tree->root, key);
}
