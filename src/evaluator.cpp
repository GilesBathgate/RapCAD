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

#include "evaluator.h"
#include "echomodule.h"
#include "vectorvalue.h"
#include <stdio.h>

Evaluator::Evaluator()
{
	context=NULL;
	startContext();
}

Evaluator::~Evaluator()
{
}

void Evaluator::startContext()
{
	Context* parent = context;
	context = new Context();
	context->parent = parent;
	contextStack.push(context);
}

void Evaluator::finishContext()
{
	contextStack.pop();
	context=contextStack.top();
}

void Evaluator::visit(ModuleScope* scp)
{
	QVector<Value*> arguments = context->arguments;
	QVector<Value*> parameters = context->parameters;

	startContext();
	context->currentScope = scp;

	context->setArguments(arguments,parameters);

	foreach(Declaration* d, scp->getDeclarations())
		d->accept(*this);

	if(context->returnValue)
		printf("Warning: return statement not valid inside module scope.\n");

	finishContext();
}

void Evaluator::visit(Instance* inst)
{
	QString name = inst->getName();
	Module* mod = context->lookupModule(name);
	if(mod) {
		foreach(Argument* arg, inst->getArguments())
			arg->accept(*this);

		foreach(Parameter* p, mod->getParameters())
			p->accept(*this);

		Scope* scp = mod->getScope();
		if(scp)
			scp->accept(*this);
		else
			mod->evaluate(context,inst);

		context->arguments.clear();
		context->parameters.clear();
	}
}

void Evaluator::visit(Module* mod)
{
	context->addModule(mod);
}

void Evaluator::visit(Function* func)
{
	context->addFunction(func);
}

void Evaluator::visit(FunctionScope* scp)
{
	QVector<Value*> arguments = context->arguments;
	QVector<Value*> parameters = context->parameters;

	startContext();
	context->currentScope = scp;

	context->setArguments(arguments,parameters);

	Expression* e=scp->getExpression();
	if(e) {
		e->accept(*this);
		context->returnValue = context->currentValue;
	} else {
		foreach(Statement* s, scp->getStatements()) {
			s->accept(*this);
			if(context->returnValue)
				break;
		}
	}

	//"pop" our return value
	Value* v = context->returnValue;
	if(!v)
		v=new Value();

	finishContext();
	context->currentValue=v;
}

void Evaluator::visit(CompoundStatement* stmt)
{
	foreach(Statement* s, stmt->getChildren())
		s->accept(*this);
}

void Evaluator::visit(IfElseStatement* ifelse)
{
}

void Evaluator::visit(ForStatement* forstmt)
{
}

void Evaluator::visit(Parameter* param)
{
	QString name = param->getName();

	Value* v;
	Expression* e = param->getExpression();
	if(e) {
		e->accept(*this);
		v = context->currentValue;
	} else {
		v = new Value();
	}

	v->setName(name);
	context->parameters.append(v);
}

void Evaluator::visit(BinaryExpression* exp)
{
	exp->getLeft()->accept(*this);
	Value* left=context->currentValue;

	exp->getRight()->accept(*this);
	Value* right=context->currentValue;

	Value* result = Value::operation(left,exp->getOp(),right);

	context->currentValue=result;
}

void Evaluator::visit(Argument* arg)
{
	QString name;
	Variable* var = arg->getVariable();
	if(var) {
		var->accept(*this);
		name=context->currentName;
	} else {
		name="";
	}

	arg->getExpression()->accept(*this);
	Value* v = context->currentValue;

	v->setName(name);
	context->arguments.append(v);
}

void Evaluator::visit(AssignStatement* stmt)
{
	stmt->getVariable()->accept(*this);
	QString name = context->currentName;

	stmt->getExpression()->accept(*this);
	Value* v = context->currentValue;

	v->setName(name);
	context->addVariable(v);
}

void Evaluator::visit(VectorExpression* exp)
{
	QVector<Value*> childvalues;
	foreach(Expression* e, exp->getChildren()) {
		e->accept(*this);
		childvalues.append(context->currentValue);
	}

	Value* v = new VectorValue(childvalues);
	context->currentValue=v;
}

void Evaluator::visit(RangeExpression* exp)
{
}

void Evaluator::visit(UnaryExpression* exp)
{
	exp->getExpression()->accept(*this);
	Value* left=context->currentValue;

	Value* result = Value::operation(left,exp->getOp());

	context->currentValue=result;
}

void Evaluator::visit(ReturnStatement* stmt)
{
	Expression* e = stmt->getExpression();
	e->accept(*this);
	context->returnValue = context->currentValue;
}

void Evaluator::visit(TernaryExpression* exp)
{
}

void Evaluator::visit(Invocation* stmt)
{
	QString name = stmt->getName();
	Function* func = context->lookupFunction(name);
	if(func) {
		foreach(Argument* arg, stmt->getArguments())
			arg->accept(*this);

		foreach(Parameter* p, func->getParameters())
			p->accept(*this);

		Scope* scp = func->getScope();
		if(scp)
			scp->accept(*this);

		context->arguments.clear();
		context->parameters.clear();
	}
}

void Evaluator::visit(ModuleImport* decl)
{
}

void Evaluator::visit(Literal* lit)
{
	Value* v= lit->getValue();

	context->currentValue=v;
}

void Evaluator::visit(Variable* var)
{
	QString name = var->getName();
	Value* v=context->lookupVariable(name);

	context->currentValue=v;
	context->currentName=name;
}

void Evaluator::visit(Script* sc)
{
	//TODO add our "builtin" here for now
	sc->addDeclaration(new EchoModule());

	context->currentScope = sc;
	foreach(Declaration* d, sc->getDeclarations())
		d->accept(*this);

	if(context->returnValue)
		printf("Warning: return statement not valid inside global scope.\n");
}
