#ifndef TERNARYEXPRESSION_H
#define TERNARYEXPRESSION_H

#include <QString>
#include "expression.h"

class TernaryExpression : public Expression
{
public:
	TernaryExpression();
	~TernaryExpression();
	void setCondition(Expression*);
	Expression* getCondition();
	void setTrueExpression(Expression*);
	Expression* getTrueExpression();
	void setFalseExpression(Expression*);
	Expression* getFalseExpression();
	void accept(TreeVisitor&);
private:
	Expression* condition;
	Expression* trueExpression;
	Expression* falseExpression;
};

#endif // TERNARYEXPRESSION_H
