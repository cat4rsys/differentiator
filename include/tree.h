#ifndef TREE_H
#define TREE_H

#include <stdio.h>

const int sizeOfTable = 50;

#define DO_CREATE_TREE(tree, pathToLog) {                                                      \
	createTree(tree, pathToLog, __FILE__, __LINE__);                                           \
}

enum TypeOfValue
{
    NUM = 1,
    VAR = 2,
    OP  = 3
};

enum Operations
{
    ADD  = 1,
    SUB  = 2,
    MUL  = 3,
    DIV  = 4,
    DEG  = 5,
    LOG  = 6,
    SIN  = 7,
    COS  = 8,
    TG   = 9,
    CTG  = 10,
    ASIN = 11,
    ACOS = 12,
    ATG  = 13,
    ACTG = 14,
    SH   = 15,
    CH   = 16,
    TH   = 17
};

typedef double Value_t;

typedef struct Node
{
    TypeOfValue typeOfData;
    Value_t data;

    struct Node * left;
    struct Node * right;
} Node;

enum TreeError 
{
    NORMAL      = 0,
    NULL_CALLOC = 1 << 1,
    LOG_FAIL    = 1 << 2,
    NULL_PTR    = 1 << 3,
    UNKNOWN_ERR = 1 << 4
};

typedef struct
{
    char symbol;
    Value_t value;
} VarElement;

typedef struct Tree
{
    Node * firstNode;
    TreeError codeOfError;

    FILE * logFile;
    int numberOfOperation;

    VarElement * tableOfVar;
} Tree;

TreeError createTree(Tree * newTree, const char * pathToLog, const char * file, int line);

Node * createNode(Tree * tree, TypeOfValue type, Value_t value, Node * left, Node * right);

TreeError destructTree(Tree * tree);

void _freeNode(Node * node);

#define ADD(left, right) createNode(NULL, OP,  ADD,   left, right)
#define SUB(left, right) createNode(NULL, OP,  SUB,   left, right)
#define MUL(left, right) createNode(NULL, OP,  MUL,   left, right)
#define DIV(left, right) createNode(NULL, OP,  DIV,   left, right)
#define DEG(left, right) createNode(NULL, OP,  DEG,   NULL, NULL)
#define NUM(value)       createNode(NULL, NUM, value, NULL, NULL)
#define VAR(tree, value) createNode(tree, VAR, value, NULL, NULL)

int searchAtVarTable(Tree * tree, char symbol);

int addToVarTable(Tree * tree, char symbol);

#endif // TREE_H
