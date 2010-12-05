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
    Expression* getExpression();
    void accept(Visitor *v);
private:
    Expression* expression;
};

#endif // UNARYEXPRESSION_H
