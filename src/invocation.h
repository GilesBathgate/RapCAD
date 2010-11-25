#ifndef INVOCATION_H
#define INVOCATION_H

#include <QString>
#include <QVector>
#include "expression.h"
#include "argument.h"

class Invocation : public Expression
{
public:
    Invocation();
    ~Invocation();

    void setName(QString);
    void setArguments(QVector<Argument*>);
    QString toString();
private:
    QString name;
    QVector<Argument*> arguments;
};

#endif // INVOCATION_H
