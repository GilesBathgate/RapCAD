#include "invocation.h"

Invocation::Invocation()
{
}

Invocation::~Invocation()
{
}

void Invocation::setName(QString name)
{
    this->name = name;
}

void Invocation::setArguments(QVector<Argument*> args)
{
    this->arguments = args;
}

QString Invocation::toString()
{
    QString result;
    result.append("Invocation: ");
    result.append(name);
    result.append(" (");
    for(int i=0; i<this->arguments.size(); i++)
        result.append(this->arguments.at(i)->toString());
    result.append(")\n");
    return result;
}
