#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum Operation {
    NON_OP = -1,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    OP_LOG,
    OP_EXP,
    OP_SIN,
    OP_COS,
    OP_TAN,
    OP_SQRT
};

#define dL (differentiate(tree, node->left))
#define dR (differentiate(tree, node->right))
#define cL (copyTree     (tree, node->left))
#define cR (copyTree     (tree, node->right))

#define create_CONST(num) createNode(CONST, createConstantData(num), nullptr, nullptr)

#define create_ADD(l, r) createNode(node->type, {OP_ADD}, l, r)
#define create_SUB(l, r) createNode(node->type, {OP_SUB}, l, r)
#define create_MUL(l, r) createNode(node->type, {OP_MUL}, l, r)
#define create_DIV(l, r) createNode(node->type, {OP_DIV}, l, r)
#define create_POW(l, r) createNode(node->type, {OP_POW}, l, r)

#define create_SQRT(r) createNode(node->type, {OP_SQRT}, nullptr, r)
#define create_EXP(r) createNode(node->type, {OP_EXP}, nullptr, r)
#define create_LOG(r) createNode(node->type, {OP_LOG}, nullptr, r)
#define create_SIN(r) createNode(node->type, {OP_SIN}, nullptr, r)
#define create_COS(r) createNode(node->type, {OP_COS}, nullptr, r)

struct Text {
    char* buffer     = nullptr;
    size_t text_size = 0;
};

enum NodeType {
    CONST,
    VAR,
    OPER
};

union Data {
    Operation op;
    char var;
    double constant;
};

struct Node {
    NodeType type;
    Data data;
    Node* parent = nullptr;
    Node* left   = nullptr;
    Node* right  = nullptr;
};

struct Tree {
    Node* root = nullptr;
    int size   = 0;
};

const char* OP[11] = { "+", "-", "*", "/", "^", "log", "exp", "sin", "cos", "tan", "sqrt" };
const char* Unary[6] = { "log", "exp", "sin", "cos", "tan", "sqrt" };

Text readFileToBuffer(const char* file_name);
size_t getFileSize   (const char* file_name);
void textConstruct   (Text* text);

Tree* newTree();
Text* newText();
void fillNode           (Text* text, Node** leaf, int* ofs);
bool searchBinary       (Text* text, Node** leaf, int* ofs);
bool searchUnary        (Text* text, Node** leaf, int* ofs);
void makeRightNode      (Tree* tree, Node** leaf);
void makeLeftNode       (Tree* tree, Node* leaf);
Node* differentiate     (Tree* tree, Node* node);
void buildTree          (Text* text, Tree* tree, int* ofs);
void makeRoot           (Tree* tree);
void constSimplification(Tree* tree, Node* node, bool* is_change);
void neutralOperationSimplification(Tree* tree, Node* node, bool* is_change);
void binaryOperation    (Tree* tree, Node* node);
void unaryOperation     (Tree* tree, Node* node);
void simplification     (Tree* tree);
Node* makeNode();

void getNewParent(Tree* tree, Node* node);
void getNewParentForConst(Tree* tree, Node* node, double value);

void addSimplification(Tree* tree, Node* node);
void subSimplification(Tree* tree, Node* node);
void mulSimplification(Tree* tree, Node* node);
void divSimplification(Tree* tree, Node* node);
void powSimplification(Tree* tree, Node* node);
void addSimplification(Tree* tree, Node* node);
void expSimplification(Tree* tree, Node* node);

void PrintExpressionRecursively(Tree* tree, Node* node, FILE* tech_file);