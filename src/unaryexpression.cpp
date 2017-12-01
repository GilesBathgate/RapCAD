#include "unaryexpression.h"

UnaryExpression::UnaryExpression()
{
	expression=nullptr;
}

UnaryExpression::~UnaryExpression()
{
}

void UnaryExpression::setExpression(Expression* exp)
{
	expression = exp;
}

Expression* UnaryExpression::getExpression() const
{
	return expression;
}

void UnaryExpression::accept(TreeVisitor& v)
{
	v.visit(this);
}
