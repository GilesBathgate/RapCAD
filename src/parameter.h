#ifndef PARAMETER_H
#define PARAMETER_H
#include <QString>
#include "expression.h"

class Parameter
{
public:
    QString name;
    Expression* expression;
    Parameter();
};

#endif // PARAMETER_H
