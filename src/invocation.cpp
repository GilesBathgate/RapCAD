#include "invocation.h"

Invocation::Invocation()
{
}

Invocation::~Invocation()
{
}

void Invocation::setName(QString n)
{
	name = n;
}

QString Invocation::getName() const
{
	return name;
}

void Invocation::setNamespace(QString name)
{
	nameSpace = name;
}

QString Invocation::getNamespace() const
{
	return nameSpace;
}

void Invocation::setArguments(QList<Argument*> args)
{
	arguments = args;
}

QList<Argument*> Invocation::getArguments() const
{
	return arguments;
}

void Invocation::accept(TreeVisitor& v)
{
	v.visit(this);
}
