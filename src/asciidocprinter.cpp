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

#include "asciidocprinter.h"
#include "onceonly.h"

AsciidocPrinter::AsciidocPrinter(QTextStream& s) : result(s)
{
}

AsciidocPrinter::~AsciidocPrinter()
{
}

static QString capitalize(const QString &str)
{
	QString tmp=str;
	tmp=tmp.toLower();
	tmp[0]=str[0].toUpper();
	return tmp;
}

void AsciidocPrinter::visit(Module* mod)
{
	QString name=mod->getName();
	result << capitalize(name) << "\n";
	result << QString(name.length(),'^') << "\n\n";
	result << mod->getDescription() << "\n\n";
	QList<Parameter*> params=mod->getParameters();
	if(!params.isEmpty()) {
		result << "Parameters\n";
		result << "++++++++++\n";
		result << "|=========\n";
		foreach(Parameter* p,params) {
			result << "|" << p->getName() << "|" << p->getDescription() << "\n";
		}
		result << "|=========\n\n";
		result << "Examples\n";
		result << "++++++++\n";
		result << "[source,csharp]\n";
		result << "---------------\n";
		result << name << "();\n";
		result << name << "(";
		OnceOnly first;
		foreach(Parameter* p,params) {
			if(!first())
				result << ",";
			result << p->getName();
		}
		result << ");\n";
		result << "---------------\n\n";
	}
}
