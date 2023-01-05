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

#ifndef FUNCTION_H
#define FUNCTION_H

#include "declaration.h"
#include "parameter.h"
#include "scope.h"
#include "value.h"
#include "valuefactory.h"
#include <QCoreApplication>
#include <QList>
#include <QString>

class Context;

class Function : public Declaration
{
public:
	Function();
	explicit Function(const QString&);
	~Function() override;
	QString getName() const;
	void setName(const QString&);
	const QList<Parameter*> getParameters() const;
	void setParameters(const QList<Parameter*>&);
	Scope* getScope() const;
	void setScope(Scope*);
	void accept(TreeVisitor&) override;
	virtual Value& evaluate(const Context&) const;
	QString getDescription() const;

protected:
	void addDescription(const QString&);
	void addParameter(const QString&,const QString&,const QString&);
	template <class V>
	V* getParameterArgument(const Context&,int) const;
private:
	Value* getArgument(const Context&,int,const QString&) const;
	Scope* scope;
	QString name;
	QString description;
	QList<Parameter*> parameters;
};

template <class V>
V* Function::getParameterArgument(const Context& ctx,int index) const
{
	Parameter* p=parameters.at(index);
	return dynamic_cast<V*>(getArgument(ctx,index,p->getName()));
}

#endif // FUNCTION_H
