#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int max_str_size = 550;
const int first_height = 2;
const int second_height = -2;

struct Map {
  char* str1 = NULL;
  char* str2 = NULL;
};

void WriteStrings(Map* map, char* str1, char* str2);

struct Node {
  Map* key = NULL;
  Node* parent = NULL;
  Node* left = NULL;
  Node* right = NULL;
  unsigned int height = 0;
};

void WriteStrings(Map* map, char* str1, char* str2) {
  map->str1 = (char*)calloc(max_str_size, sizeof(char));
  strcpy(map->str1, str1);
  map->str2 = (char*)calloc(max_str_size, sizeof(char));
  strcpy(map->str2, str2);
}

inline int GetHeight(Node* node);
inline void SetHeight(Node* node);

struct Avl {
  Node* root = NULL;
  size_t size = 0;
};

Avl* NewAvl();
void DeleteAvl(Avl* tree);

void SmallLeft(Avl* tree, Node* node);
void SmallRight(Avl* tree, Node* node);
void BigLeft(Avl* tree, Node* node);
void BigRight(Avl* tree, Node* node);
Node* Find(Avl* tree, char* key);
Node* FindSubtree(Node* subtree_root, char* key);

int Balance(Node* node);
void FixUp(Avl* tree, Node* node);

void Insert(Avl* tree, Map* key);

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

void Insert(Avl* tree, Map* key) {
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
      if (strcmp(key->str1, new_leaf->key->str1) < 0) {
        new_leaf = new_leaf->left;
      } else {
        new_leaf = new_leaf->right;
      }
    }

    node->parent = new_parent;
    if (!(new_parent == nullptr)) {
      tree->root = node;
    } else if (strcmp(node->key->str1, new_parent->key->str1) < 0) {
      new_parent->left = node;
    } else {
      new_parent->right = node;
    }

    FixUp(tree, node);
  }

  tree->size++;
}

Node* FindSubtree(Node* subtree_root, char* key) {
  if (subtree_root == NULL) {
    return NULL;
  }

  if (strcmp(key, subtree_root->key->str1) < 0) {
    return FindSubtree(subtree_root->left, key);
  }

  if (strcmp(subtree_root->key->str1, key) < 0) {
    return FindSubtree(subtree_root->right, key);
  }

  return subtree_root;
}

Node* Find(Avl* tree, char* key) { return FindSubtree(tree->root, key); }

int main() {
  int count = 0;
  scanf("%d", &count);

  char* str1 = (char*)calloc(max_str_size, sizeof(char));
  char* str2 = (char*)calloc(max_str_size, sizeof(char));

  Avl* tree = NewAvl();
  for (int i = 0; i < count; ++i) {
    scanf("%s%s", str1, str2);
    Map* map1 = (Map*)calloc(1, sizeof(Map));
    WriteStrings(map1, str1, str2);
    Insert(tree, map1);
    Map* map2 = (Map*)calloc(1, sizeof(Map));
    WriteStrings(map2, str2, str1);
    Insert(tree, map2);
  }

  printf("\n");

  scanf("%d", &count);
  for (int i = 0; i < count; ++i) {
    scanf("%s", str1);
    Node* node = Find(tree, str1);
    printf("%s\n", node->key->str2);
  }

  DeleteAvl(tree);

  return 0;
}