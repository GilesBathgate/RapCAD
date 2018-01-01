/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2018 Giles Bathgate
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

#include <QString>
#include <QList>
#include "declaration.h"
#include "parameter.h"
#include "declaration.h"
#include "scope.h"
#include "value.h"

class Context;

class Function : public Declaration
{
public:
	Function();
	Function(QString);
	~Function() override;
	QString getName() const;
	void setName(QString);
	QList<Parameter*> getParameters() const;
	void setParameters(QList<Parameter*>);
	Scope* getScope() const;
	void setScope(Scope*);
	void accept(TreeVisitor&) override;
	virtual Value* evaluate(const Context&) const;
protected:
	void addParameter(QString);
	Value* getParameterArgument(const Context&, int) const;
private:
	QString name;
	QList<Parameter*> parameters;
	Scope* scope;
};

#endif // FUNCTION_H
