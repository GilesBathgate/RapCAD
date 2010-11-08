#include "binaryexpression.h"

BinaryExpression::BinaryExpression()
{
}

QString BinaryExpression::toString()
{
    QString result;
    result.append("Expression: ");
    result.append(this->left->toString());
    result.append(this->right->toString());
    return result;
}
