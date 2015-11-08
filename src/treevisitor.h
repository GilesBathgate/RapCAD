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

#ifndef TREEVISITOR_H
#define TREEVISITOR_H

class TreeVisitor
{
public:
	virtual ~TreeVisitor() {}
	virtual void visit(class Module*)=0;
	virtual void visit(class ModuleScope*)=0;
	virtual void visit(class Instance*)=0;
	virtual void visit(class Function*)=0;
	virtual void visit(class FunctionScope*)=0;
	virtual void visit(class CompoundStatement*)=0;
	virtual void visit(class IfElseStatement*)=0;
	virtual void visit(class ForStatement*)=0;
	virtual void visit(class Parameter*)=0;
	virtual void visit(class BinaryExpression*)=0;
	virtual void visit(class Argument*)=0;
	virtual void visit(class AssignStatement*)=0;
	virtual void visit(class VectorExpression*)=0;
	virtual void visit(class RangeExpression*)=0;
	virtual void visit(class UnaryExpression*)=0;
	virtual void visit(class ReturnStatement*)=0;
	virtual void visit(class TernaryExpression*)=0;
	virtual void visit(class Invocation*)=0;
	virtual void visit(class Callback*)=0;
	virtual void visit(class ModuleImport*)=0;
	virtual void visit(class ScriptImport*)=0;
	virtual void visit(class Literal*)=0;
	virtual void visit(class Variable*)=0;
	virtual void visit(class CodeDoc*)=0;
	virtual void visit(class Script*)=0;
	virtual void visit(class Product*)=0;
	virtual void visit(class ComplexExpression*)=0;
};

#endif // TREEVISITOR_H
