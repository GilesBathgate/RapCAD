#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>
#include "expression.h"

class Variable : public Expression
{
public:
    QString name;
    Variable();
};

#endif // VARIABLE_H
