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
	this->expression = exp;
}

Expression* ReturnStatement::getExpression() const
{
	return this->expression;
}

void ReturnStatement::accept(TreeVisitor& v)
{
	v.visit(this);
}
