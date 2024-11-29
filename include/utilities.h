#ifndef H_UTILITIES
#define H_UTILITIES

/**
 * \brief Result of comparing two doubles
 */
enum StatusDouble
{
    GREATER,                                     /**< First double is greater than second */
    EQUALS,                                      /**< First double equals second */
    LESS                                         /**< First double is less than second */
};

/**
 * \brief Assert, that doesn't stop program. Just gives warning about failed aseertion
 *
 * \param expression - expression for assertion
 */
#define CUSTOM_ASSERT(expression) {                                         \
    if( !(expression) ) {                                                   \
        setColor(RED);                                                      \
        printf("Assertion failed: file %s, line %d\n", __FILE__, __LINE__); \
        setColor(STANDART);                                                 \
    }                                                                       \
}

/**
 * \brief Compares two doubles
 *
 * \param number1 - first number
 * \param number2 - second number
 *
 * \return status of comparing of two doubles
 */
StatusDouble compareDouble(double number1, double number2);

/**
 * \brief Skips input if last entered symbol not "\n"
 *
 * \param symbol - last entered symbol
 */
void skipInput(int symbol);

#endif // H_UTILITUS
