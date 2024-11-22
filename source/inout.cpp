#include "tree.h"
#include "inout.h"
#include <stdio.h>

void printInfix(FILE * outputFile, Node * actualNode, Tree * tree)
{
    if ( !actualNode ) return;

    fprintf(outputFile, "(");

    printInfix(outputFile, actualNode->left, tree);
    printValue(outputFile, actualNode->data, actualNode->typeOfData, tree);
    printInfix(outputFile, actualNode->right, tree);

    fprintf(outputFile, ")");

    return;
}

void printPrefix(FILE * outputFile, Node * actualNode, Tree * tree)
{
    if ( !actualNode ) return;

    fprintf(outputFile, "(");

    printValue(outputFile, actualNode->data, actualNode->typeOfData, tree);
    printInfix(outputFile, actualNode->left, tree);
    printInfix(outputFile, actualNode->right, tree);

    fprintf(outputFile, ")");

    return;
}

/*void _printTex(FILE * outputFile, Node * actualNode, Tree * tree)
{
    if (actualNode->typeOfData == OP && (int)actualNode->data == DIV) {
        printf("\\frac{")
    }
}

void printTex(FILE * outputFile, Tree * tree)
{
    fprintf(outputFile, "$");

    _printTex(outputFile, tree->firstNode, tree);

    fprintf(outputFile, "$");
}*/

void printValue(FILE * outputFile, Value_t value, TypeOfValue type, Tree * tree)
{
    if (type == VAR)     fprintf(outputFile, "%c", tree->tableOfVar[(int)value].symbol);
    else if (type == OP) printOperation(outputFile, (char)value);
    else                 fprintf(outputFile, "%lg", value);
}

int printOperation(FILE * outputFile, char code) {
    switch (code) {
        case ADD:  return fprintf(outputFile, "%c", '+');
        case SUB:  return fprintf(outputFile, "%c", '-');
        case MUL:  return fprintf(outputFile, "%c", '*');
        case DIV:  return fprintf(outputFile, "%c", '/');
        case DEG:  return fprintf(outputFile, "%c", '^');
        case LOG:  return fprintf(outputFile, "%s", "log");
        case SIN:  return fprintf(outputFile, "%s", "sin");
        case COS:  return fprintf(outputFile, "%s", "cos");
        case TG:   return fprintf(outputFile, "%s", "tg");
        case ASIN: return fprintf(outputFile, "%s", "asin");
        case ACOS: return fprintf(outputFile, "%s", "acos");
        case ATG:  return fprintf(outputFile, "%s", "atg");
        case ACTG: return fprintf(outputFile, "%s", "actg");
        case TH:   return fprintf(outputFile, "%s", "th");
        case SH:   return fprintf(outputFile, "%s", "sh");
        case CH:   return fprintf(outputFile, "%s", "ch");
        case CTG:  return fprintf(outputFile, "%s", "ctg");
        default:   return -1;
    }
}