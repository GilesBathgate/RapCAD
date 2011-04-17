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

#include "treeprinter.h"
#include <stdio.h>

TreePrinter::TreePrinter()
{
	this->indent=0;
}

TreePrinter::~TreePrinter()
{
}

void TreePrinter::createIndent()
{
	for(unsigned int i=0; i<indent; i++)
		result.append("  ");
}

void TreePrinter::visit(ModuleScope* scp)
{
	++indent;
	foreach(Declaration* d, scp->getDeclarations()) {
		createIndent();
		d->accept(*this);
	}
	--indent;
}

void TreePrinter::visit(Instance* inst)
{

	switch(inst->getType()) {
	case Instance::Root:
		result.append("!");
		break;
	case Instance::Debug:
		result.append("#");
		break;
	case Instance::Background:
		result.append("%");
		break;
	case Instance::Disable:
		result.append("*");
		break;
	default:
		break;
	}

	QString name = inst->getNamespace();
	if(!name.isEmpty()) {
		result.append(name);
		result.append("::");
	}
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
		if(c>1) {
			result.append("{\n");
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
			result.append("}");
		}
	} else
		result.append(";");

	result.append("\n");
}

void TreePrinter::visit(Module* mod)
{
	result.append("module ");
	result.append(mod->getName());
	result.append("(");
	QVector<Parameter*> parameters = mod->getParameters();
	int s = parameters.size();
	for(int i=0; i<s; i++) {
		parameters.at(i)->accept(*this);
		if(i+1<s)
			result.append(",");
	}
	result.append("){\n");
	mod->getScope()->accept(*this);
	createIndent();
	result.append("}\n");
}

void TreePrinter::visit(Function* func)
{
	result.append("function ");
	result.append(func->getName());
	result.append("(");
	QVector<Parameter*> parameters = func->getParameters();
	int s = parameters.size();
	for(int i=0; i<s; i++) {
		parameters.at(i)->accept(*this);
		if(i+1<s)
			result.append(",");
	}

	result.append(")");
	func->getScope()->accept(*this);
	result.append("\n");
}

void TreePrinter::visit(FunctionScope* scp)
{
	Expression* expression = scp->getExpression();
	if(expression) {
		result.append("=");
		expression->accept(*this);
		result.append(";\n");
		return;
	}

	QVector<Statement*> statements = scp->getStatements();
	int s = statements.size();
	if(s>0) {
		result.append("{\n");
		++indent;
		for(int i=0; i<s; i++) {
			createIndent();
			statements.at(i)->accept(*this);
		}
		--indent;
		createIndent();
		result.append("}");
	} else
		result.append(";");

	result.append("\n");
}

void TreePrinter::visit(CompoundStatement* stmt)
{
	QVector<Statement*> children = stmt->getChildren();
	int c = children.size();
	if(c>0) {
		if(c>1) {
			result.append("{\n");
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
			result.append("}");
		}
	} else
		result.append(";");

}

void TreePrinter::visit(IfElseStatement* ifelse)
{
	result.append("if(");
	ifelse->getExpression()->accept(*this);
	result.append(")");
	Statement* trueStatement = ifelse->getTrueStatement();
	if(trueStatement)
		trueStatement->accept(*this);

	Statement* falseStatement = ifelse->getFalseStatement();
	if(falseStatement) {
		result.append("\n");
		createIndent();
		result.append("else ");
		falseStatement->accept(*this);
	}

	result.append("\n");
}

void TreePrinter::visit(ForStatement* forstmt)
{
	result.append("for(");
	foreach(Argument* a, forstmt->getArguments())
		a->accept(*this);
	result.append(")");
	Statement* statement = forstmt->getStatement();
	statement->accept(*this);

	result.append("\n");
}

void TreePrinter::visit(Parameter* param)
{
	result.append(param->getName());

	Expression* expression = param->getExpression();
	if(expression) {
		result.append("=");
		expression->accept(*this);
	}
}

void TreePrinter::visit(BinaryExpression* exp)
{
	result.append("(");
	exp->getLeft()->accept(*this);
	result.append(exp->getOpString());
	exp->getRight()->accept(*this);
	result.append(")");
}

void TreePrinter::visit(Argument* arg)
{
	Variable* variable = arg->getVariable();
	if(variable) {
		variable->accept(*this);
		result.append("=");
	}

	arg->getExpression()->accept(*this);
}

void TreePrinter::visit(AssignStatement* stmt)
{
	Variable* var = stmt->getVariable();
	if(var)
		var->accept(*this);

	switch(stmt->getOperation()) {
	case Expression::Increment:
		result.append("++");
		break;
	case Expression::Decrement:
		result.append("--");
		break;
	default: {
		result.append("=");
		Expression* expression = stmt->getExpression();
		if(expression)
			expression->accept(*this);
	}
	}

	result.append(";\n");
}

void TreePrinter::visit(VectorExpression* exp)
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

void TreePrinter::visit(RangeExpression* exp)
{
	result.append("[");
	exp->getStart()->accept(*this);

	result.append(":");
	Expression* step = exp->getStep();
	if(step) {
		step->accept(*this);
		result.append(":");
	}

	exp->getFinish()->accept(*this);
	result.append("]");
}

void TreePrinter::visit(UnaryExpression* exp)
{
	QString op = exp->getOpString();
	bool p = exp->postFix();
	if(!p)
		result.append(op);
	exp->getExpression()->accept(*this);
	if(p)
		result.append(op);
}

void TreePrinter::visit(ReturnStatement* stmt)
{
	result.append("return ");
	stmt->getExpression()->accept(*this);
	result.append(";\n");
}

void TreePrinter::visit(TernaryExpression* exp)
{
	result.append("(");
	exp->getCondition()->accept(*this);
	result.append("?");
	exp->getTrueExpression()->accept(*this);
	result.append(":");
	exp->getFalseExpression()->accept(*this);
	result.append(")");
}

void TreePrinter::visit(Invocation* stmt)
{
	QString nameSpace = stmt->getNamespace();
	if(!nameSpace.isEmpty()) {
		result.append(nameSpace);
		result.append("::");
	}
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

void TreePrinter::visit(ModuleImport* decl)
{
	result.append("import <");
	result.append(decl->getImport());
	result.append(">");
	QString name = decl->getName();
	if(!name.isEmpty()) {
		result.append(" as ");
		result.append(name);
	}
	QVector<Parameter*> parameters = decl->getParameters();
	int s = parameters.size();
	if(s>0) {
		result.append("(");
		for(int i=0; i<s; i++) {
			parameters.at(i)->accept(*this);
			if(i+1<s)
				result.append(",");
		}

		result.append(")");
	}
	result.append(";\n");
}

void TreePrinter::visit(ScriptImport* decl)
{
	result.append("use <");
	result.append(decl->getImport());
	result.append(">");
	QString name = decl->getNamespace();
	if(!name.isEmpty()) {
		result.append(" as ");
		result.append(name);
		result.append(";");
	}
	result.append("\n");
}

void TreePrinter::visit(Literal* lit)
{
	result.append(lit->getValueString());
}

void TreePrinter::visit(Variable* var)
{
	switch(var->getType()) {
	case Variable::Const:
		result.append("const ");
		break;
	case Variable::Param:
		result.append("param ");
		break;
	default:
		break;
	}

	if(var->getType()==Variable::Special)
		result.append("$");
	result.append(var->getName());
}

void TreePrinter::visit(Script* sc)
{
	foreach(Declaration* d, sc->getDeclarations())
		d->accept(*this);

	printf("%s",result.toLocal8Bit().constData());
}
