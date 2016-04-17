/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "context.h"
#include "modulescope.h"

Context::Context()
{
	parent=NULL;
	currentValue=NULL;
	returnValue=NULL;
	currentScope=NULL;
}

Context::~Context()
{
	arguments.clear();
	parameters.clear();
}

void Context::setParent(Context* value)
{
	parent=value;
}

void Context::setCurrentScope(Scope* value)
{
	currentScope=value;
}

Scope* Context::getCurrentScope()
{
	return currentScope;
}

void Context::setReturnValue(Value* value)
{
	returnValue=value;
}

Value* Context::getReturnValue()
{
	return returnValue;
}

Value* Context::getCurrentValue()
{
	return currentValue;
}

void Context::setCurrentValue(Value* value)
{
	currentValue=value;
}

QString Context::getCurrentName()
{
	return currentName;
}

void Context::setCurrentName(QString value)
{
	currentName=value;
}

bool Context::addVariable(Value* v)
{
	QString name=v->getName();
	if(!variables.contains(name)) {
		variables.insert(name,v);
		return true;
	}
	return false;
}

void Context::setVariable(Value* v)
{
	variables.insert(v->getName(),v);
}

Value* Context::lookupVariable(QString name,Variable::Storage_e& c,Layout* l)
{
	if(variables.contains(name)) {
		if(l->inScope(currentScope)) {
			Value* v=variables.value(name);
			c=v->getStorage();
			return v;
		}
	} else if(parent) {
		return parent->lookupVariable(name,c,l);
	}

	Value* v=Value::undefined();
	v->setStorage(c);
	return v;

}

/* Lookup child doesn't currently
 * check the lexical scope of the
 * parent */
Node* Context::lookupChild(int index)
{
	QList<Node*> children=getInputNodes();
	if(index>=0&&index<children.length())
		return children.at(index);
	if(parent)
		return parent->lookupChild(index);

	return NULL;
}

/* Lookup children doesn't currently
 * check the lexical scope of the
 * parent */
QList<Node*> Context::lookupChildren()
{
	QList<Node*> children=getInputNodes();
	if(children.length()>0)
		return children;
	if(parent)
		return parent->lookupChildren();

	return QList<Node*>();
}

void Context::setVariablesFromArguments()
{
	for(int i=0; i<parameters.size(); i++) {
		Value* val=parameters.at(i);
		bool found=false;
		QString paramName=val->getName();
		foreach(Value* arg,arguments) {
			QString argName=arg->getName();
			if(arg->isDefined()&&argName==paramName) {
				val=arg;
				found=true;
				break;
			}
		}
		if(!found&&i<arguments.size()) {
			Value* arg=arguments.at(i);
			QString argName=arg->getName();
			if(arg->isDefined()&&argName.isEmpty()) {
				val=arg;
			}
		}

		variables.insert(paramName,val);
	}
}

QList<Value*> Context::getArguments()
{
	return arguments;
}

void Context::addArgument(Value* value)
{
	arguments.append(value);
}

void Context::clearArguments()
{
	arguments.clear();
}

Value* Context::getArgument(int index, QString name)
{
	//TODO make matchLast work for name ending with any digit
	bool matchLast = name.endsWith('1') || name.endsWith('2');

	return matchArgumentIndex(true,matchLast,index,name);
}

Value* Context::getArgumentDeprecatedModule(int index, QString deprecated, QString module, Reporter* r)
{
	Value* v = matchArgumentIndex(false,false,index,deprecated);
	if(v)
		r->reportWarning(tr("'%1' parameter is deprecated use %2 instead").arg(deprecated).arg(module));
	return v;
}

Value* Context::getArgumentDeprecated(int index, QString name, QString deprecated, Reporter* r)
{
	Value* v = matchArgumentIndex(true,false,index,name);
	if(!v) {
		v = matchArgumentIndex(false,false,index,deprecated);
		if(v)
			r->reportWarning(tr("'%1' parameter is deprecated use '%2' instead").arg(deprecated).arg(name));
	}

	return v;
}

void Context::clearParameters()
{
	parameters.clear();
}

void Context::addParameter(Value* value)
{
	parameters.append(value);
}

void Context::setInputNodes(QList<Node*> value)
{
	inputNodes=value;
}

QList<Node*> Context::getInputNodes()
{
	return inputNodes;
}

void Context::setCurrentNodes(QList<Node*> value)
{
	currentNodes=value;
}

QList<Node*> Context::getCurrentNodes()
{
	return currentNodes;
}

void Context::addCurrentNode(Node* value)
{
	currentNodes.append(value);
}

Value* Context::getArgumentSpecial(QString name)
{
	Value* v=matchArgument(false,false,name);
	if(v && v->getStorage()==Variable::Special)
		return v;

	return NULL;
}

Value* Context::matchArgumentIndex(bool allowChar,bool matchLast, int index, QString name)
{
	if(index >= arguments.size())
		return NULL;

	Value* arg = arguments.at(index);
	QString argName = arg->getName();
	if(argName.isEmpty() || match(allowChar,matchLast,argName,name))
		return arg;

	return matchArgument(allowChar,matchLast,name);
}

Value* Context::matchArgument(bool allowChar,bool matchLast, QString name)
{
	foreach(Value* namedArg,arguments) {
		QString namedArgName = namedArg->getName();
		if(match(allowChar,matchLast,namedArgName,name))
			return namedArg;
	}

	return NULL;
}

bool Context::match(bool allowChar,bool matchLast, QString a,QString n)
{
	if(allowChar) {
		if(matchLast&&a.length()==2)
			return a.left(1)==n.left(1) && a.right(1)==n.right(1);
		if(!matchLast&&a.length()==1)
			return a.left(1)==n.left(1);
	}
	return a==n;
}
