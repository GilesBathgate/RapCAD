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

#ifndef TREEVISITOR_H
#define TREEVISITOR_H

class TreeVisitor
{
public:
	virtual ~TreeVisitor() {}
	virtual void visit(const class Module&)=0;
	virtual void visit(const class ModuleScope&)=0;
	virtual void visit(const class Instance&)=0;
	virtual void visit(const class Function&)=0;
	virtual void visit(const class FunctionScope&)=0;
	virtual void visit(const class CompoundStatement&)=0;
	virtual void visit(const class IfElseStatement&)=0;
	virtual void visit(const class ForStatement&)=0;
	virtual void visit(const class Parameter&)=0;
	virtual void visit(const class BinaryExpression&)=0;
	virtual void visit(const class Argument&)=0;
	virtual void visit(const class AssignStatement&)=0;
	virtual void visit(const class VectorExpression&)=0;
	virtual void visit(const class RangeExpression&)=0;
	virtual void visit(const class UnaryExpression&)=0;
	virtual void visit(const class ReturnStatement&)=0;
	virtual void visit(const class TernaryExpression&)=0;
	virtual void visit(const class Invocation&)=0;
	virtual void visit(const class ModuleImport&)=0;
	virtual void visit(const class ScriptImport&)=0;
	virtual void visit(const class Literal&)=0;
	virtual void visit(const class Variable&)=0;
	virtual void visit(const class CodeDoc&)=0;
	virtual void visit(const class ComplexExpression&)=0;

	virtual void visit(class Script&)=0;
	virtual void visit(class Product&)=0;
	virtual void visit(class Callback&)=0;
};

#endif // TREEVISITOR_H
