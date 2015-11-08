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
	~TreePrinter();
	void visit(Module*);
	void visit(ModuleScope*);
	void visit(Instance*);
	void visit(Function*);
	void visit(FunctionScope*);
	void visit(CompoundStatement*);
	void visit(IfElseStatement*);
	void visit(ForStatement*);
	void visit(Parameter*);
	void visit(BinaryExpression*);
	void visit(Argument*);
	void visit(AssignStatement*);
	void visit(VectorExpression*);
	void visit(RangeExpression*);
	void visit(UnaryExpression*);
	void visit(ReturnStatement*);
	void visit(TernaryExpression*);
	void visit(Invocation*);
	void visit(Callback*);
	void visit(ModuleImport*);
	void visit(ScriptImport*);
	void visit(Literal*);
	void visit(Variable*);
	void visit(CodeDoc*);
	void visit(Script*);
	void visit(Product*);
	void visit(ComplexExpression*);
private:
	void createIndent();
	QTextStream& result;
	unsigned int indent;
};

#endif // TREEPRINTER_H
