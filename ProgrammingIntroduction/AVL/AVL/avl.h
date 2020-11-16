#pragma once

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "map.h"

struct Node {
	Map* key = NULL;
	Node* parent = NULL;
	Node* left = NULL;
	Node* right = NULL;
	unsigned int height = 0;
};

inline
int getHeight(Node* node);
inline
void setHeight(Node* node);

struct AVL
{
	Node* root = NULL;
	size_t size = 0;
};

AVL* newAVL();
void deleteAVL(AVL* tree);

void smallLeft(AVL* tree, Node* node);
void smallRight(AVL* tree, Node* node);
void bigLeft(AVL* tree, Node* node);
void bigRight(AVL* tree, Node* node);
Node* Find(AVL* tree, char* key);
Node* FindSubtree(Node* subtree_root, char* key);

int balance(Node* node);
void fixUp(AVL* tree, Node* node);

void insert(AVL* tree, Map* key);
