/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2017 Giles Bathgate
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

#ifndef TREEPRINTER_H
#define TREEPRINTER_H

#include <QTextStream>
#include "treevisitor.h"
#include "script.h"
#include "declaration.h"
#include "module.h"
#include "modulescope.h"
#include "instance.h"
#include "function.h"
#include "functionscope.h"
#include "compoundstatement.h"
#include "ifelsestatement.h"
#include "forstatement.h"
#include "parameter.h"
#include "expression.h"
#include "binaryexpression.h"
#include "argument.h"
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
#include "variable.h"
#include "codedoc.h"
#include "product.h"
#include "complexexpression.h"

class TreePrinter : public TreeVisitor
{
public:
	TreePrinter(QTextStream&);
	~TreePrinter() override;
	void visit(Module&) override;
	void visit(ModuleScope&) override;
	void visit(Instance&) override;
	void visit(Function&) override;
	void visit(FunctionScope&) override;
	void visit(CompoundStatement&) override;
	void visit(IfElseStatement&) override;
	void visit(ForStatement&) override;
	void visit(Parameter&) override;
	void visit(BinaryExpression&) override;
	void visit(Argument&) override;
	void visit(AssignStatement&) override;
	void visit(VectorExpression&) override;
	void visit(RangeExpression&) override;
	void visit(UnaryExpression&) override;
	void visit(ReturnStatement&) override;
	void visit(TernaryExpression&) override;
	void visit(Invocation&) override;
	void visit(Callback&) override;
	void visit(ModuleImport&) override;
	void visit(ScriptImport&) override;
	void visit(Literal&) override;
	void visit(Variable&) override;
	void visit(CodeDoc&) override;
	void visit(Script&) override;
	void visit(Product&) override;
	void visit(ComplexExpression&) override;
private:
	void createIndent();
	QTextStream& result;
	unsigned int indent;
};

#endif // TREEPRINTER_H
