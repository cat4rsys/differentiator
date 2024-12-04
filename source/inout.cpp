#include "tree.h"
#include "inout.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cctype>

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

int identifyOperation(char * buf)
{
    if (!strcmp(buf, "+"))    return ADD;
    if (!strcmp(buf, "-"))    return SUB;
    if (!strcmp(buf, "*"))    return MUL;
    if (!strcmp(buf, "/"))    return DIV;
    if (!strcmp(buf, "^"))    return DEG;
    if (!strcmp(buf, "log"))  return LOG;
    if (!strcmp(buf, "sin"))  return SIN;
    if (!strcmp(buf, "cos"))  return COS;
    if (!strcmp(buf, "tg"))   return TG;
    if (!strcmp(buf, "ctg"))  return CTG;
    if (!strcmp(buf, "asin")) return ASIN;
    if (!strcmp(buf, "acos")) return ACOS;
    if (!strcmp(buf, "atg"))  return ATG;
    if (!strcmp(buf, "actg")) return ACTG;
    if (!strcmp(buf, "sh"))   return SH;
    if (!strcmp(buf, "ch"))   return CH;
    if (!strcmp(buf, "th"))   return TH;

    return UNK;
}

Node * readInfix(FILE * inputFile, Tree * tree)
{
    int c = 0;
    Node * node = (Node *)calloc(1, sizeof(Node));

    while ( (c = fgetc(inputFile)) != '(') printf("%c\n", c);
    bool isLeaf = (getType(inputFile) != UNK);

    if (!isLeaf) node->left  = readInfix(inputFile, tree);

    node->typeOfData = getType(inputFile);
    node->data       = getValue(inputFile, node->typeOfData, tree);

    if (!isLeaf) node->right = readInfix(inputFile, tree);

    while ( (c = fgetc(inputFile)) != ')');

    return node;
}

TypeOfValue getType(FILE * inputFile)
{
    TypeOfValue type = UNK;

    int firstSymbol  = fgetc(inputFile);
    int secondSymbol = fgetc(inputFile);

    if (isdigit(firstSymbol)) type = NUM;
    else if (isalpha(firstSymbol) && secondSymbol == ')') type = VAR;
    else if (firstSymbol == ')') type = EMP;
    else if (firstSymbol == '(') type = UNK;
    else type = OP;

    if (secondSymbol != EOF) ungetc(secondSymbol, inputFile);
    ungetc(firstSymbol,  inputFile);

    return type;
}

Value_t getValue(FILE * inputFile, TypeOfValue type, Tree * tree)
{
    Value_t value = 0;

    switch (type) {
        case VAR: 
        {
            value = fgetc(inputFile);
            int searchResult = searchAtVarTable(tree, (char)value);
            value = (searchResult == -1) ? addToVarTable(tree, (char)value) : searchResult;

            break;
        }
        case NUM:
        {
            fscanf(inputFile, "%lg", &value);

            break;
        }
        case OP:
        {
            char buf[5] = {};

            fscanf(inputFile, "%[^(]", buf);
            value = identifyOperation(buf);
            break;
        }
        case EMP:
        {
            value = 0;
            break;
        }
        case UNK: break;
        default: return value;
    }

    return value;
}

const char * s = "25*10*(3*(25-10*2)+1)$";
int p = 0;

void syntaxError()
{
    fprintf(stderr, "Syntax error at %d symbol.", p);
    abort();
}

int getG()
{
    int val = getE();
    if (s[p] != '$')
        syntaxError();
    p++;
    return val;
}

int getN()
{
    int val = 0;
    while ('0' <= s[p] && s[p] <= '9')
    {
        val = val * 10 + s[p] - '0';
        p++;
    }

    return val;
}

int getE()
{
    int val = getT();
    while (s[p] == '+' || s[p] == '-')
    {
        int op = s[p];
        p++;
        int val2 = getT();
        if (op == '+')      val += val2;
        else if (op == '-') val -= val2;
    }

    return val;
}

int getT()
{
    int val = getP();
    while (s[p] == '*' || s[p] == '/')
    {
        int op = s[p];
        p++;
        int val2 = getP();
        if (op == '*')      val *= val2;
        else if (op == '/') val /= val2;
    }

    return val;
}

int getP()
{
    if (s[p] == '(') {
        p++;
        int val = getE();
        if (s[p] != ')') syntaxError();
        p++;
        return val;
    }
    else {
        int val = getN();
        return val;
    }
}

Node * getGrammar(Grammar *g)
{
    Node * val = getEquation(g);

    if (g->str[g->position] != '$')
        syntaxError();

    p++;

    return val;
}

Node * getNumber(Grammar * g)
{
    double num = 0;

    bool ifDot        = 0;
    int numsAfterDot  = 0;
    char symbol       = g->str[g->position];

    while ( isdigit(symbol) || symbol == '.' || symbol == ',' ) {
        symbol = g->str[g->position];

        if ( isdigit(symbol) ) {
            num = num * 10 + symbol - '0';
            if ( ifDot == 1 ) {
                numsAfterDot++;
            }
        }

        if ( symbol == '.' || symbol == ',' ) {
            if (ifDot == 1) {
                g->error = TWO_OR_MORE_FRACTIONAL;
                return NULL;
            }
            ifDot = 1;
        }

        p++;
    }

    for ( ; numsAfterDot > 0; numsAfterDot-- ) {
        num /= 10.0;
    }

    return _NUM(num);
}