#include "diff.h"
#include "log.h"
#include "tree.h"
#include "inout.h"
#include <cstdio>
#include <stdlib.h>

int main()
{
    Tree * tree = (Tree *)calloc(1, sizeof(Tree));

    DO_CREATE_TREE(tree, "log/logFile.htm");

    tree->firstNode = readInfix(fopen("penis.txt", "r"), tree);

    DO_LOG_DUMP(0, 0, tree);

    tree->firstNode = diff(tree, tree->firstNode, 'x');

    while(simplifyEquation(tree->firstNode));

    DO_LOG_DUMP(0, 0, tree);

    printInfix(fopen("penis.txt", "w"), tree->firstNode, tree);

    //printf("%d\n", getG());
}