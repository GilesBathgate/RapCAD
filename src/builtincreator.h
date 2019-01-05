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

#ifndef BUILTINCREATOR_H
#define BUILTINCREATOR_H

#include <QHash>
#include "reporter.h"
#include "declaration.h"
#include "script.h"
#include "module.h"

class BuiltinCreator
{
public:
	static BuiltinCreator* getInstance(Reporter&);
	static void cleanUp();
	void initBuiltins(Script&);
	void saveBuiltins(Script&);
	void generateDocs(QTextStream&);
	void generateDocs(TreeVisitor&);
	QHash<QString,Module*> getModuleNames() const;
private:
	explicit BuiltinCreator(Reporter&);
	~BuiltinCreator();
	static BuiltinCreator* instance;
	QList<Declaration*> builtins;
};
#endif // BUILTINCREATOR_H
