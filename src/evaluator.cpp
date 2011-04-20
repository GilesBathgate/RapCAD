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
#include "vectorvalue.h"
#include "rangevalue.h"

#include "echomodule.h"
#include "cubemodule.h"
#include "cylindermodule.h"
#include "polyhedronmodule.h"
#include "differencemodule.h"
#include "unionmodule.h"
#include "intersectionmodule.h"

#include "unionnode.h"

Evaluator::Evaluator(QTextStream& s) : output(s)
{
	context=NULL;
	rootNode=NULL;
}

Evaluator::~Evaluator()
{
}

void Evaluator::startContext(Scope* scp)
{
	Context* parent = context;
	context = new Context();
	context->parent = parent;
	context->currentScope=scp;
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

	startContext(scp);

	context->setArguments(arguments,parameters);

	QVector<Node*> childnodes;
	foreach(Declaration* d, scp->getDeclarations()) {
		context->currentNode=NULL;
		d->accept(*this);
		if(context->currentNode)
			childnodes.append(context->currentNode);
	}

	if(context->returnValue)
		output << "Warning: return statement not valid inside module scope.\n";

	finishContext();

	createUnion(childnodes);
}

void Evaluator::visit(Instance* inst)
{
	QString name = inst->getName();

	startContext(context->currentScope);

	QVector<Node*> childnodes;
	foreach(Statement* s, inst->getChildren()) {
		context->currentNode=NULL;
		s->accept(*this);
		if(context->currentNode)
			childnodes.append(context->currentNode);
	}
	finishContext();

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
			context->currentNode = mod->evaluate(context,childnodes);

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

	startContext(scp);

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
	ifelse->getExpression()->accept(*this);
	Value* v = context->currentValue;
	if(v->isTrue())
		ifelse->getTrueStatement()->accept(*this);
	else
		ifelse->getFalseStatement()->accept(*this);
}

void Evaluator::visit(ForStatement* forstmt)
{
	foreach(Argument* arg, forstmt->getArguments())
		arg->accept(*this);

	//TODO for now just consider the first arg.
	Value* first = context->arguments.at(0);
	startContext(context->currentScope);

	Iterator<Value*>* i = first->createIterator();
	for(i->first(); !i->isDone(); i->next()) {

		Value* v = i->currentItem();
		v->setName(first->getName());
		context->addVariable(v);

		forstmt->getStatement()->accept(*this);

	}
	delete i;

	finishContext();

	context->arguments.clear();
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

	Value* result;
	switch(stmt->getOperation()) {
	case Expression::Increment: {
		Value* v = context->lookupVariable(name);
		result=Value::operation(v,Expression::Increment);
		break;
	}
	case Expression::Decrement: {
		Value* v = context->lookupVariable(name);
		result=Value::operation(v,Expression::Decrement);
		break;
	}
	default: {
		Expression* expression = stmt->getExpression();
		if(expression) {
			expression->accept(*this);
			result = context->currentValue;
		}
	}
	}

	result->setName(name);
	context->addVariable(result);
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
	exp->getStart()->accept(*this);
	Value* start = context->currentValue;

	Value* increment = NULL;
	Expression* step = exp->getStep();
	if(step) {
		step->accept(*this);
		increment=context->currentValue;
	}

	exp->getFinish()->accept(*this);
	Value* finish=context->currentValue;

	Value* result = new RangeValue(start,increment,finish);
	context->currentValue=result;
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
	exp->getCondition()->accept(*this);
	Value* v = context->currentValue;
	if(v->isTrue())
		exp->getTrueExpression()->accept(*this);
	else
		exp->getFalseExpression()->accept(*this);

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

void Evaluator::visit(ModuleImport*)
{
	//TODO
}

void Evaluator::visit(ScriptImport*)
{
	//TODO
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

void Evaluator::createUnion(QVector<Node*> childnodes)
{
	if(childnodes.size()==1) {
		context->currentNode=childnodes.at(0);
	} else {
		UnionNode* u=new UnionNode();
		u->setChildren(childnodes);
		context->currentNode=u;
	}
}

void Evaluator::visit(Script* sc)
{
	//TODO add our "builtin" here for now
	sc->addDeclaration(new EchoModule(output));
	sc->addDeclaration(new CubeModule());
	sc->addDeclaration(new CylinderModule());
	sc->addDeclaration(new PolyhedronModule());
	sc->addDeclaration(new DifferenceModule());
	sc->addDeclaration(new UnionModule());
	sc->addDeclaration(new IntersectionModule());

	startContext(sc);
	QVector<Node*> childnodes;
	foreach(Declaration* d, sc->getDeclarations()) {
		context->currentNode=NULL;
		d->accept(*this);
		if(context->currentNode)
			childnodes.append(context->currentNode);
	}

	if(context->returnValue)
		output << "Warning: return statement not valid inside global scope.\n";

	createUnion(childnodes);
	rootNode=context->currentNode;
}

Node* Evaluator::getRootNode() const
{
	return rootNode;
}
