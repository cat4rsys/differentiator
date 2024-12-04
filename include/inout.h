#ifndef INOUT_H
#define INOUT_H

#include "tree.h"
#include <stdio.h>

enum GrammarError
{
    TWO_OR_MORE_FRACTIONAL = 1 << 1
};

typedef struct Grammar
{
    char * str;
    size_t position;
    VarElement * tableOfVar;

    GrammarError error;
} Grammar;

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

Node * getGrammar(Grammar * g);

Node * getNumber(Grammar * g);

Node * getVar(Grammar * g);

Node * getFunction(Grammar * g);

Node * getElementary(Grammar * g);

Node * getDegree(Grammar * g);

Node * getMult(Grammar * g);

Node * getAdd(Grammar * g);

Node * getEquation(Grammar * g);

#endif // INOUT_H
