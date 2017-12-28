/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2017 Giles Bathgate
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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QHash>
#include <QTextStream>
#include "value.h"
#include "node.h"
#include "scope.h"
#include "layout.h"

class Context
{
	Q_DECLARE_TR_FUNCTIONS(Context)
public:
	Context();
	virtual ~Context();

	void setParent(Context*);

	void setCurrentScope(Scope*);
	Scope* getCurrentScope() const;

	void setReturnValue(Value*);
	Value* getReturnValue() const;

	Value* getCurrentValue() const;
	void setCurrentValue(Value*);

	QString getCurrentName() const;
	void setCurrentName(QString);

	Value* lookupVariable(QString,Variable::Storage_e&,Layout*) const;
	bool addVariable(Value*);
	void setVariable(Value*);

	Node* lookupChild(int) const;
	QList<Node*> lookupChildren() const;

	void setVariablesFromArguments();
	QList<Value*> getArguments() const;
	void addArgument(Value*);
	void clearArguments();

	Value* getArgument(int,QString) const;
	Value* getArgumentSpecial(QString) const;
	Value* getArgumentDeprecated(int,QString,QString,Reporter*) const;
	Value* getArgumentDeprecatedModule(int,QString,QString,Reporter*) const;

	void clearParameters();
	void addParameter(Value*);

	void setInputNodes(QList<Node*>);
	QList<Node*> getInputNodes() const;

	void setCurrentNodes(QList<Node*>);
	QList<Node*> getCurrentNodes() const;
	void addCurrentNode(Node*);
private:
	Context* parent;
	QList<Value*> arguments;
	QList<Value*> parameters;
	QList<Node*> currentNodes;
	QList<Node*> inputNodes;
	Value* currentValue;
	Value* returnValue;
	QString currentName;
	Scope* currentScope;
	Value* matchArgumentIndex(bool,bool,int,QString) const;
	Value* matchArgument(bool,bool,QString) const;
	bool match(bool,bool,QString,QString) const;
	QHash<QString,Value*> variables;
};

#endif // CONTEXT_H
