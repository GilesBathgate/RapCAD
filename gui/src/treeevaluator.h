/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#ifndef TREEEVALUATOR_H
#define TREEEVALUATOR_H

#include "argument.h"
#include "assignstatement.h"
#include "binaryexpression.h"
#include "callback.h"
#include "codedocdeclaration.h"
#include "codedocparam.h"
#include "complexexpression.h"
#include "compoundstatement.h"
#include "context.h"
#include "forstatement.h"
#include "function.h"
#include "functionscope.h"
#include "ifelsestatement.h"
#include "instance.h"
#include "intervalexpression.h"
#include "invocation.h"
#include "layout.h"
#include "literal.h"
#include "module.h"
#include "module/importmodule.h"
#include "moduleimport.h"
#include "modulescope.h"
#include "parameter.h"
#include "product.h"
#include "rangeexpression.h"
#include "returnstatement.h"
#include "script.h"
#include "scriptimport.h"
#include "ternaryexpression.h"
#include "treevisitor.h"
#include "unaryexpression.h"
#include "variable.h"
#include "vectorexpression.h"
#include <QStack>

class TreeEvaluator : public TreeVisitor
{
	Q_DECLARE_TR_FUNCTIONS(TreeEvaluator)
	Q_DISABLE_COPY_MOVE(TreeEvaluator)
public:
	explicit TreeEvaluator(Reporter&);
	~TreeEvaluator() override;
	void visit(const Module&) override;
	void visit(const ModuleScope&) override;
	void visit(const Instance&) override;
	void visit(const Function&) override;
	void visit(const FunctionScope&) override;
	void visit(const CompoundStatement&) override;
	void visit(const IfElseStatement&) override;
	void visit(const ForStatement&) override;
	void visit(const Parameter&) override;
	void visit(const BinaryExpression&) override;
	void visit(const Argument&) override;
	void visit(const AssignStatement&) override;
	void visit(const VectorExpression&) override;
	void visit(const IntervalExpression&) override;
	void visit(const RangeExpression&) override;
	void visit(const UnaryExpression&) override;
	void visit(const ReturnStatement&) override;
	void visit(const TernaryExpression&) override;
	void visit(const Invocation&) override;
	void visit(const ModuleImport&) override;
	void visit(const ScriptImport&) override;
	void visit(const Literal&) override;
	void visit(const Variable&) override;
	void visit(const CodeDocParam&) override;
	void visit(const CodeDocDeclaration&) override;
	void visit(const ComplexExpression&) override;

	void visit(Script&) override;
	void visit(Product&) override;
	void visit(Callback&) override;

	Node* getRootNode() const;

private:
	void startContext(Scope*);
	void finishContext();
	void descend(Scope*);
	void startLayout(Scope*);
	void finishLayout();
	QFileInfo getFullPath(const QString&);

	Reporter& reporter;
	Context* context;
	Layout* layout;
	QStack<Context*> contextStack;
	QStack<Layout*> layoutStack;
	QHash<Scope*,Layout*> scopeLookup;
	bool descendDone;
	Node* rootNode;
	QList<ImportModule*> modules;
	QHash<const ScriptImport*,Script*> imports;
	QStack<QDir> importLocations;
};

#endif // TREEEVALUATOR_H
