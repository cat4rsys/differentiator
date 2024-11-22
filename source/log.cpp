#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "tree.h"

void logDump(const char * func, const int line, const char * file, Tree * tree)
{
    char dotFile[sizeOfBuf] = {};
    char pngFile[sizeOfBuf] = {};

    snprintf(dotFile, sizeOfBuf, "log/%d.dot", tree->numberOfOperation);
    snprintf(pngFile, sizeOfBuf, "log/%d.png", tree->numberOfOperation);

    tree->numberOfOperation++;

    fprintf(tree->logFile, "Dump called from function %s (%s:%d)\n\n", func, file, line);
    dump(tree, dotFile);

    char cmd[sizeOfBuf] = "dot -Tpng -o ";
    strcat(cmd, pngFile);
    strcat(cmd, " ");
    strcat(cmd, dotFile);

    system(cmd);

    fprintf(tree->logFile, "<img src=\"%s\">\n", pngFile + 4);
}

void printInfoAboutNode(Node * actualNode, FILE * dotFile)
{
    if (!actualNode) return;

    fprintf(dotFile, "ptr%p [shape=record,label=\"{ ptr:%p | type:%d | data:%lg }\",", actualNode, actualNode, actualNode->typeOfData, actualNode->data);
    printColorToLog(actualNode->typeOfData, dotFile);

    if ( actualNode->left ) {
        printInfoAboutNode(actualNode->left, dotFile);
        fprintf(dotFile, "ptr%p -> ptr%p;\n\t", actualNode, actualNode->left);
    }
    if ( actualNode->right ) {
        printInfoAboutNode(actualNode->right, dotFile);
        fprintf(dotFile, "ptr%p -> ptr%p;\n\t", actualNode, actualNode->right);
    }

    return;
}

void printColorToLog(TypeOfValue type, FILE * dotFile) {
    switch (type) {
        case VAR:
        {
            fprintf(dotFile, "color=\"darkorchid1\"]\n\t");
            break;
        }
        case NUM:
        {
            fprintf(dotFile, "color=\"springgreen\"]\n\t");
            break;
        }
        case OP:
        {
            fprintf(dotFile, "color=\"aqua\"]\n\t");
            break;
        }
        default:
        {
            printf("UNKNOWN ERR\n");
        }
    }

    return;
}

void dump(Tree * tree, char * pathToDot)
{
    FILE * dotFile = fopen(pathToDot, "w");
    if (!dotFile) return;

    fprintf(dotFile, "digraph G{\n\trankdir=TB;\n\tnode[shape=rect,style=rounded];\n\t");

    printInfoAboutNode(tree->firstNode, dotFile);

    fprintf(dotFile, "}\n");

    fclose(dotFile);
}

