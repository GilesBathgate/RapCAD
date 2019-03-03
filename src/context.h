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
	void setCurrentName(const QString&);

	Value* lookupVariable(const QString&,Variable::Storage_e&,Layout*) const;
	bool addVariable(const QString&,Value*);
	void setVariable(const QString&,Value*);

	Node* lookupChild(int) const;
	QList<Node*> lookupChildren() const;

	void setVariablesFromArguments();
	QList<QPair<QString,Value*>> getArguments() const;
	QList<Value*> getArgumentValues() const;
	void addArgument(const QString&, Value*);
	void addArgument(QPair<QString,Value*>);
	void clearArguments();

	Value* getArgument(int,const QString&) const;
	Value* getArgumentSpecial(const QString&) const;
	Value* getArgumentDeprecated(int, const QString&, const QString&, Reporter&) const;
	Value* getArgumentDeprecatedModule(int, const QString&, const QString&, Reporter&) const;

	void clearParameters();
	void addParameter(const QString&, Value*);

	void setInputNodes(const QList<Node*>&);
	QList<Node*> getInputNodes() const;

	void setCurrentNodes(const QList<Node*>&);
	QList<Node*> getCurrentNodes() const;
	void addCurrentNode(Node*);
private:
	Context* parent;
	QList<QPair<QString,Value*>> arguments;
	QList<QPair<QString,Value*>> parameters;
	QList<Node*> currentNodes;
	QList<Node*> inputNodes;
	Value* currentValue;
	Value* returnValue;
	QString currentName;
	Scope* currentScope;
	Value* matchArgumentIndex(bool,bool,int,const QString&) const;
	Value* matchArgument(bool,bool,const QString&) const;
	bool match(bool,bool,const QString&,const QString&) const;
	QHash<QString,Value*> variables;
};

#endif // CONTEXT_H
