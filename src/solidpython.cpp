/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include "solidpython.h"
#include "numbervalue.h"
#include <stdio.h>

class NotImplementedException {};

SolidPython::SolidPython()
{
	this->indent=0;
}

SolidPython::~SolidPython()
{
}

void SolidPython::createIndent()
{
	for(unsigned int i=0; i<indent; i++)
		result.append("  ");
}

void SolidPython::visit(ModuleScope* scp)
{
	++indent;
	char varname = 'a';
	QVector<Instance*> instances;
	QVector<Declaration*> decls = scp->getDeclarations();
	bool lastinstance=false;
	foreach(Declaration* d, decls) {
		Instance* inst = dynamic_cast<Instance*>(d);
		if(inst) {
			instances.append(inst);
			lastinstance=true;
		} else {
			lastinstance=false;
		}
	}

	if(lastinstance&&instances.size()==1) {
		foreach(Declaration* d, decls) {
			createIndent();
			Instance* inst = dynamic_cast<Instance*>(d);
			if(inst) {
				result.append("return ");
			}
			d->accept(*this);
		}
	} else {

		foreach(Declaration* d, decls) {
			createIndent();
			Instance* inst = dynamic_cast<Instance*>(d);
			if(inst) {
				result.append("__");
				result.append(varname);
				result.append(" = ");
				++varname;
				if(varname>'z')
					throw new NotImplementedException();
			}
			d->accept(*this);
			if(inst)
				result.append("\n");
		}
		if(instances.size()>0) {
			createIndent();
			result.append("return union()(");
			char var='a';
			for(int i=0; var<varname; var++,i++) {
				if(i>0)
					result.append(",");
				result.append("__");
				result.append(var);
			}
			result.append(")\n");
		}
	}
	--indent;
}

void SolidPython::visit(Instance* inst)
{
	result.append(inst->getName());
	result.append("(");
	QVector<Argument*> arguments = inst->getArguments();
	int s = arguments.size();
	for(int i=0; i<s; i++) {
		arguments.at(i)->accept(*this);
		if(i+1<s)
			result.append(",");
	}
	result.append(")");

	QVector<Statement*> children = inst->getChildren();
	int c = children.size();
	if(c>0) {
		result.append("(\n");
		++indent;
		for(int i=0; i<c; i++) {
			if(i>0 && i<=c) {
				result.append(",\n");
				createIndent();
			} else
				createIndent();
			children.at(i)->accept(*this);
		}
		result.append("\n");
		--indent;
		createIndent();
		result.append(")");
	}
}

void SolidPython::visit(Module* mod)
{
	result.append("def ");
	result.append(mod->getName());
	result.append("(");
	QVector<Parameter*> parameters = mod->getParameters();
	int s = parameters.size();
	for(int i=0; i<s; i++) {
		parameters.at(i)->accept(*this);
		if(i+1<s)
			result.append(",");
	}
	result.append("):\n");
	mod->getScope()->accept(*this);
	result.append("\n\n");
}

void SolidPython::visit(Function* func)
{
	result.append("def ");
	result.append(func->getName());
	result.append("(");
	QVector<Parameter*> parameters = func->getParameters();
	int s = parameters.size();
	for(int i=0; i<s; i++) {
		parameters.at(i)->accept(*this);
		if(i+1<s)
			result.append(",");
	}

	result.append("):\n");
	func->getScope()->accept(*this);
	result.append("\n\n");
}

void SolidPython::visit(FunctionScope* scp)
{
	Expression* expression = scp->getExpression();
	if(expression) {
		++indent;
		createIndent();
		result.append("return ");
		expression->accept(*this);
		--indent;
		return;
	}

	QVector<Statement*> statements = scp->getStatements();
	int s = statements.size();
	if(s>0) {
		++indent;
		for(int i=0; i<s; i++) {
			createIndent();
			statements.at(i)->accept(*this);
		}
		--indent;
	}
}

void SolidPython::visit(CompoundStatement* stmt)
{
	QVector<Statement*> children = stmt->getChildren();
	int c = children.size();
	if(c>0) {
		if(c>1) {
			result.append("\n");
			++indent;
		}
		for(int i=0; i<c; i++) {
			if(c>1)
				createIndent();
			children.at(i)->accept(*this);
		}
		if(c>1) {
			--indent;
			createIndent();
		}
	}
}

void SolidPython::visit(IfElseStatement* ifelse)
{
	result.append("if ");
	ifelse->getExpression()->accept(*this);
	result.append(":\n");
	Statement* trueStatement = ifelse->getTrueStatement();
	if(trueStatement) {
		++indent;
		createIndent();
		trueStatement->accept(*this);
		result.append("\n");
		--indent;
	}

	Statement* falseStatement = ifelse->getFalseStatement();
	if(falseStatement) {
		createIndent();
		result.append("else:\n");
		++indent;
		createIndent();
		falseStatement->accept(*this);
		result.append("\n");
		--indent;
	}
}

void SolidPython::visit(ForStatement* forstmt)
{
	int levels=0;
	QVector<Argument*> args = forstmt->getArguments();
	int c = args.size();
	for(int i=0; i<c; i++) {
		Argument* a = args.at(i);

		if(i>0) {
			++indent;
			++levels;
			createIndent();
		}
		result.append("for ");
		Variable* variable = a->getVariable();
		if(variable) {
			variable->accept(*this);
			result.append(" in ");
		}

		a->getExpression()->accept(*this);
		result.append(":\n");
	}

	++indent;
	createIndent();
	Statement* statement = forstmt->getStatement();
	statement->accept(*this);
	--indent;

	indent-=levels;
}

void SolidPython::visit(Parameter* param)
{
	result.append(param->getName());

	Expression* expression = param->getExpression();
	if(expression) {
		result.append("=");
		expression->accept(*this);
	}
}

void SolidPython::visit(BinaryExpression* exp)
{
	result.append("(");
	exp->getLeft()->accept(*this);
	result.append(exp->getOpString());
	exp->getRight()->accept(*this);
	result.append(")");
}

void SolidPython::visit(Argument* arg)
{
	Variable* variable = arg->getVariable();
	if(variable) {
		variable->accept(*this);
		result.append("=");
	}

	arg->getExpression()->accept(*this);
}

void SolidPython::visit(AssignStatement* stmt)
{
	Variable* var = stmt->getVariable();
	if(var)
		var->accept(*this);
	result.append("=");
	Expression* expression = stmt->getExpression();
	if(expression)
		expression->accept(*this);

	result.append("\n");
}

void SolidPython::visit(VectorExpression* exp)
{
	result.append("[");
	QVector<Expression*> children = exp->getChildren();
	int s = children.size();
	for(int i=0; i<s; i++) {
		children.at(i)->accept(*this);
		if(i+1<s)
			result.append(",");
	}
	result.append("]");
}

void SolidPython::visit(RangeExpression* exp)
{
	result.append("range(");
	exp->getStart()->accept(*this);
	result.append(",");

	Expression* fin = exp->getFinish();
	Literal* val = dynamic_cast<Literal*>(fin);
	if(val) {
		NumberValue* num = dynamic_cast<NumberValue*>(val->getValue());
		if(num) {
			double n = num->getNumber();
			QString ns = QString().number(n+1,'g',16);
			result.append(ns);
		} else {
			fin->accept(*this);
			result.append("+1");
		}
	} else {
		fin->accept(*this);
		result.append("+1");
	}

	Expression* step = exp->getStep();
	if(step) {
		result.append(",");
		step->accept(*this);
	}

	result.append(")");
}

void SolidPython::visit(UnaryExpression* exp)
{
	result.append(exp->getOpString());
	exp->getExpression()->accept(*this);
}

void SolidPython::visit(ReturnStatement* stmt)
{
	result.append("return ");
	stmt->getExpression()->accept(*this);
	result.append("\n");
}

void SolidPython::visit(TernaryExpression* exp)
{
	result.append("(");
	exp->getCondition()->accept(*this);
	result.append("?");
	exp->getTrueExpression()->accept(*this);
	result.append(":");
	exp->getFalseExpression()->accept(*this);
	result.append(")");
}

void SolidPython::visit(Invocation* stmt)
{
	result.append(stmt->getName());
	result.append("(");
	QVector<Argument*> arguments = stmt->getArguments();
	int s = arguments.size();
	for(int i=0; i<s; i++) {
		arguments.at(i)->accept(*this);
		if(i+1<s)
			result.append(",");
	}
	result.append(")");
}

void SolidPython::visit(ModuleImport*)
{
	throw NotImplementedException();
}

void SolidPython::visit(ScriptImport* imp)
{
	result.append("use(");
	result.append(imp->getImport());
	result.append(")\n");
}

void SolidPython::visit(Literal* lit)
{
	QString val = lit->getValueString();
	if(val=="false")
		val="False";
	if(val=="true")
		val="True";

	result.append(val);
}

void SolidPython::visit(Variable* var)
{
	if(var->getType()==Variable::Special)
		result.append("$");
	result.append(var->getName());
}

void SolidPython::visit(Script* sc)
{
	foreach(Declaration* d, sc->getDeclarations())
		d->accept(*this);

	printf("%s",result.toLocal8Bit().constData());
}
