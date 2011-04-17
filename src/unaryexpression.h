#ifndef UNARYEXPRESSION_H
#define UNARYEXPRESSION_H

#include <QString>
#include "expression.h"

class UnaryExpression : public Expression
{
public:
	UnaryExpression();
	~UnaryExpression();

	void setExpression(Expression*);
	Expression* getExpression() const;
	void accept(TreeVisitor&);
private:
	Expression* expression;
};

#endif // UNARYEXPRESSION_H
