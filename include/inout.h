#ifndef INOUT_H
#define INOUT_H

#include "tree.h"
#include <stdio.h>

void printInfix(FILE * outputFile, Node * actualNode, Tree * tree);

void printPrefix(FILE * outputFile, Node * actualNode, Tree * tree);

void printValue(FILE * outputFile, Value_t value, TypeOfValue type, Tree * tree);

int printOperation(FILE * outputFile, char code);

#endif // INOUT_H
