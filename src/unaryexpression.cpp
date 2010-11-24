#include "unaryexpression.h"

UnaryExpression::UnaryExpression()
{
}

UnaryExpression::~UnaryExpression()
{
}

void UnaryExpression::setExpression(Expression* exp)
{
    this->expression = exp;
}

QString UnaryExpression::toString()
{
    QString result;
    result.append("Unary: ");
    result.append(this->getOpString());
    result.append(" ");
    result.append(this->expression->toString());
    return result;
}
