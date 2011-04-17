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

Expression* TernaryExpression::getCondition() const
{
	return this->condition;
}

void TernaryExpression::setTrueExpression(Expression* exp)
{
	this->trueExpression = exp;
}

Expression* TernaryExpression::getTrueExpression() const
{
	return this->trueExpression;
}

void TernaryExpression::setFalseExpression(Expression* exp)
{
	this->falseExpression = exp;
}

Expression* TernaryExpression::getFalseExpression() const
{
	return this->falseExpression;
}

void TernaryExpression::accept(TreeVisitor& v)
{
	v.visit(this);
}
