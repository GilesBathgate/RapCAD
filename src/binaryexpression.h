#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H

#include "expression.h"

class BinaryExpression : public Expression
{
public:
    Expression* left;
    Expression* right;
    BinaryExpression();
};

#endif // BINARYEXPRESSION_H
