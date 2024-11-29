#ifndef INOUT_H
#define INOUT_H

#include "tree.h"
#include <stdio.h>

void printInfix(FILE * outputFile, Node * actualNode, Tree * tree);

void printPrefix(FILE * outputFile, Node * actualNode, Tree * tree);

void printValue(FILE * outputFile, Value_t value, TypeOfValue type, Tree * tree);

int printOperation(FILE * outputFile, char code);

TypeOfValue getType(FILE * inputFile);

Value_t getValue(FILE * inputFile, TypeOfValue type, Tree * tree);

Node * readInfix(FILE * inputFile, Tree * tree);

int identifyOperation(char * buf);

int getP();

int getG();

int getE();

int getT();

int getN();

#endif // INOUT_H
