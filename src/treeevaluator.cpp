/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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

#include "treeevaluator.h"
#include "vectorvalue.h"
#include "rangevalue.h"
#include "node/unionnode.h"
#include "builtincreator.h"
#include "module/importmodule.h"

TreeEvaluator::TreeEvaluator(QTextStream& s) : output(s)
{
	context=NULL;
	rootNode=NULL;
}

TreeEvaluator::~TreeEvaluator()
{
	Value::cleanup();
}

void TreeEvaluator::startContext(Scope* scp)
{
	Context* parent = context;
	context = new Context(output);
	context->parent = parent;
	context->currentScope=scp;
	contextStack.push(context);
}

void TreeEvaluator::finishContext()
{
	contextStack.pop();
	context=contextStack.top();
}

void TreeEvaluator::visit(ModuleScope* scp)
{
	QList<Value*> arguments = context->arguments;
	QList<Value*> parameters = context->parameters;
	QList<Node*> childnodes = context->inputNodes;

	startContext(scp);

	context->setArguments(arguments,parameters);
	context->inputNodes=childnodes;

	foreach(Declaration* d, scp->getDeclarations()) {
		d->accept(*this);
	}

	if(context->returnValue)
		output << "Warning: return statement not valid inside module scope.\n";

	//"pop" our child nodes.
	childnodes=context->currentNodes;
	finishContext();

	Node* n=createUnion(childnodes);
	context->currentNodes.append(n);
}

void TreeEvaluator::visit(Instance* inst)
{
	QString name = inst->getName();

	QList <Statement*> stmts = inst->getChildren();
	if(stmts.size()>0) {
		startContext(context->currentScope);

		foreach(Statement* s,stmts) {
			s->accept(*this);
		}
		//"pop" our child nodes.
		QList<Node*> childnodes=context->currentNodes;
		finishContext();
		context->inputNodes=childnodes;
	}

	Module* mod = context->lookupModule(name);
	if(mod) {
		foreach(Argument* arg, inst->getArguments())
			arg->accept(*this);

		foreach(Parameter* p, mod->getParameters())
			p->accept(*this);

		Scope* scp = mod->getScope();
		if(scp) {
			scp->accept(*this);
		} else {
			Node* node=mod->evaluate(context);
			if(node)
				context->currentNodes.append(node);
		}

		context->arguments.clear();
		context->parameters.clear();
	} else {
		output << "Warning: cannot find module '" << name << "'.\n";
	}
}

void TreeEvaluator::visit(Module* mod)
{
	context->addModule(mod);
}

void TreeEvaluator::visit(Function* func)
{
	context->addFunction(func);
}

void TreeEvaluator::visit(FunctionScope* scp)
{
	QList<Value*> arguments = context->arguments;
	QList<Value*> parameters = context->parameters;

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

void TreeEvaluator::visit(CompoundStatement* stmt)
{
	foreach(Statement* s, stmt->getChildren())
		s->accept(*this);
}

void TreeEvaluator::visit(IfElseStatement* ifelse)
{
	ifelse->getExpression()->accept(*this);
	Value* v=context->currentValue;
	if(v->isTrue()) {
		ifelse->getTrueStatement()->accept(*this);
	} else {
		Statement* falseStmt=ifelse->getFalseStatement();
		if(falseStmt)
			falseStmt->accept(*this);
	}
}

void TreeEvaluator::visit(ForStatement* forstmt)
{
	foreach(Argument* arg, forstmt->getArguments())
		arg->accept(*this);
	if(context->arguments.count()>0) {
		//TODO for now just consider the first arg.
		Value* first = context->arguments.at(0);
		context->arguments.clear();

		Iterator<Value*>* i = first->createIterator();
		for(i->first(); !i->isDone(); i->next()) {

			Value* v = i->currentItem();
			v->setName(first->getName());
			context->setVariable(v);

			forstmt->getStatement()->accept(*this);

		}
		delete i;
	} else {
		forstmt->getStatement()->accept(*this);
	}
}

void TreeEvaluator::visit(Parameter* param)
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

void TreeEvaluator::visit(BinaryExpression* exp)
{
	exp->getLeft()->accept(*this);
	Value* left=context->currentValue;

	exp->getRight()->accept(*this);
	Value* right=context->currentValue;

	Value* result = Value::operation(left,exp->getOp(),right);

	context->currentValue=result;
}

void TreeEvaluator::visit(Argument* arg)
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

void TreeEvaluator::visit(AssignStatement* stmt)
{
	stmt->getVariable()->accept(*this);
	QString name = context->currentName;

	Value* lvalue = context->currentValue;

	Value* result=NULL;
	Expression::Operator_e op=stmt->getOperation();
	switch(op) {
	case Expression::Increment:
	case Expression::Decrement:
		break;
	default: {
		Expression* expression = stmt->getExpression();
		if(expression) {
			expression->accept(*this);
			result = context->currentValue;
		}
	}
	}

	switch(op) {
	case Expression::Append:
	case Expression::AddAssign:
	case Expression::SubAssign: {
		result=Value::operation(lvalue,op,result);
		break;
	}
	case Expression::Increment: {
		result=Value::operation(lvalue,op);
		break;
	}
	case Expression::Decrement: {
		result=Value::operation(lvalue,op);
		break;
	}
	default:
		break;
	}

	result->setName(name);
	result->setType(lvalue->getType());
	switch(lvalue->getType()) {
	case Variable::Const:
		if(!context->addVariable(result))
			output << "Warning: Attempt to alter constant variable '" << name << "'\n";
		break;
	case Variable::Param:
		if(!context->addVariable(result))
			output << "Warning: Attempt to alter parametric variable '" << name << "'\n";
		break;
	default:
		context->setVariable(result);
		break;
	}
}

void TreeEvaluator::visit(VectorExpression* exp)
{
	QList<Value*> childvalues;
	foreach(Expression* e, exp->getChildren()) {
		e->accept(*this);
		childvalues.append(context->currentValue);
	}
	int commas=exp->getAdditionalCommas();
	if(commas>0)
		output << "Warning: " << commas << " additional comma(s) found at the end of vector expression.\n";

	Value* v = new VectorValue(childvalues);
	context->currentValue=v;
}

void TreeEvaluator::visit(RangeExpression* exp)
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

void TreeEvaluator::visit(UnaryExpression* exp)
{
	exp->getExpression()->accept(*this);
	Value* left=context->currentValue;

	Value* result = Value::operation(left,exp->getOp());

	context->currentValue=result;
}

void TreeEvaluator::visit(ReturnStatement* stmt)
{
	Expression* e = stmt->getExpression();
	e->accept(*this);
	context->returnValue = context->currentValue;
}

void TreeEvaluator::visit(TernaryExpression* exp)
{
	exp->getCondition()->accept(*this);
	Value* v = context->currentValue;
	if(v->isTrue())
		exp->getTrueExpression()->accept(*this);
	else
		exp->getFalseExpression()->accept(*this);

}

void TreeEvaluator::visit(Invocation* stmt)
{
	QString name = stmt->getName();
	Function* func = context->lookupFunction(name);
	if(func) {
		foreach(Argument* arg, stmt->getArguments())
			arg->accept(*this);

		foreach(Parameter* p, func->getParameters())
			p->accept(*this);

		Scope* scp = func->getScope();
		if(scp) {
			scp->accept(*this);
		} else {
			Value* result=func->evaluate(context);
			if(result)
				context->currentValue=result;
		}

		context->arguments.clear();
		context->parameters.clear();
	} else {
		output << "Warning: cannot find function '" << name << "'.\n";
	}
}

void TreeEvaluator::visit(ModuleImport* imp)
{
	ImportModule* mod=new ImportModule();
	mod->setImport(imp->getImport());
	mod->setName(imp->getName());
	//TODO global import args.
	context->addModule(mod);
}

void TreeEvaluator::visit(ScriptImport*)
{
	//TODO
}

void TreeEvaluator::visit(Literal* lit)
{
	Value* v= lit->getValue();

	context->currentValue=v;
}

void TreeEvaluator::visit(Variable* var)
{
	QString name = var->getName();
	Variable::Type_e oldType=var->getType();
	Variable::Type_e type=oldType;
	Value* v=context->lookupVariable(name,type);
	if(type!=oldType)
		switch(oldType) {
		case Variable::Const:
			output << "Warning: Attempt to make previously non-constant variable '" << name << "' constant\n";
			break;
		case Variable::Param:
			output << "Warning: Attempt to make previously non-parametric variable '" << name << "' parametric\n";
			break;
		default:
			break;
		}

	context->currentValue=v;
	context->currentName=name;
}

Node* TreeEvaluator::createUnion(QList<Node*> childnodes)
{
	if(childnodes.size()==1) {
		return childnodes.at(0);
	} else {
		UnionNode* u=new UnionNode();
		u->setChildren(childnodes);
		return u;
	}
}

void TreeEvaluator::visit(CodeDoc*)
{
}

void TreeEvaluator::visit(Script* sc)
{
	BuiltinCreator* b=BuiltinCreator::getInstance(output);
	b->initBuiltins(sc);

	startContext(sc);
	foreach(Declaration* d, sc->getDeclarations()) {
		d->accept(*this);
	}
	QList<Node*> childnodes=context->currentNodes;

	if(context->returnValue)
		output << "Warning: return statement not valid inside global scope.\n";

	rootNode=createUnion(childnodes);

	b->saveBuiltins(sc);
}

Node* TreeEvaluator::getRootNode() const
{
	return rootNode;
}
