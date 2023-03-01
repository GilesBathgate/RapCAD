/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#ifndef MODULE_H
#define MODULE_H

#include "declaration.h"
#include "node.h"
#include "parameter.h"
#include "reporter.h"
#include "scope.h"
#include "value.h"
#include <QList>
#include <QString>

class Context;

class Module : public Declaration
{
public:
	Module(Reporter&, const QString&);
	~Module() override;
	QString getName() const;
	void setName(const QString&);
	QString getFullName() const;
	QString getDescription() const;
	QString getExample() const;
	bool getAuxilary() const;
	const QList<Parameter*>& getParameters() const;
	void setParameters(const QList<Parameter*>&);
	Scope* getScope() const;
	void setScope(Scope*);
	void accept(TreeVisitor&) override;
	virtual Node* evaluate(const Context&) const;
	bool isDeprecated() const;
	bool hasExample() const;
protected:
	void addDescription(const QString&);
	void addExample(const QString&);
	void addDeprecated(const QString&);
	void addParameter(const QString&,const QString& t,const QString&);
	template <class V>
	V* getParameterArgument(const Context&,int) const;
	template <class V>
	V* getParameterArgument(const Context&,int,int) const;

	bool auxilary;
	Reporter& reporter;
private:
	Value* getArgument(const Context&,int,const QString&) const;
	QString name;
	QString description;
	QString example;
	bool deprecated;
	QList<Parameter*> parameters;
	Scope* scope;
};

template <class V>
V* Module::getParameterArgument(const Context& ctx,int index) const
{
	return getParameterArgument<V>(ctx,index,index);
}

template <class V>
V* Module::getParameterArgument(const Context& ctx,int index,int expectedIndex) const
{
	Parameter* p=parameters.at(index);
	return dynamic_cast<V*>(getArgument(ctx,expectedIndex,p->getName()));
}

#endif // MODULE_H
