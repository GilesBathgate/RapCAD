#ifndef LITERAL_H
#define LITERAL_H

#include "expression.h"

class Literal : public Expression
{
public:
    enum Type_e
    {
	Boolean,
	Number,
	String
    };

    Type_e Type;

    union Value_t
    {
    bool Boolean;
    double Number;
    char* String;
    };

    Value_t Value;

    Literal();
};

#endif // LITERAL_H
