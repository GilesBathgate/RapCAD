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

#include "asciidocprinter.h"
#include "onceonly.h"

AsciidocPrinter::AsciidocPrinter(QTextStream& m,QTextStream& f) :
	modulesOutput(m),
	functionsOutput(f)
{
	functionsOutput << ".RapCAD functions\n";
	functionsOutput << "[width=\"100%\",frame=\"topbot\",options=\"header\"]\n";
	functionsOutput << "|======================\n";
	functionsOutput << "|Function |Description\n";
}

AsciidocPrinter::~AsciidocPrinter()
{
	functionsOutput << "|======================\n";
	functionsOutput.flush();
	modulesOutput.flush();
}

static QString capitalize(QString str)
{
	str=str.toLower();
	str[0]=str[0].toUpper();
	return str;
}

void AsciidocPrinter::visit(const Module& mod)
{
	QString name=mod.getName();
	modulesOutput << capitalize(name) << "\n";
	modulesOutput << QString(name.length(),'^') << "\n\n";
	modulesOutput << mod.getDescription() << "\n\n";
	QList<Parameter*> params=mod.getParameters();
	if(!params.isEmpty()) {
		modulesOutput << "Parameters\n";
		modulesOutput << "++++++++++\n";
		modulesOutput << "|=========\n";
		for(Parameter* p: params) {
			modulesOutput << "|" << p->getName() << "|" << p->getDescription() << "\n";
		}
		modulesOutput << "|=========\n\n";
		modulesOutput << "Examples\n";
		modulesOutput << "++++++++\n";
		modulesOutput << "[source,csharp]\n";
		modulesOutput << "---------------\n";
		modulesOutput << name << "();\n";
		modulesOutput << name << "(";
		OnceOnly first;
		for(Parameter* p: params) {
			if(!first())
				modulesOutput << ",";
			modulesOutput << p->getName();
		}
		modulesOutput << ");\n";
		modulesOutput << "---------------\n\n";
	}
}

void AsciidocPrinter::visit(const Function & func)
{
	functionsOutput << "|" << func.getName() << "(";
	OnceOnly first;
	for(Parameter* p: func.getParameters()) {
		if(!first())
			functionsOutput << ",";
		functionsOutput << p->getName();
	}
	functionsOutput << ") | " << func.getDescription() << "\n";
}
