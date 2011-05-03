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

QString SolidPython::getVariable()
{
	for(char var='a'; var<'z'; var++) {
		QChar v = var;
		if(!varnames.contains(v)) {
			varnames.append(v);
			return v;
		}
	}
	//TODO create double letter variable names
	throw new NotImplementedException();
}

void SolidPython::visit(ModuleScope* scp)
{
	++indent;
	QList<Instance*> instances;
	QList<Declaration*> decls = scp->getDeclarations();
	bool lastinstance=false;

	QString saveResult = result;
	foreach(Declaration* d, decls) {
		d->accept(*this);

		Instance* inst = dynamic_cast<Instance*>(d);
		if(inst) {
			instances.append(inst);
			lastinstance=true;
		} else {
			lastinstance=false;
		}
	}
	result=saveResult;

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
		QList<QString> vars;
		foreach(Declaration* d, decls) {
			createIndent();
			Instance* inst = dynamic_cast<Instance*>(d);
			if(inst) {
				varname=getVariable();
				result.append(varname);
				vars.append(varname);
				result.append("=");
			}
			ForStatement* forstmt = dynamic_cast<ForStatement*>(d);
			if(forstmt) {
				varname=getVariable();
				vars.append(varname);
				result.append(varname);
				result.append("=union()\n");
				createIndent();
			}
			d->accept(*this);
			if(inst)
				result.append("\n");
		}
		int c=vars.size();
		if(c>0) {
			createIndent();
			result.append("return ");
			if(c>1)
				result.append("union()(");
			for(int i=0; i<c; i++) {
				if(i>0)
					result.append(",");
				result.append(vars.at(i));
			}
			if(c>1)
				result.append(")\n");
		}
	}
	--indent;
}

void SolidPython::visit(Instance* inst)
{
	result.append(inst->getName());
	result.append("(");
	QList<Argument*> arguments = inst->getArguments();
	int s = arguments.size();
	for(int i=0; i<s; i++) {
		arguments.at(i)->accept(*this);
		if(i+1<s)
			result.append(",");
	}
	result.append(")");

	QList<Statement*> children = inst->getChildren();
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
	varnames.clear();
	result.append("def ");
	result.append(mod->getName());
	result.append("(");
	QList<Parameter*> parameters = mod->getParameters();
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
	QList<Parameter*> parameters = func->getParameters();
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

	QList<Statement*> statements = scp->getStatements();
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
	QList<Statement*> children = stmt->getChildren();
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
	QList<Argument*> args = forstmt->getArguments();
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
	result.append(varname);
	result.append(".add(");
	Statement* statement = forstmt->getStatement();
	statement->accept(*this);
	result.append(")\n");
	--indent;

	indent-=levels;
}

void SolidPython::visit(Parameter* param)
{
	QString v = param->getName();
	result.append(v);
	varnames.append(v);

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
	QList<Expression*> children = exp->getChildren();
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
	QList<Argument*> arguments = stmt->getArguments();
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
	QString v = var->getName();
	result.append(v);
	varnames.append(v);
}

void SolidPython::visit(CodeDoc*)
{
}

void SolidPython::visit(Script* sc)
{
	result.append("#!/usr/bin/python\n");
	result.append("import sys,os\n");
	result.append("from pyopenscad import *\n");
	result.append("\n");
	foreach(Declaration* d, sc->getDeclarations())
		d->accept(*this);

	printf("%s",result.toLocal8Bit().constData());
}
