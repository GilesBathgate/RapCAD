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

#ifndef MODULE_H
#define MODULE_H

#include <QString>
#include <QList>
#include "parameter.h"
#include "declaration.h"
#include "instance.h"
#include "scope.h"
#include "node.h"
#include "value.h"
#include "reporter.h"

class Context;

class Module : public Declaration
{
public:
	Module(Reporter&, const QString&);
	~Module() override;
	QString getName() const;
	void setName(const QString&);
	QString getDescription() const;
	bool getAuxilary() const;
	QList<Parameter*> getParameters() const;
	void setParameters(const QList<Parameter*>&);
	Scope* getScope() const;
	void setScope(Scope*);
	void accept(TreeVisitor&) override;
	virtual Node* evaluate(const Context&) const;
	bool isDeprecated() const;
protected:
	void addDescription(const QString&);
	void addDeprecated(const QString&);
	void addParameter(const QString&,const QString&);
	Value* getParameterArgument(const Context&, int) const;
	Value* getParameterArgument(const Context&, int, int) const;

	bool auxilary;
	Reporter& reporter;
private:
	QString name;
	QString description;
	bool deprecated;
	QList<Parameter*> parameters;
	Scope* scope;
};

#endif // MODULE_H
