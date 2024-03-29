#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <ctype.h>
#include "stack.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

const int START_POSITION = 1;
const int WORD_MAX_LEN = 255;
const int START_STACK_SIZE = 50000;

struct Line {
    char* string = NULL;
    size_t str_idx = 0;
};

struct Text {
    char* buffer       = NULL;
    size_t text_size   = 0;
    size_t lines_count = 0;
    Line* lines        = NULL;
};

struct Tree {
    Node* root  = NULL;
    bool change = true;
};

enum Commands {
    first  = 1,
    second = 2,
    third  = 3,
    fourth = 4,
    fifth  = 5,
    sixth  = 6
};

//----
Text* newText();
void textConstruct   (Text* text);
void getLines        (Text* text);
size_t getFileSize   (const char* file_name);
Text readFileToBuffer(const char* file_name);
size_t getLinesCount (char* buffer, size_t buffer_size);
//----

//----
Tree* buildTree  (Text* text);
void doCommand   (Text* text, Tree* tree);
void makeRoot    (Text* text, Tree* tree);
void makeLeafs   (Text* text, Node* node, int line_idx);
bool myIsAlphabet(char symbol);
//----

//----
Node* loadNodes (FILE* file, Node* node);
void graphvizBST(Tree* bst);
//----

//----
void saveNewFile        (Tree* tree, Node* node, FILE* data);
void playGame           (Tree* tree, Node* node);
void putInStack         (Tree* tree, Node* node, Stack* stack);
Node* findWord          (Tree* tree, Node* node, char* definition);
void save               (Tree* tree);
void findWordDefinition (Tree* tree);
void comparator         (Tree* tree);
void getDifferendBetween(Node* node);
void writeDefinition    (Stack* stack, Node* node);
void writeDifferent     (Stack* stk1, char* first, Stack* stk2, char* second);
void deleteSymbol       (char* string);
bool isfindWord();
//----

int main()
{
    Text* text = newText();
    Tree* tree = buildTree(text);

    printf("Hello! My name is Akinator, I can predict some words!\n\n");

    printf("Instruction: (+) - yes\n"
           "             (-) - no \n\n");

    printf("Main menu: 1-Start game     \n"
           "           2-Give definition\n"
           "           3-Compare words  \n"
           "           4-Draw tree      \n"
           "           5-Save           \n"
           "           6-Exit           \n");

    doCommand(text, tree);

    return 0;
}

void doCommand(Text* text, Tree* tree)
{
    int command = 0;
    while (command != sixth)
    {
        printf("Select a number to continue:");
        scanf("%d", &command);
        printf("\n");
    
        while (command < first || command > sixth)
        {
            printf("Wrong input! Enter correct number of command:");
            scanf("%d", &command);
            printf("\n");
        }

        switch (command)
        {
            case(first):
            {
                playGame(tree, tree->root);

                break;
            }
            case(second):
            {
                printf("Enter word:");
                findWordDefinition(tree);

                break;
            }
            case(third):
            {
                comparator(tree);

                break;
            }
            case(fourth):
            {
                graphvizBST(tree);

                break;
            }
            case(fifth):
            {
                save(tree);

                break;
            }
            case(sixth):
            {
                printf("Closing game, thank you and goodbye!\n");

                break;
            }
        }
    
    }
}

Text* newText()
{
    Text* text = (Text*)calloc(1, sizeof(Text));
    assert(text != NULL);
    text->text_size = 0;
    text->lines_count = 0;

    textConstruct(text);

    return text;
}

void textConstruct(Text* text)
{
    assert(text != NULL);

    getFileSize("data.txt");
    *text = readFileToBuffer("data.txt");
    text->lines_count = getLinesCount(text->buffer, text->text_size);
    getLines(text);
}

size_t getFileSize(const char* file_name)
{
    assert(file_name != NULL);

    struct stat fileStat = {};
    stat(file_name, &fileStat);

    return fileStat.st_size;
}

Text readFileToBuffer(const char* file_name)
{
    assert(file_name != NULL);

    FILE* file_descriptor = fopen("data.txt", "r");

    size_t file_size = getFileSize(file_name);
    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    assert(buffer != NULL);
    file_size = fread(buffer, sizeof(char), file_size + 1, file_descriptor);

    fclose(file_descriptor);

    Text text_params = { buffer, file_size };

    return text_params;
}

size_t getLinesCount(char* buffer, size_t buffer_size)
{
    assert(buffer != NULL);

    size_t lines_count = 1;
    for (size_t index = 0; index < buffer_size; ++index)
    {
        if (buffer[index] == '\n')
        {
            ++lines_count;
            buffer[index] = '\0';
        }
    }
 
    return lines_count;
}

void getLines(Text* text)
{
    assert(text != NULL);

    int line_idx = 1;
    text->lines = (Line*)calloc(text->lines_count, sizeof(Line));
    assert(text->lines != NULL);
    text->lines[0].string = text->buffer;
    for (int index = 0; index < text->text_size; ++index)
    {
        if (text->buffer[index] == '\0' && line_idx < text->lines_count)
        {
            text->lines[line_idx].string = &text->buffer[index + 1];
            text->lines[line_idx].str_idx = line_idx;
            ++line_idx;
        }
    }
}

Tree* buildTree(Text* text)
{
    assert(text != NULL);

    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    assert(tree != NULL);

    makeRoot(text, tree);

    makeLeafs(text, tree->root, START_POSITION);

    return tree;
}

void makeRoot(Text* text, Tree* tree)
{    
    tree->root = (Node*)calloc(1, sizeof(Node));
    assert(tree->root != NULL);
    tree->root->data = text->lines[0].string;
}

void makeLeafs(Text* text, Node* node, int line_idx)
{
    assert(text != NULL);
    assert(node != NULL);

    Node* new_leaf = (Node*)calloc(1, sizeof(Node));
    assert(new_leaf != NULL);
    
    if (strcmp(text->lines[line_idx].string, "{") == 0)
    {
        ++line_idx;
        node->right = new_leaf;
        new_leaf->parent = node;
        new_leaf->data = text->lines[line_idx].string;

        makeLeafs(text, node->right, line_idx + 1);

        return;
    }

    while (strcmp(text->lines[line_idx].string, "}") == 0 && line_idx < text->lines_count - 1)
    {
        node = node->parent;
        ++line_idx;
    }

    if (line_idx < text->lines_count - 1)
    {
        ++line_idx;
        node->left = new_leaf;
        new_leaf->parent = node;
        new_leaf->data = text->lines[line_idx].string;

        makeLeafs(text, node->left, line_idx + 1);
    }
}

bool myIsAlphabet(char symbol)
{
    return ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'));
}

Node* loadNodes(FILE* file, Node* node)
{
    if (node->right != NULL && node->left != NULL)
    {
        fprintf(file, "\"%p\"[shape=\"ellipse\", label=\"%s?\"]\n", node, node->data);
    }
    else
    {
        fprintf(file, "\"%p\"[shape=\"septagon\", label=\"%s\"]\n", node, node->data);
    }

    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->left != nullptr)
    {
        fprintf(file, "edge[color=orange]\n\"%p\":sw->\"%p\";\n", loadNodes(file, node->left)->parent, node->left);  // link parent and left child
    }
    if (node->right != nullptr)
    {
        fprintf(file, "edge[color=red]\n\"%p\":se->\"%p\";\n", loadNodes(file, node->right)->parent, node->right); //link parent and right child
    }
   
    return node;
}

void graphvizBST(Tree* bst)
{
    if (bst->root == nullptr)
    {
        return;
    }
    FILE* file = fopen("graphAkinatorPlot.txt", "w");

    fprintf(file, "digraph G{\n");
    fprintf(file, "node [shape=\"record\", style=\"solid\", color=\"green\"];\n");

    loadNodes(file, bst->root);

    fprintf(file, "}");

    fclose(file);

    system("dot -Tjpg  graphAkinatorPlot.txt > dump.jpg");
    system("start dump.jpg");
}

void playGame(Tree* tree, Node* node)// ??????? ????
{
    if (node->left == NULL || node->right == NULL)
    {
        printf("Is it your word:%s?", node->data);
        printf("\n");

        if (isfindWord())
        {
            printf("So cool!\n");
        }
        else
        {
            getDifferendBetween(node);
            printf("Ok, now i know your word!\n\n");
            tree->change = true;
        }

        return;
    }
    
    printf("Is it:%s?\n", node->data);

    bool answer = isfindWord();

    if (answer)
    {
        playGame(tree, node->right);
    }
    else
    {
        playGame(tree, node->left);
    }
}

bool isfindWord()
{
    char* answer = (char*)calloc(1, sizeof(char));
    assert(answer != NULL);
    scanf("%s", answer);

    if (answer[0] == '+')
    {
        return true;
    }
    
    return false;
}

void getDifferendBetween(Node* node)
{
    char* new_word = (char*)calloc(WORD_MAX_LEN, sizeof(char));
    assert(new_word != NULL);

    printf("Ok, what did you mean?:");

    Node* new_word_leaf = (Node*)calloc(1, sizeof(Node));
    assert(new_word_leaf != NULL);

    scanf("%s", new_word);
    new_word_leaf->data = new_word;
    new_word_leaf->parent = node;

    printf("\n");
    printf("What different between (%s) and (%s):", new_word, node->data);

    char* new_data = (char*)calloc(WORD_MAX_LEN, sizeof(char));
    scanf("\n");
    fgets(new_data, WORD_MAX_LEN, stdin);
    deleteSymbol(new_data);

    printf("\n");

    Node* new_leaf = (Node*)calloc(1, sizeof(Node));
    assert(new_leaf != NULL);
    
    new_leaf->data = node->data;
    new_leaf->parent = node;
    node->data = new_data;
    node->left = new_leaf;
    node->right = new_word_leaf;
}

void deleteSymbol(char* string)
{
    int idx = 0;
    while (string[idx] != '\n')
    {
        ++idx;
    }

    string[idx] = '\0';
}

void save(Tree* tree)
{
    assert(tree);

    if (tree->change == true)
    {
        FILE* DataFile = fopen("data.txt", "w");
        saveNewFile(tree, tree->root, DataFile);
        fclose(DataFile);

        tree->change = false;
        printf("Your file was changed\n\n");
    }
    else
    {
        printf("No changes in your file\n\n");
    }
}

void saveNewFile(Tree* tree, Node* node, FILE* data)
{
    assert(tree);
    assert(node);

    fprintf(data, "%s\n", node->data);
    if (node->right != NULL)
    {
        fprintf(data, "{\n");
        saveNewFile(tree, node->right, data);
        fprintf(data, "}\n");
    }

    if (node->left !=NULL)
    {
        fprintf(data, "{\n");
        saveNewFile(tree, node->left, data);
        fprintf(data, "}\n");
    }
}

void findWordDefinition(Tree* tree)
{
    assert(tree != NULL);

    char* word = (char*)calloc(WORD_MAX_LEN, sizeof(char));
    scanf("%s", word);

    Node* find_node = findWord(tree, tree->root, word);

    if (find_node == NULL)
    {
        printf("I cant find this word\n");
    }
    else
    {
        Stack* stack = newStack(START_STACK_SIZE);
        putInStack(tree, find_node, stack);
        printf("%s-> ", find_node->data);
        writeDefinition(stack, tree->root);
        deleteStack(stack);
        printf("\n\n");
    }
}

Node* findWord(Tree* tree, Node* node, char* definition)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (strcmp(node->data, definition) == 0 && node->right == NULL && node->left == NULL)
    {
        return node;
    }

    Node* find_node = findWord(tree, node->left, definition);
    if (find_node != NULL)
    {
        return find_node;
    }

    return findWord(tree, node->right, definition);
}

void putInStack(Tree* tree, Node* node, Stack* stack)
{
    assert(tree != NULL);
    assert(node != NULL);

    while (node != NULL)
    {
        push(stack, node);
        node = node->parent;
    }
    pop(stack);
}

void writeDefinition(Stack* stack, Node* node)
{
    assert(stack != NULL);
    assert(node != NULL);

    if (stack->size_ == 0)
    {
        return;
    }

    Node* find_node = top(stack);
    pop(stack);

    if (find_node == node->right)
    {
        printf("(%s) ", node->data);
    }
    else
    {
        printf("(not %s) ", node->data);
    }

    writeDefinition(stack, find_node);
}

void comparator(Tree* tree)
{
    printf("first word:");
    char* first = (char*)calloc(WORD_MAX_LEN, sizeof(char));
    scanf("%s", first);
    printf("\n");

    Node* find_node1 = findWord(tree, tree->root, first);

    if (find_node1 == NULL)
    {
        printf("I cant find this word\n");

        return;
    }
    Stack* stk1 = newStack(START_STACK_SIZE);
    putInStack(tree, find_node1, stk1);

    printf("second word:");
    char* second = (char*)calloc(WORD_MAX_LEN, sizeof(char));
    scanf("%s", second);

    Node* find_node2 = findWord(tree, tree->root, second);

    if (find_node2 == NULL)
    {
        printf("I cant find this word\n");

        return;
    }
    Stack* stk2 = newStack(START_STACK_SIZE);
    putInStack(tree, find_node2, stk2);

    writeDifferent(stk1, first, stk2, second);

    deleteStack(stk1);
    deleteStack(stk2);
}

void writeDifferent(Stack* stk1, char* first, Stack* stk2, char* second)
{
    Node* start_node = top(stk1);

    pop(stk1);
    Node* first_tag = top(stk1);
    pop(stk2);
    Node* second_tag = top(stk2);
    if (first_tag == second_tag)
    {
        printf("\nidentical tags:\n");
        while (top(stk1) == top(stk2) && (stk1->size_ != 0 && stk2->size_ != 0))
        {
            Node* next_node = top(stk1);
            pop(stk1);
            pop(stk2);
            if (next_node == start_node->right)
            {
                printf("(%s) ", start_node->data);
            }
            else
            {
                printf("(not %s) ", start_node->data);
            }
            start_node = next_node;
        }
    }

    printf("\n\ndifferent tags:\n");

    printf("%s->", first);
    writeDefinition(stk1, start_node);

    printf("\n");

    printf("%s->", second);
    writeDefinition(stk2, start_node);

    printf("\n\n");
}