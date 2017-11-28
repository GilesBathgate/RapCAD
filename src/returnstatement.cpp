#include "returnstatement.h"

ReturnStatement::ReturnStatement()
{
	expression=nullptr;
}

ReturnStatement::~ReturnStatement()
{
	delete expression;
}

void ReturnStatement::setExpression(Expression* exp)
{
	expression = exp;
}

Expression* ReturnStatement::getExpression() const
{
	return expression;
}

void ReturnStatement::accept(TreeVisitor& v)
{
	v.visit(this);
}
