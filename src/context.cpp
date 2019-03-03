/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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

Context::Context() :
	parent(nullptr),
	currentValue(nullptr),
	returnValue(nullptr),
	currentScope(nullptr)
{
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

Scope* Context::getCurrentScope() const
{
	return currentScope;
}

void Context::setReturnValue(Value* value)
{
	returnValue=value;
}

Value* Context::getReturnValue() const
{
	return returnValue;
}

Value* Context::getCurrentValue() const
{
	return currentValue;
}

void Context::setCurrentValue(Value* value)
{
	currentValue=value;
}

QString Context::getCurrentName() const
{
	return currentName;
}

void Context::setCurrentName(const QString& value)
{
	currentName=value;
}

bool Context::addVariable(const QString& name,Value* v)
{
	if(!variables.contains(name)) {
		setVariable(name,v);
		return true;
	}
	return false;
}

void Context::setVariable(const QString& name, Value* v)
{
	variables.insert(name,v);
}

Value* Context::lookupVariable(const QString& name,Variable::Storage_e& c,Layout* l) const
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
Node* Context::lookupChild(int index) const
{
	QList<Node*> children=getInputNodes();
	if(index>=0&&index<children.length())
		return children.at(index);
	if(parent)
		return parent->lookupChild(index);

	return nullptr;
}

/* Lookup children doesn't currently
 * check the lexical scope of the
 * parent */
QList<Node*> Context::lookupChildren() const
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
	for(auto i=0; i<parameters.size(); ++i) {
		auto param=parameters.at(i);
		QString paramName=param.first;
		Value* paramVal=param.second;
		bool found=false;
		for(auto arg: arguments) {
			QString argName=arg.first;
			Value* argVal=arg.second;
			if(argVal->isDefined()&&argName==paramName) {
				paramVal=argVal;
				found=true;
				break;
			}
		}
		if(!found&&i<arguments.size()) {
			auto arg=arguments.at(i);
			QString argName=arg.first;
			Value* argVal=arg.second;
			if(argVal->isDefined()&&argName.isEmpty()) {
				paramVal=argVal;
			}
		}

		variables.insert(paramName,paramVal);
	}
}

QList<QPair<QString,Value*>> Context::getArguments() const
{
	return arguments;
}

QList<Value*> Context::getArgumentValues() const
{
	QList<Value*> values;
	for(auto arg: arguments)
		values.append(arg.second);
	return values;
}

void Context::addArgument(QPair<QString,Value*> value)
{
	arguments.append(value);
}

void Context::addArgument(const QString& name,Value* value)
{
	addArgument(QPair<QString,Value*>(name,value));
}

void Context::clearArguments()
{
	arguments.clear();
}

Value* Context::getArgument(int index, const QString& name) const
{
	//TODO make matchLast work for name ending with any digit
	bool matchLast = name.endsWith('1') || name.endsWith('2');

	return matchArgumentIndex(true,matchLast,index,name);
}

Value* Context::getArgumentDeprecatedModule(int index, const QString& deprecated, const QString& module, Reporter& r) const
{
	Value* v = matchArgumentIndex(false,false,index,deprecated);
	if(v)
		r.reportWarning(tr("'%1' parameter is deprecated use %2 instead").arg(deprecated).arg(module));
	return v;
}

Value* Context::getArgumentDeprecated(int index, const QString& name, const QString& deprecated, Reporter& r) const
{
	Value* v = matchArgumentIndex(true,false,index,name);
	if(!v) {
		v = matchArgumentIndex(false,false,index,deprecated);
		if(v)
			r.reportWarning(tr("'%1' parameter is deprecated use '%2' instead").arg(deprecated).arg(name));
	}

	return v;
}

void Context::clearParameters()
{
	parameters.clear();
}

void Context::addParameter(const QString& name,Value* value)
{
	parameters.append(QPair<QString,Value*>(name,value));
}

void Context::setInputNodes(const QList<Node*>& value)
{
	inputNodes=value;
}

QList<Node*> Context::getInputNodes() const
{
	return inputNodes;
}

void Context::setCurrentNodes(const QList<Node*>& value)
{
	currentNodes=value;
}

QList<Node*> Context::getCurrentNodes() const
{
	return currentNodes;
}

void Context::addCurrentNode(Node* value)
{
	currentNodes.append(value);
}

Value* Context::getArgumentSpecial(const QString& name) const
{
	Value* v=matchArgument(false,false,name);
	if(v && v->getStorage()==Variable::Special)
		return v;

	return nullptr;
}

Value* Context::matchArgumentIndex(bool allowChar,bool matchLast, int index, const QString& name) const
{
	if(index >= arguments.size())
		return matchArgument(allowChar,matchLast,name);

	auto arg = arguments.at(index);
	QString argName = arg.first;
	if(argName.isEmpty() || match(allowChar,matchLast,argName,name))
		return arg.second;

	return matchArgument(allowChar,matchLast,name);
}

Value* Context::matchArgument(bool allowChar,bool matchLast, const QString& name) const
{
	for(auto namedArg: arguments) {
		QString namedArgName = namedArg.first;
		if(match(allowChar,matchLast,namedArgName,name))
			return namedArg.second;
	}

	return nullptr;
}

bool Context::match(bool allowChar,bool matchLast, const QString& a,const QString& n) const
{
	if(allowChar) {
		if(matchLast&&a.length()==2)
			return a.left(1)==n.left(1) && a.right(1)==n.right(1);
		if(!matchLast&&a.length()==1)
			return a.left(1)==n.left(1);
	}
	return a==n;
}
