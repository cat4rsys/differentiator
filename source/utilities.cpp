#include <stdio.h>
#include <math.h>
#include "utilities.h"

void skipInput(int symbol)
{
    if (symbol != '\n') {
        while (getchar() != '\n') {
            continue;
        }
    }
}

StatusDouble compareDouble(double number1, double number2)
{
    const double eps = 1e-8;                    /**< error of double */

    if ( fabs(number1 - number2) < eps ) return EQUALS;
    if (     (number1 - number2) > eps ) return GREATER;

    return LESS;
}
