#include "tree.h"
#include "utilities.h"
#include "diff.h"
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

static int _simplifyEquation(Node * actualNode, Node * parentNode)
{
    if ( !actualNode ) return 0;
    else if (actualNode->typeOfData != OP) return 0;
    else if ((actualNode->typeOfData == OP && IS_NUM(actualNode->left) && IS_NUM(actualNode->right)) || (IS_EMP_NORM(actualNode))) {
        doOperation(actualNode);
        printf("New data: %lg\n", actualNode->data);

        free(actualNode->left);
        free(actualNode->right);
        actualNode->left  = NULL;
        actualNode->right = NULL;

        return 1;
    }
    else if ((IS_EMP(actualNode->right)) || (IS_EMP(actualNode->right))) {
        actualNode->typeOfData = EMP;
        free(actualNode->left);
        free(actualNode->right);

        return 1;
    }

    return _simplifyEquation(actualNode->right, actualNode) + _simplifyEquation(actualNode->left, actualNode);
}

int simplifyEquation(Node * root)
{
    return _simplifyEquation(root, NULL);
}

int changeNodes(Node * parentNode, Node * prevNode, Node * newNode)
{
    if ( !parentNode ) {
        Node * temp = prevNode;
        prevNode = newNode;
        free(temp);
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