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

Expression* UnaryExpression::getExpression()
{
	return this->expression;
}

void UnaryExpression::accept(TreeVisitor& v)
{
	v.visit(this);
}
