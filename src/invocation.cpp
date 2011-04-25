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

QString Invocation::getName() const
{
	return this->name;
}

void Invocation::setNamespace(QString name)
{
	this->nameSpace = name;
}

QString Invocation::getNamespace() const
{
	return this->nameSpace;
}

void Invocation::setArguments(QList<Argument*> args)
{
	this->arguments = args;
}

QList<Argument*> Invocation::getArguments() const
{
	return this->arguments;
}

void Invocation::accept(TreeVisitor& v)
{
	v.visit(this);
}
