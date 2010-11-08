#ifndef MODULE_H
#define MODULE_H

#include <QString>
#include <QVector>
#include "parameter.h"

class Module
{
public:
    QString name;
    QVector<Parameter*>* parameters;
    QString toString();
    Module();
};

#endif // MODULE_H
