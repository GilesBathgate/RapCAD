#include "ternaryexpression.h"

TernaryExpression::TernaryExpression()
{
    condition=NULL;
    trueExpression=NULL;
    falseExpression=NULL;
}

TernaryExpression::~TernaryExpression()
{
   delete condition;
   delete trueExpression;
   delete falseExpression;
}

void TernaryExpression::setCondition(Expression* exp)
{
    this->condition = exp;
}

void TernaryExpression::setTrueExpression(Expression* exp)
{
    this->trueExpression = exp;
}

void TernaryExpression::setFalseExpression(Expression* exp)
{
   this->falseExpression = exp;
}

QString TernaryExpression::toString()
{
    QString result;
    result.append("Ternary: ");
    if(this->condition)
        result.append(this->condition->toString());

    if(this->trueExpression)
        result.append(this->trueExpression->toString());

    if(this->falseExpression)
        result.append(this->falseExpression->toString());

    return result;
}
