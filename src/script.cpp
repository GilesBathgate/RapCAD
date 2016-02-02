/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

#include "script.h"

Script::Script()
{
	fileLocation=NULL;
}

Script::~Script()
{
	foreach(Declaration* d,declarations)
		delete d;

	delete fileLocation;
}

void Script::setDeclarations(QList<Declaration*> decls)
{
	this->declarations = decls;
}

QList<Declaration*> Script::getDeclarations() const
{
	return this->declarations;
}

void Script::addDeclaration(Declaration* dec)
{
	declarations.prepend(dec);
}

void Script::appendDeclaration(Declaration* dec)
{
	declarations.append(dec);
}

void Script::removeDeclaration(Declaration* dec)
{
	declarations.removeAll(dec);
}

void Script::addDocumentation(QList<CodeDoc*> docs)
{
	documentation.append(docs);
}

QList<QList<CodeDoc*> > Script::getDocumentation()
{
	return documentation;
}

void Script::accept(TreeVisitor& v)
{
	v.visit(this);
}

QFileInfo* Script::getFileLocation() const
{
	return fileLocation;
}

void Script::setFileLocation(QFileInfo* value)
{
	fileLocation = value;
}
