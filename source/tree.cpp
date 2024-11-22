#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include "log.h"

TreeError createTree(Tree * newTree, const char * pathToLog, const char * file, int line)
{
    if (!newTree) return NULL_PTR;

    newTree->logFile           = fopen(pathToLog, "w");
    newTree->numberOfOperation = 0;

    newTree->tableOfVar        = (VarElement *)calloc(sizeOfTable, sizeof(VarElement));

    if (!newTree->logFile) {
        newTree->codeOfError = LOG_FAIL;
        return LOG_FAIL;
    }

    fprintf(newTree->logFile, "<pre><span style=\"font-size: 16px\">\n\n");

    DO_LOG_DUMP(line, file, newTree)
    
    return NORMAL;
}

Node * createNode(Tree * tree, TypeOfValue type, Value_t value, Node * left, Node * right)
{
    Node * newNode = (Node *)calloc(1, sizeof(Node));
    if (!newNode) return NULL;

    newNode->typeOfData = type;
    newNode->left       = left;
    newNode->right      = right;

    if (type != VAR) newNode->data = value;
    else {
        int searchResult = searchAtVarTable(tree, (char)value);

        newNode->data = (searchResult == -1) ? addToVarTable(tree, (char)value) : searchResult;
    }

    return newNode;
}

TreeError destructTree(Tree * tree)
{
    _freeNode(tree->firstNode);
    fclose(tree->logFile);

    return NORMAL;
}

void _freeNode(Node * node)
{
    if (node->left) _freeNode(node->left);
    if (node->right) _freeNode(node->right);

    free(node);
}

int searchAtVarTable(Tree * tree, char symbol)
{
    if ( !tree ) return -1;
    for (int i = 0; i < sizeOfTable; i++) {
        if (tree->tableOfVar[i].symbol == symbol) return i;
    }

    return -1;
}

int addToVarTable(Tree * tree, char symbol)
{
    if ( !tree ) return -1;
    for (int i = 0; i < sizeOfTable; i++) {
        if (tree->tableOfVar[i].symbol == 0) {
            tree->tableOfVar[i].symbol = symbol;
            return i;
        }
    }

    return -1;
}

