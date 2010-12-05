#ifndef RETURNSTATEMENT_H
#define RETURNSTATEMENT_H

#include <QString>
#include "statement.h"
#include "expression.h"

class ReturnStatement : public Statement
{
public:
    ReturnStatement();
    ~ReturnStatement();
    void setExpression(Expression*);
    Expression* getExpression();
    void accept(Visitor *v);
private:
    Expression* expression;
};

#endif // RETURNSTATEMENT_H
