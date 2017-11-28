#include "ternaryexpression.h"

TernaryExpression::TernaryExpression()
{
	condition=nullptr;
	trueExpression=nullptr;
	falseExpression=nullptr;
}

TernaryExpression::~TernaryExpression()
{
	delete condition;
	delete trueExpression;
	delete falseExpression;
}

void TernaryExpression::setCondition(Expression* exp)
{
	condition = exp;
}

Expression* TernaryExpression::getCondition() const
{
	return condition;
}

void TernaryExpression::setTrueExpression(Expression* exp)
{
	trueExpression = exp;
}

Expression* TernaryExpression::getTrueExpression() const
{
	return trueExpression;
}

void TernaryExpression::setFalseExpression(Expression* exp)
{
	falseExpression = exp;
}

Expression* TernaryExpression::getFalseExpression() const
{
	return falseExpression;
}

void TernaryExpression::accept(TreeVisitor& v)
{
	v.visit(this);
}
