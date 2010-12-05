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

QString Invocation::getName()
{
    return this->name;
}

void Invocation::setArguments(QVector<Argument*> args)
{
    this->arguments = args;
}

QVector<Argument*> Invocation::getArguments()
{
    return this->arguments;
}

void Invocation::accept(Visitor *v)
{
    v->visit(this);
}
