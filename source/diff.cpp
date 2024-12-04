#include "tree.h"
#include "utilities.h"
#include "diff.h"
#include "log.h"
#include <math.h>
#include <cstdio>
#include <stdlib.h>

Node * diff(Tree * tree, Node * actualNode, char var)
{
    printf("%d\n", actualNode->typeOfData);
    if (actualNode->typeOfData == NUM) {
        return _NUM(0);
    }
    else if (actualNode->typeOfData == EMP) {
        return actualNode;
    }
    else if (actualNode->typeOfData == VAR) {
        if (tree->tableOfVar[(int)actualNode->data].symbol == var) {
            return _NUM(1);
        }
        return _NUM(0);
    }
    else if (actualNode->typeOfData == OP) {
        switch ((int)actualNode->data) {
            // TODO DIFF_(left)
            // TODO ADD_, SUB_

            case ADD: return _ADD(diff(tree, actualNode->left, var), diff(tree, actualNode->right, var));
            case SUB: return _SUB(diff(tree, actualNode->left, var), diff(tree, actualNode->right, var));
            case MUL: return _ADD(_MUL(diff(tree, actualNode->left, var), clone(actualNode->right)),
                                  _MUL(diff(tree, actualNode->right, var), clone(actualNode->left)));
            case DIV: return _DIV(_SUB(_MUL(diff(tree, actualNode->left, var), clone(actualNode->right)),
                                       _MUL(clone(actualNode->left), diff(tree, actualNode->right, var))),
                                  _DEG(clone(actualNode->left), _NUM(2)));
            case DEG:
            {
                if (actualNode->right->typeOfData == NUM) return _MUL(_MUL(_NUM(actualNode->right->data), _DEG(clone(actualNode->left),
                                                                 _NUM(actualNode->right->data - 1))), diff(tree, actualNode->left, var));
                if (actualNode->left->typeOfData  == NUM) return _MUL(_MUL(clone(actualNode), _LOG(_NUM(e), _NUM(actualNode->left->data))),
                                                                 diff(tree, actualNode->right, var));
                
                return diff(tree, _DEG(_NUM(e), _MUL(clone(actualNode->right), _LOG(_NUM(e), clone(actualNode->left)))), var);
            }
            case SIN: return _MUL(_COS(clone(actualNode->right)), diff(tree, actualNode->right, var));
            case COS: return _MUL(_NUM(-1), _MUL(_SIN(clone(actualNode->right)), diff(tree, actualNode->right, var)));
            case TG:  return _DIV(diff(tree, actualNode->right, var), _DEG(_COS(clone(actualNode->right)), _NUM(2)));
            case CTG: return _MUL(_NUM(-1), _DIV(diff(tree, actualNode->right, var), _DEG(_SIN(clone(actualNode->right)), _NUM(2))));
            default:
            {
                fprintf(stderr, "UNKNOWN ERROR\n");
                return NULL;
            }
        }
    }

    fprintf(stderr, "UNKNOWN ERROR\n");
    return NULL;
}

Node * clone(Node * actualNode)
{
    if (actualNode == NULL) return NULL;

    Node * copiedNode = (Node *)malloc(sizeof(Node));
    
    *copiedNode = (Node){actualNode->typeOfData, actualNode->data, 
                        clone(actualNode->left), clone(actualNode->right)};

    return copiedNode;
}

static int _simplifyEquation(Node * actualNode, Node * parentNode) // TODO underscore
{
    if ( !actualNode ) return 0;
    printf("TRYING TO SIMPLIFY:\n");
    printf("node: %p, type %u, data %lg\n", actualNode, actualNode->typeOfData, actualNode->data);
    if ( parentNode) printf("parent node: %p, type %u, data %lg\n", parentNode, parentNode->typeOfData, parentNode->data);
    if (actualNode->left) printf("left node: %p, type %u, data %lg\n", actualNode->left, actualNode->left->typeOfData, actualNode->left->data);
    if (actualNode->right) printf("right node: %p, type %u, data %lg\n", actualNode->right, actualNode->right->typeOfData, actualNode->right->data);
    if (actualNode->typeOfData != OP) return 0;
    else if ((actualNode->typeOfData == OP && IS_NUM(actualNode->left) && IS_NUM(actualNode->right)) || (IS_EMP_NORM(actualNode))) {
        doOperation(actualNode);
        printf("New data: %lg\n", actualNode->data);

        freeLeaves(actualNode);

        return 1;
    }
    /*else if ((IS_EMP(actualNode->right)) || (IS_EMP(actualNode->right))) { // TODO IS_EMPTY()
        actualNode->typeOfData = EMP;
        freeLeaves(actualNode);

        return 1;
    }*/
    else if (IS_ZERO(actualNode->left)) {
        switch ((int)actualNode->data) { // TODO use union
            case MUL:
            {
                actualNode->data = 0;
                actualNode->typeOfData = NUM;
                freeLeaves(actualNode);

                return 1;
            }
            case ADD:
            {
                if (changeNodes(parentNode, actualNode, actualNode->right)) {
                    free(actualNode->left);
                    free(actualNode);
                    return 1;
                }
                return 1;
            }
            case SUB:
            {
                actualNode->data = MUL;
                actualNode->left->data = -1;

                return 1;
            }
            case DEG:
            {
                actualNode->data = 0;
                actualNode->typeOfData = NUM;
                freeLeaves(actualNode);

                return 1;
            }
            case DIV:
            {
                actualNode->data = 0;
                actualNode->typeOfData = NUM;
                freeLeaves(actualNode);

                return 1;
            }
            default: break;
        }
    }
    else if (IS_ZERO(actualNode->right)) {
        switch ((int)actualNode->data) {
            case MUL:
            {
                actualNode->data = 0;
                actualNode->typeOfData = NUM;
                freeLeaves(actualNode);

                return 1;
            }
            case ADD: case SUB:
            {
                if (changeNodes(parentNode, actualNode, actualNode->left)) {
                    free(actualNode->right);
                    free(actualNode);
                    return 1;
                }
                return 1;
            }
            case DEG:
            {
                actualNode->data = 1;
                actualNode->typeOfData = NUM;
                freeLeaves(actualNode);

                return 1;
            }
            case DIV:
            {
                actualNode->typeOfData = EMP;
                freeLeaves(actualNode);

                return 1;
            }
            default: break;
        }
    }
    else if (IS_ONE(actualNode->left)) {
        switch ((int)actualNode->data) {
            case MUL:
            {
                if (changeNodes(parentNode, actualNode, actualNode->right)) {
                    free(actualNode->left);
                    free(actualNode);
                    return 1;
                }

                return 1;
            }
            case DEG:
            {
                actualNode->data = 1;
                actualNode->typeOfData = NUM;
                freeLeaves(actualNode);

                return 1;
            }
            default: break;
        }
    }
    else if (IS_ONE(actualNode->right)) {
        switch ((int)actualNode->data) {
            case MUL: case DEG: case DIV:
            {
                if (changeNodes(parentNode, actualNode, actualNode->left)) {
                    free(actualNode->right);
                    free(actualNode);
                    return 1;
                }

                return 1;
            }
            default: break;
        }
    }

    return _simplifyEquation(actualNode->right, actualNode) + _simplifyEquation(actualNode->left, actualNode);
}

void freeLeaves(Node * actualNode)
{
    free(actualNode->left);
    free(actualNode->right);
    actualNode->left  = NULL;
    actualNode->right = NULL;
}

int simplifyEquation(Node * root)
{
    dump(root, "aaa.dot", "aaa.png");
    char buf[2] = {};
    fgets(buf, 2, stdin);
    return _simplifyEquation(root, NULL);
}

int changeNodes(Node * parentNode, Node * prevNode, Node * newNode)
{
    if ( !parentNode ) {
        if (prevNode->left == newNode) free(prevNode->right);
        else                           free(prevNode->left);

        *prevNode = *newNode;
        free(newNode);
        return 0;
    }

    if (parentNode->left == prevNode) {
        parentNode->left = newNode;
        return 1;
    }
    if (parentNode->right == prevNode) {
        parentNode->right = newNode;
        return 1;
    }

    return 0;
}

void doOperation(Node * operationNode)
{
    printf("%lg, %lg, %lg\n", operationNode->data, operationNode->left->data, operationNode->right->data);
    switch ((int) operationNode->data) {
        case ADD:
        {
            operationNode->typeOfData = NUM;
            operationNode->data = operationNode->left->data + operationNode->right->data;
            return;
        }
        case SUB:
        {
            operationNode->typeOfData = NUM;
            operationNode->data = operationNode->left->data - operationNode->right->data;
            return;
        }
        case MUL:
        {
            operationNode->typeOfData = NUM;
            operationNode->data = operationNode->left->data * operationNode->right->data;
            return;
        }
        case DIV:
        {
            if (compareDouble(operationNode->right->data, 0) != EQUALS) {
                operationNode->typeOfData = NUM;
                operationNode->data = operationNode->left->data / operationNode->right->data;
            }
            else {
                operationNode->typeOfData = EMP;
            }

            return;
        }
        case DEG:
        {
            if (isnan(operationNode->data = pow(operationNode->left->data, operationNode->right->data))) {
                operationNode->typeOfData = EMP;
                return;
            }

            operationNode->typeOfData = NUM;
            return;
        }
        case LOG:
        {
            if (isnan(operationNode->data = log(operationNode->right->data) / log(operationNode->left->data))) {
                operationNode->typeOfData = EMP;
                return;
            }

            operationNode->typeOfData = NUM;
            return;
        }
        case SIN:
        {
            operationNode->typeOfData = NUM;
            operationNode->data = sin(operationNode->right->data);
            return;
        }
        case COS:
        {
            operationNode->typeOfData = NUM;
            operationNode->data = cos(operationNode->right->data);
            return;
        }
        case TG:
        {
            if (isnan(operationNode->data = tan(operationNode->left->data))) {
                operationNode->typeOfData = EMP;
                return;
            }

            operationNode->typeOfData = NUM;
            return;
        }
        case CTG:
        {
            if (isnan(operationNode->data = 1 / tan(operationNode->left->data))) {
                operationNode->typeOfData = EMP;
                return;
            }

            operationNode->typeOfData = NUM;
            return;
        }
        case ASIN:
        {
            if (isnan(operationNode->data = asin(operationNode->left->data))) {
                operationNode->typeOfData = EMP;
                return;
            }

            operationNode->typeOfData = NUM;
            return;
        }
        case ACOS:
        {
            if (isnan(operationNode->data = acos(operationNode->left->data))) {
                operationNode->typeOfData = EMP;
                return;
            }

            operationNode->typeOfData = NUM;
            return;
        }
        case ATG:
        {
            if (isnan(operationNode->data = atan(operationNode->left->data))) {
                operationNode->typeOfData = EMP;
                return;
            }

            operationNode->typeOfData = NUM;
            return;
        }
        case CH:
        {
            operationNode->typeOfData = NUM;
            operationNode->data = cosh(operationNode->right->data);
            return;
        }
        case SH:
        {
            operationNode->typeOfData = NUM;
            operationNode->data = sinh(operationNode->right->data);
            return;
        }
        case TH:
        {
            if (isnan(operationNode->data = tanh(operationNode->left->data))) {
                operationNode->typeOfData = EMP;
                return;
            }

            operationNode->typeOfData = NUM;
            return;
        }
        default:
        {
            fprintf(stderr, "Can't do operation at node %p\n", operationNode);
        }
    }
}