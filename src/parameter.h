#ifndef PARAMETER_H
#define PARAMETER_H
#include <QString>
#include "expression.h"

class Parameter
{
public:
    QString name;
    Expression* expression;

    QString toString();
    Parameter();
};

#endif // PARAMETER_H
