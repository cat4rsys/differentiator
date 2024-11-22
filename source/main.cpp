#include "log.h"
#include "tree.h"
#include "inout.h"
#include <cstdio>
#include <stdlib.h>

int main()
{
    Tree * tree = (Tree *)calloc(1, sizeof(Tree));

    DO_CREATE_TREE(tree, "log/logFile.htm");

    tree->firstNode = DIV(ADD(VAR(tree, 'x'), NUM(3)), SUB(VAR(tree, 'y'), NUM(7)));

    DO_LOG_DUMP(0, 0, tree);

    printPrefix(fopen("penis.txt", "w"), tree->firstNode, tree);
}