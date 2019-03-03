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

#ifndef ASCIIDOCPRINTER_H
#define ASCIIDOCPRINTER_H

#include "treevisitor.h"
#include "module.h"
#include "modulescope.h"
#include "function.h"
#include "functionscope.h"
#include "compoundstatement.h"
#include "ifelsestatement.h"
#include "forstatement.h"
#include "binaryexpression.h"
#include "assignstatement.h"
#include "vectorexpression.h"
#include "rangeexpression.h"
#include "unaryexpression.h"
#include "returnstatement.h"
#include "ternaryexpression.h"
#include "invocation.h"
#include "callback.h"
#include "moduleimport.h"
#include "scriptimport.h"
#include "literal.h"
#include "codedoc.h"
#include "script.h"
#include "product.h"
#include "complexexpression.h"

class AsciidocPrinter : public TreeVisitor
{
public:
	explicit AsciidocPrinter(QTextStream&,QTextStream&);
	~AsciidocPrinter() override;
	void visit(const Module&) override;
	void visit(const ModuleScope&) override {}
	void visit(const Instance&) override {}
	void visit(const Function&) override;
	void visit(const FunctionScope&) override {}
	void visit(const CompoundStatement&) override {}
	void visit(const IfElseStatement&) override {}
	void visit(const ForStatement&) override {}
	void visit(const Parameter&) override {}
	void visit(const BinaryExpression&) override {}
	void visit(const Argument&) override {}
	void visit(const AssignStatement&) override {}
	void visit(const VectorExpression&) override {}
	void visit(const RangeExpression&) override {}
	void visit(const UnaryExpression&) override {}
	void visit(const ReturnStatement&) override {}
	void visit(const TernaryExpression&) override {}
	void visit(const Invocation&) override {}
	void visit(const ModuleImport&) override {}
	void visit(const ScriptImport&) override {}
	void visit(const Literal&) override {}
	void visit(const Variable&) override {}
	void visit(const CodeDoc&) override {}
	void visit(const ComplexExpression&) override {}

	void visit(Script&) override {}
	void visit(Product&) override {}
	void visit(Callback&) override {}

private:
	QTextStream& modulesOutput;
	QTextStream& functionsOutput;
};

#endif // ASCIIDOCPRINTER_H
