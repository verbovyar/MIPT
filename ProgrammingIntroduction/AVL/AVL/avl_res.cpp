#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"
#include "map.h"

const size_t MAX_STR_SIZE = 550;

void graphvizBST(AVL* bst);

int main()
{
	AVL* tree = newAVL();

	int count = 0;
	scanf("%d", &count);

    char* comand[1] = {};

    for (int i = 0; i < count; ++i)
    {
        scanf("%s", comand);

    }

	return 0;
}

Node* loadNudes(FILE* file, Node* node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->left != nullptr)
    {
        fprintf(file, "edge[color=orange]\n\"%p\":sw->\"%p\";\n", loadNudes(file, node->left)->parent, node->left);  // link parent and left child
    }
    if (node->right != nullptr)
    {
        fprintf(file, "edge[color=red]\n\"%p\":se->\"%p\";\n", loadNudes(file, node->right)->parent, node->right); //link parent and right child
    }
    if (node->parent != nullptr)
    {
        fprintf(file, "\"%p\" [label=\"{{par: %p|ptr: %p}|{str: %s}|{H: %d}|{l:%p|r:%p}}\"]\n ", node, (node->parent), node,
            ((Map*)(node->key))->str1, node->height, node->left, node->right); // <-- label printing
    }
    else
    {
        //printf("parent is null\n");
        fprintf(file, "\"%p\" [label=\"{{par: %s}|{str: %s}|{H: %d}}\"]\n ", node, "root",
            ((Map*)(node->key))->str1, getHeight(node)); // <-- label printing
    }
    return node;
}


void graphvizBST(AVL* bst)
{
    if (bst->root == nullptr)
    {
        return;
    }
    FILE* file = fopen("graphPlot.txt", "w");

    fprintf(file, "digraph G{\n");
    fprintf(file, "node [shape=\"record\", style=\"solid\", color=\"green\"];\n");

    loadNudes(file, bst->root);

    fprintf(file, "}");

    fclose(file);

    system("dot -Tjpg  graphPlot.txt > dump.jpg");
}
