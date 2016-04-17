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

#include "treeevaluator.h"
#include "vectorvalue.h"
#include "complexvalue.h"
#include "rangevalue.h"
#include "builtincreator.h"
#include "module/importmodule.h"
#include "syntaxtreebuilder.h"
#include "module/unionmodule.h"

TreeEvaluator::TreeEvaluator(Reporter* r)
{
	reporter=r;
	context=NULL;
	rootNode=NULL;
	layout=NULL;
	descendDone=false;
}

TreeEvaluator::~TreeEvaluator()
{
	Value::cleanup();
	foreach(Layout* l, scopeLookup.values())
		delete l;
}

void TreeEvaluator::startLayout(Scope* scp)
{
	Layout* parent=layout;
	layout=new Layout(reporter);
	layout->setParent(parent);
	layout->setScope(scp);
	scopeLookup.insert(scp,layout);
	layoutStack.push(layout);
}

void TreeEvaluator::finishLayout()
{
	layoutStack.pop();
	layout=layoutStack.top();
}

void TreeEvaluator::startContext(Scope* scp)
{
	Context* parent = context;
	context = new Context();
	context->setParent(parent);
	context->setCurrentScope(scp);
	contextStack.push(context);
}

void TreeEvaluator::finishContext()
{
	delete context;
	contextStack.pop();
	context=contextStack.top();
}

void TreeEvaluator::visit(ModuleScope* scp)
{
	context->setVariablesFromArguments();
	context->clearArguments();
	context->clearParameters();

	foreach(Declaration* d, scp->getDeclarations()) {
		d->accept(*this);
	}

	if(context->getReturnValue())
		reporter->reportWarning(tr("return statement not valid inside module scope."));
}

void TreeEvaluator::visit(Instance* inst)
{
	QString name = inst->getName();
	bool aux=(inst->getType()==Instance::Auxilary);

	/* The first step for module invocations is to evaluate all the children if
	 * there are any, we do this in a seperate context because children can
	 * have children */
	Scope* c=context->getCurrentScope();
	QList<Node*> childnodes;
	QList <Statement*> stmts = inst->getChildren();
	if(stmts.size()>0) {
		startContext(c);

		foreach(Statement* s,stmts) {
			s->accept(*this);
		}

		childnodes=context->getCurrentNodes();
		finishContext();
	}

	/* Process the arguments. Arguments can themselves contain references to
	 * function calls so evaluate them in a different context */
	startContext(c);
	foreach(Argument* arg, inst->getArguments())
		arg->accept(*this);
	QList<Value*> arguments=context->getArguments();
	finishContext();

	/* Look up the layout which is currently in scope and then lookup the
	 * module in that layout */
	Layout* l=scopeLookup.value(c);
	Module* mod = l->lookupModule(name,aux);
	if(mod) {
		/* Now we need to create a context for the module itself, if we are
		 * invoking a built in module we have to use the current scope to
		 * initalise the context */
		Scope* scp = mod->getScope();
		if(scp)
			startContext(scp);
		else
			startContext(c);

		context->setInputNodes(childnodes);

		/* Pull the arguments in that we evaluated previously into this
		 * context */
		foreach(Value* a, arguments)
			context->addArgument(a);

		foreach(Parameter* p, mod->getParameters())
			p->accept(*this);

		/* Invoke the module whether it be a user defined module or a build
		 * in module */
		Node* node=NULL;
		if(scp) {
			scp->accept(*this);
			childnodes=context->getCurrentNodes();
			node=UnionModule::createUnion(childnodes);
		} else {
			node=mod->evaluate(context);
		}

		finishContext();
		if(node)
			context->addCurrentNode(node);

	} else {
		reporter->reportWarning(tr("cannot find module '%1%2'").arg(name).arg(aux?"$":""));
	}
}

void TreeEvaluator::visit(Module* mod)
{
	if(descendDone)
		return;

	layout->addModule(mod);

	Scope* scp=mod->getScope();
	if(scp) {
		startLayout(scp);
		descend(scp);
		finishLayout();
	}

}

void TreeEvaluator::visit(Function* func)
{
	if(descendDone)
		return;

	layout->addFunction(func);
	Scope* scp=func->getScope();
	if(scp) {
		startLayout(scp);
		/* Functions cannot have nested functions,
		 * so no need to descend */
		finishLayout();
	}
}

void TreeEvaluator::descend(Scope* scp)
{
	foreach(Declaration* d, scp->getDeclarations()) {
		ScriptImport* i=dynamic_cast<ScriptImport*>(d);
		if(i)
			i->accept(*this);
		Module* m = dynamic_cast<Module*>(d);
		if(m)
			m->accept(*this);
		Function* f = dynamic_cast<Function*>(d);
		if(f)
			f->accept(*this);
	}
}

void TreeEvaluator::visit(FunctionScope* scp)
{
	context->setVariablesFromArguments();
	context->clearArguments();
	context->clearParameters();

	Expression* e=scp->getExpression();
	if(e) {
		e->accept(*this);
		context->setReturnValue(context->getCurrentValue());
	} else {
		foreach(Statement* s, scp->getStatements()) {
			s->accept(*this);
			if(context->getReturnValue())
				break;
		}
	}
}

void TreeEvaluator::visit(CompoundStatement* stmt)
{
	foreach(Statement* s, stmt->getChildren())
		s->accept(*this);
}

void TreeEvaluator::visit(IfElseStatement* ifelse)
{
	ifelse->getExpression()->accept(*this);
	Value* v=context->getCurrentValue();
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
	QList<Value*> args=context->getArguments();
	context->clearArguments();

	if(!args.isEmpty()) {
		//TODO for now just consider the first arg.
		Value* first = args.at(0);

		Iterator<Value*>* i = first->createIterator();
		for(i->first(); !i->isDone(); i->next()) {

			Value* v = i->currentItem();
			v->setName(first->getName());
			context->setVariable(v);

			forstmt->getStatement()->accept(*this);

		}
		delete i;
	}
}

void TreeEvaluator::visit(Parameter* param)
{
	QString name = param->getName();

	Value* v;
	Expression* e = param->getExpression();
	if(e) {
		e->accept(*this);
		v = context->getCurrentValue();
	} else {
		v = Value::undefined();
	}

	v->setName(name);
	context->addParameter(v);
}

void TreeEvaluator::visit(BinaryExpression* exp)
{
	exp->getLeft()->accept(*this);
	Value* left=context->getCurrentValue();

	bool shortc=false;
	Expression::Operator_e op=exp->getOp();

	switch(op) {
	case Expression::LogicalAnd:
		shortc=!left->isTrue();
		break;
	case Expression::LogicalOr:
		shortc=left->isTrue();
		break;
	default:
		break;
	}

	Value* result;
	if(shortc) {
		result=left;
	} else {
		exp->getRight()->accept(*this);
		Value* right=context->getCurrentValue();
		result=Value::operation(left,op,right);
	}

	context->setCurrentValue(result);
}

void TreeEvaluator::visit(Argument* arg)
{
	QString name;
	Variable::Storage_e c=Variable::Var;
	Variable* var = arg->getVariable();
	if(var) {
		var->accept(*this);
		name=context->getCurrentName();
		c=var->getStorage();
	}

	arg->getExpression()->accept(*this);
	Value* v = context->getCurrentValue();
	v->setName(name);
	v->setStorage(c); //TODO Investigate moving this to apply to all variables.
	context->addArgument(v);
}

void TreeEvaluator::visit(AssignStatement* stmt)
{
	stmt->getVariable()->accept(*this);
	QString name = context->getCurrentName();

	Value* lvalue = context->getCurrentValue();

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
			result = context->getCurrentValue();
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
	Variable::Storage_e c;
	c=lvalue->getStorage();
	result->setStorage(c);
	switch(c) {
	case Variable::Const:
		if(!context->addVariable(result))
			reporter->reportWarning(tr("attempt to alter constant variable '%1'").arg(name));
		break;
	case Variable::Param:
		if(!context->addVariable(result))
			reporter->reportWarning(tr("attempt to alter parametric variable '%1'").arg(name));
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
		childvalues.append(context->getCurrentValue());
	}
	int commas=exp->getAdditionalCommas();
	if(commas>0)
		reporter->reportWarning(tr("%1 additional comma(s) found at the end of vector expression").arg(commas));

	Value* v = new VectorValue(childvalues);
	context->setCurrentValue(v);
}

void TreeEvaluator::visit(RangeExpression* exp)
{
	exp->getStart()->accept(*this);
	Value* start = context->getCurrentValue();

	Value* increment = NULL;
	Expression* step = exp->getStep();
	if(step) {
		step->accept(*this);
		increment=context->getCurrentValue();
	}

	exp->getFinish()->accept(*this);
	Value* finish=context->getCurrentValue();

	Value* result = new RangeValue(start,increment,finish);
	context->setCurrentValue(result);
}

void TreeEvaluator::visit(UnaryExpression* exp)
{
	exp->getExpression()->accept(*this);
	Value* left=context->getCurrentValue();

	Value* result = Value::operation(left,exp->getOp());

	context->setCurrentValue(result);
}

void TreeEvaluator::visit(ReturnStatement* stmt)
{
	Expression* e = stmt->getExpression();
	e->accept(*this);
	context->setReturnValue(context->getCurrentValue());
}

void TreeEvaluator::visit(TernaryExpression* exp)
{
	exp->getCondition()->accept(*this);
	Value* v = context->getCurrentValue();
	if(v->isTrue())
		exp->getTrueExpression()->accept(*this);
	else
		exp->getFalseExpression()->accept(*this);

}

void TreeEvaluator::visit(Invocation* stmt)
{
	QString name = stmt->getName();

	Scope* c=context->getCurrentScope();
	/* Process the arguments first. Arguments can themselves contain references
	 * to function calls so evaluate them in a different context */
	startContext(c);
	foreach(Argument* arg, stmt->getArguments())
		arg->accept(*this);
	QList<Value*> arguments=context->getArguments();
	finishContext();

	Value* result=NULL;
	/* Look up the layout which is currently in scope and then lookup the
	 * function in that layout */
	Layout* l = scopeLookup.value(c);
	Function* func = l->lookupFunction(name);
	if(func) {

		/* Now we need to create a context for the function itself, if we are
		 * invoking a built in function we have to use the current scope to
		 * initalise the context */
		Scope* scp = func->getScope();
		if(scp)
			startContext(scp);
		else
			startContext(c);

		/* Pull the arguments in that we evaluated previously into this
		 * context */
		foreach(Value* a, arguments)
			context->addArgument(a);

		foreach(Parameter* p, func->getParameters())
			p->accept(*this);

		/* Invoke the function whether it be a user defined function or a build
		 * in function */
		if(scp) {
			scp->accept(*this);
			result=context->getReturnValue();
		} else {
			result=func->evaluate(context);
		}

		finishContext();

	} else {
		reporter->reportWarning(tr("cannot find function '%1'").arg(name));
	}

	if(!result)
		result=Value::undefined();

	context->setCurrentValue(result);
}

void TreeEvaluator::visit(Callback* c)
{
	Expression* e = c->getExpression();
	e->accept(*this);
	c->setResult(context->getCurrentValue());
}

void TreeEvaluator::visit(ModuleImport* imp)
{
	ImportModule* mod=new ImportModule();
	mod->setImport(imp->getImport());
	mod->setName(imp->getName());
	//TODO global import args.

	/* Adding the import module to the current layout is ok here because we
	 * assume import statements are at the top level, and that we will be at
	 * the top level at this point */
	layout->addModule(mod);
}

void TreeEvaluator::visit(ScriptImport* sc)
{
	if(descendDone)
		return;

	QString imp=sc->getImport();
	QFileInfo* f;
	if(!importLocations.isEmpty())
		f=new QFileInfo(importLocations.top()->absoluteDir(),imp);
	else
		f=new QFileInfo(imp); /* relative to working dir */

	Script* s=parse(f->absoluteFilePath(),reporter,true);
	imports.append(s);
	/* Now recursively descend any modules functions or script imports within
	 * the imported script and add them to the main script */
	importLocations.push(f);
	descend(s);
	delete importLocations.pop();
}

void TreeEvaluator::visit(Literal* lit)
{
	Value* v= lit->getValue();

	context->setCurrentValue(v);
}

void TreeEvaluator::visit(Variable* var)
{
	QString name = var->getName();
	Variable::Storage_e oldStorage=var->getStorage();
	Variable::Storage_e currentStorage=oldStorage;
	Layout* l=scopeLookup.value(context->getCurrentScope());
	Value* v=context->lookupVariable(name,currentStorage,l);
	if(currentStorage!=oldStorage)
		switch(oldStorage) {
		case Variable::Const:
			reporter->reportWarning(tr("attempt to make previously non-constant variable '%1' constant").arg(name));
			break;
		case Variable::Param:
			reporter->reportWarning(tr("attempt to make previously non-parametric variable '%1' parametric").arg(name));
			break;
		default:
			break;
		}

	context->setCurrentValue(v);
	context->setCurrentName(name);
}

void TreeEvaluator::visit(CodeDoc*)
{
}

void TreeEvaluator::visit(Script* sc)
{
	BuiltinCreator* b=BuiltinCreator::getInstance(reporter);
	b->initBuiltins(sc);

	/* Use the location of the current script as the root for all imports */
	QFileInfo* info=sc->getFileLocation();
	if(info)
		importLocations.push(info);

	descendDone=false;
	startLayout(sc);
	descend(sc);
	descendDone=true;

	startContext(sc);
	foreach(Declaration* d, sc->getDeclarations()) {
		d->accept(*this);
	}
	QList<Node*> childnodes=context->getCurrentNodes();

	if(context->getReturnValue())
		reporter->reportWarning(tr("return statement not valid inside global scope."));

	rootNode=UnionModule::createUnion(childnodes);

	/* Clean up all the imported scripts as its not the responsibility of the
	 * caller to do so as we created the imported script instances within this
	 * evaluator */
	foreach(Script* sc, imports)
		delete sc;

	b->saveBuiltins(sc);
}

void TreeEvaluator::visit(Product* p)
{
	Node* r=p->evaluate(context);
	QList<Node*> childnodes=context->getCurrentNodes();
	childnodes.append(r);
	context->setCurrentNodes(childnodes);
}

void TreeEvaluator::visit(ComplexExpression* exp)
{
	Expression* real=exp->getReal();
	real->accept(*this);
	Value* result=context->getCurrentValue();

	VectorExpression* imaginary=exp->getImaginary();
	QList<Value*> childvalues;
	foreach(Expression* e, imaginary->getChildren()) {
		e->accept(*this);
		childvalues.append(context->getCurrentValue());
	}
	Value* v = new ComplexValue(result,childvalues);
	context->setCurrentValue(v);
}

Node* TreeEvaluator::getRootNode() const
{
	return rootNode;
}
