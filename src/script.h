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

#ifndef SCRIPT_H
#define SCRIPT_H

#include <QList>
#include "declaration.h"
#include "scope.h"
#include "codedoc.h"
#include "reporter.h"

class Script : public Scope
{
public:
	Script(Reporter&);
	~Script() override;

	void parse(const QString&);
	void parse(QFileInfo);

	void setDeclarations(const QList<Declaration*>&);
	QList<Declaration*> getDeclarations() const override;
	void addDeclaration(Declaration*);
	void appendDeclaration(Declaration*);
	void removeDeclaration(Declaration*);
	void addDocumentation(const QList<CodeDoc*>&);
	QList<QList<CodeDoc*> > getDocumentation() const;
	void accept(TreeVisitor&) override;
	QDir getFileLocation() const;
	void setFileLocation(QDir);
private:
	QList<Declaration*> declarations;
	QList<QList<CodeDoc*> > documentation;
	QDir fileLocation;
	Reporter& reporter;
};

#endif // SCRIPT_H
