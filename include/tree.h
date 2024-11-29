#ifndef TREE_H
#define TREE_H

#include <stdio.h>

const int sizeOfTable = 50;

#define DO_CREATE_TREE(tree, pathToLog) {                                                      \
	createTree(tree, pathToLog, __FILE__, __LINE__);                                           \
}

enum TypeOfValue
{
    UNK = 0,
    NUM = 1,
    VAR = 2,
    OP  = 3,
    EMP = 4
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

#define CREATE_EMP        createNode(NULL, EMP, 0,     0,          0)
#define _ADD(left, right) createNode(NULL, OP,  ADD,   left,       right)
#define _SUB(left, right) createNode(NULL, OP,  SUB,   left,       right)
#define _MUL(left, right) createNode(NULL, OP,  MUL,   left,       right)
#define _DIV(left, right) createNode(NULL, OP,  DIV,   left,       right)
#define _DEG(left, right) createNode(NULL, OP,  DEG,   left,       right)
#define _LOG(left, right) createNode(NULL, OP,  LOG,   left,       right)
#define _COS(right)       createNode(NULL, OP,  COS,   CREATE_EMP, right)
#define _SIN(right)       createNode(NULL, OP,  SIN,   CREATE_EMP, right)
#define _NUM(value)       createNode(NULL, NUM, value, NULL,       NULL)
#define _VAR(tree, value) createNode(tree, VAR, value, NULL,       NULL)

int searchAtVarTable(Tree * tree, char symbol);

int addToVarTable(Tree * tree, char symbol);

#endif // TREE_H
