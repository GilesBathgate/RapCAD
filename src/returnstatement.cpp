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

Expression* ReturnStatement::getExpression()
{
    return this->expression;
}

void ReturnStatement::accept(Visitor *v)
{
    v->visit(this);
}
