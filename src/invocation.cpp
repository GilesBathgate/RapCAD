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

void Invocation::setNamespace(QString name)
{
	this->nameSpace = name;
}

QString Invocation::getNamespace()
{
	return this->nameSpace;
}

void Invocation::setArguments(QVector<Argument*> args)
{
	this->arguments = args;
}

QVector<Argument*> Invocation::getArguments()
{
	return this->arguments;
}

void Invocation::accept(Visitor& v)
{
	v.visit(this);
}
