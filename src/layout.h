/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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

#ifndef LAYOUT_H
#define LAYOUT_H

#include "function.h"
#include "module.h"
#include "reporter.h"
#include "scope.h"
#include <QHash>

class Layout
{
	Q_DECLARE_TR_FUNCTIONS(Layout)
public:
	explicit Layout(Reporter&);
	virtual ~Layout() = default;

	void setParent(Layout*);

	const Module* lookupModule(const QString&,bool) const;
	void addModule(const Module& mod);

	const Function* lookupFunction(const QString&) const;
	void addFunction(const Function&);

	bool inScope(Scope*) const;
	void setScope(Scope*);

private:
	Reporter& reporter;
	Layout* parent;
	QHash<QString,const Module*> modules;
	QHash<QString,const Function*> functions;
	Scope* scope;
};

#endif // LAYOUT_H
