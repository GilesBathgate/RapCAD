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
    void setTrueExpression(Expression*);
    void setFalseExpression(Expression*);
    QString toString();
private:
    Expression* condition;
    Expression* trueExpression;
    Expression* falseExpression;
};

#endif // TERNARYEXPRESSION_H
