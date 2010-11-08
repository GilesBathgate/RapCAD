#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>

class Expression
{
public:
    enum Operator_e
    {
	Add,
	Subtract,
	Multiply,
	Divide,
	Modulus
    };

    virtual QString toString();
    Expression();
};

#endif // EXPRESSION_H
