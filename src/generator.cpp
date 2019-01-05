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

#include "generator.h"
#include "builtincreator.h"
#include "asciidocprinter.h"

Generator::Generator(Reporter& r) : Strategy(r)
{
}

int Generator::evaluate()
{
	QFile modFile("modules.asciidoc"),funcFile("functions.asciidoc");
	if(!modFile.open(QFile::WriteOnly | QFile::Truncate)) return EXIT_FAILURE;
	if(!funcFile.open(QFile::WriteOnly | QFile::Truncate)) return EXIT_FAILURE;
	QTextStream mod(&modFile),func(&funcFile);
	AsciidocPrinter p(mod,func);
	BuiltinCreator* b=BuiltinCreator::getInstance(reporter);
	b->generateDocs(p);
	return EXIT_SUCCESS;
}
