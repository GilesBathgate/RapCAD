#include "returnstatement.h"

ReturnStatement::ReturnStatement()
{
    expression=NULL;
}

ReturnStatement::~ReturnStatement()
{
    delete expression;
}

void ReturnStatement::setExpression(Expression* exp)
{
    this->expression = exp;
}

QString ReturnStatement::toString()
{
    QString result;
    result.append("Return: ");
    if(this->expression)
        result.append(this->expression->toString());
    return result;
}
