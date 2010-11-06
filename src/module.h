#ifndef MODULE_H
#define MODULE_H

#include <qstring.h>
#include "parameter.h"

class Module
{
public:
    QString name;
    QVector<Parameter*>* parameters;
    Module();
};

#endif // MODULE_H
