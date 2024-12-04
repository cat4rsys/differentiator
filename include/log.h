#ifndef LOGH
#define LOGH

#include <stdio.h>
#include "tree.h"

#define DO_LOG_DUMP(line, file, tree) {                           \
	logDump(__func__, line, file, tree);                          \
}

const int sizeOfBuf = 100;

void logDump(const char * func, const int line, const char * file, Tree * tree);

void printColorToLog(TypeOfValue type, FILE * dotFile);

void printInfoAboutNode(Node * actualNode, FILE * dotFile);

void dump(Node * firstNode, const char * pathToDot, const char * pathToPng);

#endif // LOGH