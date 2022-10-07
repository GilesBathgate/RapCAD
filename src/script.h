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

#ifndef SCRIPT_H
#define SCRIPT_H

#include "codedocparam.h"
#include "declaration.h"
#include "reporter.h"
#include "scope.h"
#include <QList>

class Script : public Scope
{
public:
	Script(Reporter&);
	~Script() override;

	void parse(const QString&);
	void parse(const QFileInfo&);

	bool isEmpty();
	void setDeclarations(const QList<Declaration*>&);
	const QList<Declaration*> getDeclarations() const override;
	void addDeclaration(Declaration*);
	void appendDeclaration(Declaration*);
	void removeDeclaration(Declaration*);
	void addDocumentation(const QList<CodeDocParam*>&);
	const QList<QList<CodeDocParam*> > getDocumentation() const;
	void accept(TreeVisitor&) override;
	QDir getFileLocation() const;
	void setFileLocation(const QDir&);
private:
	QList<Declaration*> declarations;
	QList<QList<CodeDocParam*> > documentation;
	QDir fileLocation;
	Reporter& reporter;
};

#endif // SCRIPT_H
