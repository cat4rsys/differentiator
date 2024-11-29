#ifndef DIFF_H
#define DIFF_H

#include "tree.h"

const double e = 2.718281828459045;

Node * clone(Node * actualNode);

Node * diff(Tree * tree, Node * actualNode, char var);

#define IS_ZERO(node)     (node) && ((node)->typeOfData == NUM) && (compareDouble((node)->data, 0) == EQUALS)
#define IS_ONE(node)      (node) && ((node)->typeOfData == NUM) && (compareDouble((node)->data, 1) == EQUALS)
#define IS_NUM(node)      (node) && ((node)->typeOfData == NUM)
#define IS_VAR(node)      (node) && ((node)->typeOfData == VAR)
#define IS_EMP(node)      (node) && ((node)->typeOfData == VAR)
#define OP_TYPE(node)     (int)(node)->data
#define IS_EMP_NORM(node) (node) && (node->right) && (node->left) && (IS_EMP(node->right)) && (IS_NUM(node->left)) && (OP_TYPE(node) != ADD) && \
                          (OP_TYPE(node) != SUB) && (OP_TYPE(node) != MUL) && (OP_TYPE(node) != DIV) && (OP_TYPE(node) != DEG) && (OP_TYPE(node) != LOG)

int changeNodes(Node * parentNode, Node * prevNode, Node * newNode);

void doOperation(Node * operationNode);

int simplifyEquation(Node * root);

#endif // DIFF_H

