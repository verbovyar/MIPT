#include "diff.h"

bool IsMul(Node* node)
{
    return  node->data.op == OP_MUL;
}

bool IsSum(Node* node)
{
    return
        (node->data.op == OP_ADD || node->data.op == OP_SUB);
}

bool IsDiv(Node* node)
{
    return  node->data.op == OP_DIV;
}

bool IsPow(Node* node)
{
    return  node->data.op == OP_POW;
}

bool IsList(Tree* tree, Node* node)
{
    return (node->right == nullptr && node->left == nullptr);
}

bool BracketNeeded(Tree* tree, Node* node)
{
    if (node->parent != nullptr)
    {
        return (!IsList(tree, node) && (IsMul(node->parent) && IsSum(node))) ||
            (IsPow(node->parent) && node->parent->left == node && !IsList(tree, node));
    }
}
    

void PrintExpression(Tree* tree)
{
    FILE* tech_file = fopen("expression.tex", "w");
    assert(tech_file);

    fprintf(tech_file, "\\documentclass[32pt]{article}\n"
        "\\begin{document}             \n$");

    PrintExpressionRecursively(tree, tree->root, tech_file);

    fprintf(tech_file, "$\n\\end{document}\n");

    fclose(tech_file);
    system("pdflatex expression.tex");
}

void PrintExpressionRecursively(Tree* tree, Node* node, FILE* tech_file)
{
    if (node == nullptr) return;

    if (BracketNeeded(tree, node))
    {
        fprintf(tech_file, "{(");
    }
    else
    {
        fprintf(tech_file, "{");
    }

    PrintExpressionRecursively(tree, node->left, tech_file);

    if (node->type == CONST)
    {
        fprintf(tech_file, "%lg", node->data.constant);
    }
    else if (node->type == VAR)
    {
        fprintf(tech_file, "%c", node->data.var);
    }
    else
    {
        fprintf(tech_file, "%s ", OP[node->data.op]);
    }
    PrintExpressionRecursively(tree, node->right, tech_file);

    if (BracketNeeded(tree, node))
    {
        fprintf(tech_file, "})");
    }
    else
    {
        fprintf(tech_file, "}");
    }
}

Data createConstantData(double constant)
{
    Data data = {};
    data.constant = constant;
    return data;
}

Node* loadNodes(FILE* file, Node* node)
{
    if (node->type == OPER)
    {
        fprintf(file, "\"%p\"[shape=\"ellipse\",style=\"filled\",fillcolor = \"aliceblue\", label=\"%s\"]\n", node, OP[node->data.op]);
    }
    else if(node->type == VAR)
    {
        fprintf(file, "\"%p\"[shape=\"septagon\",style=\"filled\",fillcolor = \"lightpink\", label=\"%c\"]\n", node, node->data.var);
    }
    else
    {
        fprintf(file, "\"%p\"[shape=\"diamond\",style=\"filled\",fillcolor = \"olivedrab2\", label=\"%lf\"]\n", node, node->data.constant);
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

//------------

int main()
{
    Text* text = newText();
    Tree* tree = newTree();

    int ofs = 0;
    buildTree(text, tree, &ofs);
    
    Tree* diff_tree = newTree();
    diff_tree->root = differentiate(tree, tree->root);

    simplification(diff_tree);

    PrintExpression(diff_tree);

    graphvizBST(diff_tree);
    //graphvizBST(tree);

    return 0;
}

//------------

Text* newText()
{
    Text* text = (Text*)calloc(1, sizeof(Text));
    assert(text != nullptr);
    text->text_size = 0;

    textConstruct(text);

    return text;
}

void textConstruct(Text* text)
{
    assert(text != nullptr);

    getFileSize("function.txt");
    *text = readFileToBuffer("function.txt");
}

size_t getFileSize(const char* file_name)
{
    assert(file_name != nullptr);

    struct stat fileStat = {};
    stat(file_name, &fileStat);

    return fileStat.st_size;
}

Text readFileToBuffer(const char* file_name)
{
    assert(file_name != nullptr);

    FILE* file_descriptor = fopen("function.txt", "r");

    size_t file_size = getFileSize(file_name);
    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    assert(buffer != nullptr);
    file_size = fread(buffer, sizeof(char), file_size + 1, file_descriptor);

    fclose(file_descriptor);

    Text text_params = { buffer, file_size };

    return text_params;
}

Node* makeNode()
{
    Node* node = (Node*)calloc(1, sizeof(Node));

    node->parent = nullptr;
    node->left = nullptr;
    node->right = nullptr;

    node->data.op = NON_OP;

    return node;
}

void makeRoot(Tree* tree)
{
    assert(tree != nullptr);

    Node* node = makeNode();

    tree->root = node;
    ++tree->size;
}

Tree* newTree()
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    assert(tree != nullptr);

    makeRoot(tree);

    return tree;
}

void makeLeftNode(Tree* tree, Node** leaf)
{
    assert(tree != nullptr);
    assert(leaf != nullptr);

    Node* new_node = makeNode();

    if (tree->size == 1)
    {
        tree->root->left = new_node;
        new_node->parent = tree->root;
    }
    else
    {
        (*leaf)->left = new_node;
        new_node->parent = *leaf;
    }
    *leaf = new_node;

    ++tree->size;
}

void makeRightNode(Tree* tree, Node** leaf)
{
    assert(tree != nullptr);
    assert(leaf != nullptr);

    Node* new_node = makeNode();

    (*leaf)->right = new_node;
    new_node->parent = *leaf;
    *leaf = (*leaf)->right;

    ++tree->size;
}

bool searchUnary(Text* text, Node** leaf, int* ofs)
{
    int size = strcspn(&text->buffer[*ofs], " (");
    char temp = (text)->buffer[*ofs + size];
    text->buffer[*ofs + size] = '\0';

    for (int idx = 0; idx < 6; ++idx)
    {
        if (strcmp(&text->buffer[*ofs], Unary[idx]) == 0)
        {
            (*leaf)->data.op = (Operation)(idx + 5);

            break;
        }
    }

    text->buffer[*ofs + size] = temp;
    *ofs += size - 1;

    return ((*leaf)->data.op != NON_OP);
}

bool searchBinary(Text* text, Node** leaf, int* ofs)
{
    char temp = (text)->buffer[*ofs + 1];
    text->buffer[*ofs + 1] = '\0';

    for (int idx = 0; idx < 6; ++idx)
    {
        if (strcmp(&text->buffer[*ofs], OP[idx]) == 0)
        {
            (*leaf)->data.op = (Operation)idx;

            break;
        }
    }

    text->buffer[*ofs + 1] = temp;

    return ((*leaf)->data.op != NON_OP);
}

void typeOfOperation(Text* text, Node** leaf, int* ofs)
{
    if (isalpha(text->buffer[*ofs]))
    {
        searchUnary(text, leaf, ofs);
    }
    else
    {
        searchBinary(text, leaf, ofs);
    }
}

void fillNode(Text* text, Node** leaf, int* ofs)
{      
    assert(text != nullptr);
    assert(leaf != nullptr);

    for (int j = 0; j < 11; ++j)
    {
        if (text->buffer[*ofs] == *OP[j] || (isalpha(text->buffer[*ofs]) && isalpha(text->buffer[*ofs + 1])))
        {
            (*leaf)->type = OPER;
            typeOfOperation(text, leaf, ofs);

            break;
        }
        else if (isdigit(text->buffer[*ofs]))
        {
            int size = 0;

            (*leaf)->type = CONST;
            sscanf(&text->buffer[*ofs], "%lf%n", &((*leaf)->data.constant), &size);
            *ofs += size - 1;

            break;
        }
        else if (isalpha(text->buffer[*ofs]))
        {
            (*leaf)->type = VAR;
            (*leaf)->data.var = text->buffer[*ofs];

            break;
        }
    }
}

void buildTree(Text* text, Tree* tree, int* ofs)
{
    assert(text != nullptr);
    assert(tree != nullptr);

    Node* leaf = makeNode();
    for (int idx = 1; idx < text->text_size; ++idx)
    {
        if (text->buffer[idx] == '(' && leaf->left == nullptr)
        {
            makeLeftNode(tree, &leaf);
        }
        else if (text->buffer[idx] == '(' && leaf->left != nullptr)
        {
            makeRightNode(tree, &leaf);
        }
        else if (text->buffer[idx] != '(' && text->buffer[idx] != ')')
        {
            fillNode(text, &leaf, &idx);
        }
        else if (text->buffer[idx] == ')' && leaf->parent != nullptr)
        {
            leaf = leaf->parent;
        }
    }
}

//------ Diff

Node* createNode(NodeType type, Data data, Node* left, Node* right)
{
    Node* node = makeNode();

    node->type = type;
    node->data = data;
    node->parent = nullptr;
    node->left = left;
    node->right = right;

    if (left != nullptr) 
    { 
        left->parent = node; 
    }
    if (right != nullptr) 
    { 
        right->parent = node; 
    }

    return node;
}

void freeNode(Node* node)
{
    node->parent = nullptr;
    node->right = nullptr;
    node->left = nullptr;

    free(node);
}

Node* copyTree(Tree* tree, Node* node)
{
    if (node == nullptr)
    { 
        return nullptr; 
    }

    return createNode(node->type, node->data, copyTree(tree, node->left), copyTree(tree, node->right));
}

Node* differentiate(Tree* tree, Node* node)
{
    assert(node != nullptr);

    if (node->type == CONST) return create_CONST(0);
    if (node->type == VAR)   return create_CONST(1);

    Operation operation = node->data.op;

    switch (operation)
    {
        case OP_ADD: return create_ADD(dL, dR); 

        case OP_SUB: return create_SUB(dL, dR);

        case OP_MUL: return create_ADD(create_MUL(dL, cR), create_MUL(cL, dR));

        case OP_DIV: return create_DIV(create_SUB(create_MUL(dL, cR), create_MUL(cL, dR)), create_MUL(cR, cR));

        case OP_SIN: return create_MUL(create_COS(cL), dL);

        case OP_COS: return create_MUL(create_MUL(create_SIN(cL), dL), create_CONST(-1));

        case OP_LOG: return create_MUL(dL, create_DIV(create_CONST(1), cL));

        case OP_EXP: return create_MUL(copyTree(tree, node), dL);

        case OP_POW: return create_MUL(create_MUL(cR, create_POW(cL, create_SUB(cR, create_CONST(1)))), dL);// ??????!!!!

        case OP_SQRT: return create_MUL(create_DIV(create_CONST(1), create_MUL(create_CONST(2), create_SQRT(cL))), dL);
    }

    return nullptr;
}

//----------

#undef dR
#undef dL
#undef cR
#undef cL
#undef create_ADD
#undef create_SUB
#undef create_MUL
#undef create_DIV
#undef create_SIN
#undef create_COS
#undef create_SQRT
#undef create_CONST
#undef create_LOG
#undef create_EXP
#undef create_POW

//---------- Symplify

void simplification(Tree* tree)
{
    assert(tree != nullptr);
    
    bool is_change = true;
    while (is_change)
    {
        is_change = false;
        constSimplification(tree, tree->root, &is_change);
        neutralOperationSimplification(tree, tree->root, &is_change);
    }
}

void constSimplification(Tree* tree, Node* node, bool* is_change)
{
    assert(tree != nullptr);
    assert(node != nullptr);

    if (node->left == nullptr || node->right == nullptr)
    {
        return;
    }

    constSimplification(tree, node->right, is_change);
    constSimplification(tree, node->left, is_change);

    if (node->right->type == CONST && node->left->type == CONST)
    {
        binaryOperation(tree, node);
        node->type = CONST;
        *is_change = true;
    }
    else if (node->right->type == CONST && node->left == nullptr)
    {
        unaryOperation(tree, node);
        node->type = CONST;
        *is_change = true;
    }
}

void binaryOperation(Tree* tree, Node* node)
{
    assert(tree != nullptr);
    assert(node != nullptr);

    Operation operation = node->data.op;

    switch (operation)
    {
        case OP_ADD:
        {
            node->data.constant = node->left->data.constant + node->right->data.constant;

            break;
        }
        case OP_SUB:
        {
            node->data.constant = node->left->data.constant - node->right->data.constant;

            break;
        }
        case OP_MUL:
        {
            node->data.constant = node->left->data.constant * node->right->data.constant;

            break;
        }
        case OP_DIV:
        {
            if (node->right->data.constant != 0)
            {
                node->data.constant = node->left->data.constant / node->right->data.constant;
            }
            else
            {
                printf("DIV ON ZERO!\n");
            }

            break;
        }
        case OP_POW:
        {
            node->data.constant = pow(node->left->data.constant, node->right->data.constant);

            break;
        }
    }

    node->left = nullptr;
    node->right = nullptr;
    free(node->left);
    free(node->right);
}

void unaryOperation(Tree* tree, Node* node)
{
    assert(tree);
    assert(node);

    Operation operation = node->data.op;

    switch (operation)
    {
        case OP_SIN :
        {
            node->data.constant = sin(node->right->data.constant);

            break;
        }
        case OP_COS :
        {
            node->data.constant = cos(node->right->data.constant);

            break;
        }
        case OP_TAN :
        {
            node->data.constant = tan(node->right->data.constant);

            break;
        }
        case OP_SQRT :
        {
            if (node->left->data.constant >= 0)
            {
                node->data.constant = sqrt(node->right->data.constant);
            }
            else
            {
                printf("ERROR");
            }

            break;
        }
        case OP_EXP :
        {
            node->data.constant = exp(node->right->data.constant);

            break;
        }
    }

    freeNode(node->right);
}

void neutralOperationSimplification(Tree* tree, Node* node, bool* is_change)
{
    assert(tree != nullptr);
    assert(node != nullptr);

    if (node->left == nullptr || node->right == nullptr)
    {
        return;
    }

    neutralOperationSimplification(tree, node->right, is_change);
    neutralOperationSimplification(tree, node->left, is_change);

    Operation operation = node->data.op;

    if (node->left != nullptr && node->right != nullptr)
    {
        switch (operation)
        {
            case OP_ADD:
            {
                addSimplification(tree, node);

                break;
            }
            case OP_SUB:
            {
                subSimplification(tree, node);

                break;
            }
            case OP_MUL:
            {
                mulSimplification(tree, node);

                break;
            }
            case OP_DIV:
            {
                divSimplification(tree, node);

                break;
            }
            case OP_POW:
            {
                powSimplification(tree, node);

                break;
            }
            case OP_EXP:
            {
                expSimplification(tree, node);

                break;
            }
        }
    }
    else
    {
        *is_change = true;
    }
}
void addSimplification(Tree* tree, Node* node)
{
    assert(tree != nullptr);
    assert(node != nullptr);

    if (node->right->type == CONST && node->right->data.constant == 0)
    {
        getNewParent(tree, node->left);
    }
    else if (node->left->type == CONST && node->left->data.constant == 0)
    {
        getNewParent(tree, node->right);
    }
}

void subSimplification(Tree* tree, Node* node)
{
    assert(tree != nullptr);
    assert(node != nullptr);

    if (node->right->type == CONST && node->right->data.constant == 0)
    {
        getNewParent(tree, node->left);
    }
}

void mulSimplification(Tree* tree, Node* node)
{
    assert(tree != nullptr);
    assert(node != nullptr);

    if (node->right->data.constant == 0 && node->right->type == CONST)
    {
        getNewParentForConst(tree, node->left, 0);
    }
    else if(node->left->data.constant  == 0 && node->left->type  == CONST)
    {
        getNewParentForConst(tree, node->right, 0);
    }
    else if (node->right->data.constant == 1 && node->right->type == CONST)
    {
        getNewParent(tree, node->left);
    }
    else if (node->left->data.constant == 1 && node->left->type == CONST)
    {
        getNewParent(tree, node->right);
    }
}

void divSimplification(Tree* tree, Node* node)
{
    assert(tree != nullptr);
    assert(node != nullptr);

    if (node->left->data.constant == 0 && node->left->type == CONST)
    {
        getNewParentForConst(tree, node, 0);
    }
    else if (node->right->data.constant == 1 && node->right->type == CONST)
    {
        getNewParent(tree, node->left);
    }
}

void powSimplification(Tree* tree, Node* node)
{
    assert(tree != nullptr);
    assert(node != nullptr);

    if (node->right->data.constant == 0 && node->right->type == CONST)
    {
        getNewParentForConst(tree, node, 1);
    }
    else if (node->right->data.constant == 1 && node->right->type == CONST)
    {
        getNewParent(tree, node->left);
    }
    else if (node->left->data.constant == 0 && node->left->type == CONST)
    {
        getNewParentForConst(tree, node, 0);
    }
    else if (node->left->data.constant == 1 && node->left->type == CONST)
    {
        getNewParentForConst(tree, node, 1);
    }
}

void expSimplification(Tree* tree, Node* node)
{
    assert(tree != nullptr);
    assert(node != nullptr);

    if (node->right->type == 0)
    {
        getNewParentForConst(tree, node, 1);
    }
}

void getNewParent(Tree* tree, Node* node)// ????? ???????? (?????????)!!!!!!!!!!
{
    if (node->parent->right == node)
    {
        node->parent->right = nullptr;
    }
    else
    {
        node->parent->left = nullptr;
    }

    Node* prev_parent_node = node->parent;
    node->parent = node->parent->parent;

    if (prev_parent_node->parent != nullptr)
    {
        if (prev_parent_node->parent->right == prev_parent_node)
        {
            prev_parent_node->parent->right = node;
        }
        else
        {
            prev_parent_node->parent->left = node;
        }
    }
    else
    {
        tree->root = node;
    }

    freeNode(prev_parent_node);
}

void getNewParentForConst(Tree* tree, Node* node, double value)
{
    assert(tree);
    assert(node);

    getNewParent(tree, node);

    node->data.constant = value;
    node->type = CONST;
}
