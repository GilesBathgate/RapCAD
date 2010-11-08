#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H

#include "expression.h"

class BinaryExpression : public Expression
{
public:
    Expression* left;
    Expression* right;
    Operator_e op;

    QString toString();
    BinaryExpression();
};

#endif // BINARYEXPRESSION_H
