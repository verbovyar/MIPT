#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int max_const = 1e9 + 1;
const long long int del = 1000000000;
const int start_result = -1;
const int first_height = 2;
const int second_height = -2;
int max = max_const;

struct Node {
  int key = NULL;
  Node* parent = NULL;
  Node* left = NULL;
  Node* right = NULL;
  unsigned int height = 0;
};

struct Avl {
  Node* root = NULL;
  size_t size = 0;
};

inline int GetHeight(Node* node);
inline void SetHeight(Node* node);
Avl* NewAvl();
void DeleteAvl(Avl* tree);
void SmallLeft(Avl* tree, Node* node);
void SmallRight(Avl* tree, Node* node);
void BigLeft(Avl* tree, Node* node);
void BigRight(Avl* tree, Node* node);
Node* Find(Avl* tree, int key);
Node* FindSubtree(Node* subtree_root, int key);
int Balance(Node* node);
void FixUp(Avl* tree, Node* node);
void Insert(Avl* tree, int key);

inline int GetHeight(Node* node) { return node != NULL ? node->height : 0; }

inline void SetHeight(Node* node) {
  if (node != NULL) {
    node->height = fmax(GetHeight(node->left), GetHeight(node->right)) + 1;
  }
}

Avl* NewAvl() {
  Avl* tree = (Avl*)calloc(1, sizeof(Avl));
  tree->root = (Node*)calloc(1, sizeof(Node));
  tree->size = 0;
  return tree;
}

void DeleteAvl(Avl* tree) {
  tree->size = 0;
  free(tree->root);
}

void SmallLeft(Avl* tree, Node* node) {
  if (node != NULL) {
    Node* temp = node->right;
    node->right = temp->left;
    if (temp->left != NULL) {
      temp->left->parent = node;
    }

    temp->parent = node->parent;
    if (node->parent == NULL) {
      tree->root = temp;
    } else if (node == node->parent->left) {
      node->parent->left = temp;
    } else {
      node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;

    SetHeight(node);
    SetHeight(node->parent);
  }
}

void SmallRight(Avl* tree, Node* node) {
  if (node != NULL) {
    Node* temp = node->left;
    node->left = temp->right;
    if (temp->right != NULL) {
      temp->right->parent = node;
    }

    temp->parent = node->parent;
    if (node->parent == NULL) {
      tree->root = temp;
    } else if (node == node->parent->right) {
      node->parent->right = temp;
    } else {
      node->parent->left = temp;
    }

    temp->right = node;
    node->parent = temp;

    SetHeight(node);
    SetHeight(node->parent);
  }
}

void BigLeft(Avl* tree, Node* node) {
  if (node != NULL) {
    SmallRight(tree, node->right);
    SmallLeft(tree, node);
  }
}

void BigRight(Avl* tree, Node* node) {
  if (node != NULL) {
    SmallLeft(tree, node->left);
    SmallRight(tree, node);
  }
}

int Balance(Node* node) {
  return GetHeight(node->right) - GetHeight(node->left);
}

void FixUp(Avl* tree, Node* node) {
  while (node != NULL) {
    if (Balance(node) == first_height) {
      if (Balance(node->right) < 0) {
        BigLeft(tree, node);
      } else {
        SmallLeft(tree, node);
      }
    }

    if (Balance(node) == second_height) {
      if (Balance(node->left) > 0) {
        BigRight(tree, node);
      } else {
        SmallRight(tree, node);
      }
    }
    SetHeight(node);
    node = node->parent;
  }
}

void Insert(Avl* tree, int key) {
  Node* new_parent = nullptr;
  Node* new_leaf = tree->root;
  Node* node = (Node*)calloc(1, sizeof(Node));
  node->parent = new_parent;
  node->key = key;

  if (tree->size == 0) {
    tree->root = node;
  } else {
    while (new_leaf != NULL) {
      new_parent = new_leaf;

      if (key < new_leaf->key) {
        new_leaf = new_leaf->left;
      } else {
        new_leaf = new_leaf->right;
      }
    }

    node->parent = new_parent;
    if (!(new_parent == nullptr)) {
      tree->root = node;
    } else if (node->key < new_parent->key) {
      new_parent->left = node;
    } else {
      new_parent->right = node;
    }
    FixUp(tree, node);
  }
  tree->size++;
}

Node* FindSubtree(Node* subtree_root, int key) {
  if (subtree_root == NULL) {
    return NULL;
  }

  if (key < subtree_root->key) {
    return FindSubtree(subtree_root->left, key);
  }

  if (subtree_root->key < key) {
    return FindSubtree(subtree_root->right, key);
  }
  return subtree_root;
}

Node* Find(Avl* tree, int key) { return FindSubtree(tree->root, key); }

int UpperBoundInTree(Node* subtree_root, int key) {
  if (subtree_root == NULL) {
    return max;
  }

  if (key == subtree_root->key) {
    max = subtree_root->key;
    return max;
  }

  if (key < subtree_root->key) {
    if (subtree_root->key < max) {
      max = subtree_root->key;
    }
    UpperBoundInTree(subtree_root->left, key);
  }

  if (subtree_root->key < key) {
    UpperBoundInTree(subtree_root->right, key);
  }
  return max;
}

int FindUpperBound(Avl* tree, int key) {
  max = max_const;
  return UpperBoundInTree(tree->root, key);
}

int main() {
  Avl* tree = NewAvl();
  int max = max_const;

  int count = 0;
  scanf("%d", &count);

  char commmand = '\0';
  int64_t number = 0;
  int64_t result = start_result;
  for (int i = 0; i < count; ++i) {
    scanf("\n%c %d", &commmand, &number);

    if (commmand == '?') {
      result = FindUpperBound(tree, number);
      if (result == max_const) {
        result = start_result;
      }
      printf("%d\n", result);
    } else {
      if (result == start_result) {
        Insert(tree, number);
      } else {
        Insert(tree, ((number + result) % del));
        result = start_result;
      }
    }
  }

  DeleteAvl(tree);

  return 0;
}
