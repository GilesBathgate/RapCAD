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

#include "treeevaluator.h"
#include "vectorvalue.h"
#include "complexvalue.h"
#include "rangevalue.h"
#include "valueiterator.h"
#include "builtincreator.h"
#include "module/importmodule.h"
#include "syntaxtreebuilder.h"
#include "module/unionmodule.h"

TreeEvaluator::TreeEvaluator(Reporter& r) :
	reporter(r),
	context(nullptr),
	layout(nullptr),
	descendDone(false),
	rootNode(nullptr)
{
}

TreeEvaluator::~TreeEvaluator()
{
	Value::cleanup();
	for(Layout* l: scopeLookup.values())
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

void TreeEvaluator::visit(const ModuleScope& scp)
{
	context->setVariablesFromArguments();
	context->clearArguments();
	context->clearParameters();

	for(Declaration* d: scp.getDeclarations()) {
		d->accept(*this);
	}

	if(context->getReturnValue())
		reporter.reportWarning(tr("return statement not valid inside module scope."));
}

void TreeEvaluator::visit(const Instance& inst)
{
	QString name = inst.getName();
	bool aux=(inst.getType()==Instance::Auxilary);

	/* The first step for module invocations is to evaluate all the children if
	 * there are any, we do this in a seperate context because children can
	 * have children */
	Scope* c=context->getCurrentScope();
	QList<Node*> childnodes;
	QList <Statement*> stmts = inst.getChildren();
	if(stmts.size()>0) {
		startContext(c);

		for(Statement* s: stmts) {
			s->accept(*this);
		}

		childnodes=context->getCurrentNodes();
		finishContext();
	}

	/* Process the arguments. Arguments can themselves contain references to
	 * function calls so evaluate them in a different context */
	startContext(c);
	for(Argument* arg: inst.getArguments())
		arg->accept(*this);
	auto arguments=context->getArguments();
	finishContext();

	/* Look up the layout which is currently in scope and then lookup the
	 * module in that layout */
	Layout* l=scopeLookup.value(c);
	const Module* mod = l->lookupModule(name,aux);
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
		for(auto a: arguments)
			context->addArgument(a);

		for(Parameter* p: mod->getParameters())
			p->accept(*this);

		/* Invoke the module whether it be a user defined module or a build
		 * in module */
		Node* node=nullptr;
		if(scp) {
			scp->accept(*this);
			childnodes=context->getCurrentNodes();
			node=UnionModule::createUnion(childnodes);
		} else {
			node=mod->evaluate(*context);
		}

		finishContext();
		if(node)
			context->addCurrentNode(node);

	} else {
		reporter.reportWarning(tr("cannot find module '%1%2'").arg(name).arg(aux?"$":""));
	}
}

void TreeEvaluator::visit(const Module& mod)
{
	if(descendDone)
		return;

	layout->addModule(mod);

	Scope* scp=mod.getScope();
	if(scp) {
		startLayout(scp);
		descend(scp);
		finishLayout();
	}

}

void TreeEvaluator::visit(const Function& func)
{
	if(descendDone)
		return;

	layout->addFunction(func);
	Scope* scp=func.getScope();
	if(scp) {
		startLayout(scp);
		/* Functions cannot have nested functions,
		 * so no need to descend */
		finishLayout();
	}
}

void TreeEvaluator::descend(Scope* scp)
{
	for(Declaration* d: scp->getDeclarations()) {
		auto* i=dynamic_cast<ScriptImport*>(d);
		if(i)
			i->accept(*this);
		auto* m = dynamic_cast<Module*>(d);
		if(m)
			m->accept(*this);
		auto* f = dynamic_cast<Function*>(d);
		if(f)
			f->accept(*this);
	}
}

void TreeEvaluator::visit(const FunctionScope& scp)
{
	context->setVariablesFromArguments();
	context->clearArguments();
	context->clearParameters();

	Expression* e=scp.getExpression();
	if(e) {
		e->accept(*this);
		context->setReturnValue(context->getCurrentValue());
	} else {
		for(Statement* s: scp.getStatements()) {
			s->accept(*this);
			if(context->getReturnValue())
				break;
		}
	}
}

void TreeEvaluator::visit(const CompoundStatement& stmt)
{
	for(Statement* s: stmt.getChildren())
		s->accept(*this);
}

void TreeEvaluator::visit(const IfElseStatement& ifelse)
{
	ifelse.getExpression()->accept(*this);
	Value* v=context->getCurrentValue();
	if(v->isTrue()) {
		ifelse.getTrueStatement()->accept(*this);
	} else {
		Statement* falseStmt=ifelse.getFalseStatement();
		if(falseStmt)
			falseStmt->accept(*this);
	}
}

void TreeEvaluator::visit(const ForStatement& forstmt)
{
	for(Argument* arg: forstmt.getArguments())
		arg->accept(*this);
	auto args=context->getArguments();
	context->clearArguments();

	if(!args.isEmpty()) {
		//TODO for now just consider the first arg.
		auto firstArg = args.at(0);
		QString name=firstArg.first;
		Value* val=firstArg.second;

		ValueIterator* it=val->createIterator();
		for(Value* v: *it) {
			context->setVariable(name,v);

			forstmt.getStatement()->accept(*this);
		}
		delete it;
	}
}

void TreeEvaluator::visit(const Parameter& param)
{
	QString name = param.getName();

	Value* v;
	Expression* e = param.getExpression();
	if(e) {
		e->accept(*this);
		v = context->getCurrentValue();
	} else {
		v = Value::undefined();
	}

	context->addParameter(name,v);
}

void TreeEvaluator::visit(const BinaryExpression& exp)
{
	exp.getLeft()->accept(*this);
	Value* left=context->getCurrentValue();

	bool shortc=false;
	Expression::Operator_e op=exp.getOp();

	switch(op) {
		case Expression::LogicalAnd:
			shortc=left->isFalse();
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
		exp.getRight()->accept(*this);
		Value* right=context->getCurrentValue();
		result=Value::operation(left,op,right);
	}

	context->setCurrentValue(result);
}

void TreeEvaluator::visit(const Argument& arg)
{
	QString name;
	Variable::Storage_e c=Variable::Var;
	Variable* var = arg.getVariable();
	if(var) {
		var->accept(*this);
		name=context->getCurrentName();
		c=var->getStorage();
	}

	Expression* exp=arg.getExpression();
	if(exp) {
		exp->accept(*this);
		Value* v = context->getCurrentValue();
		v->setStorage(c); //TODO Investigate moving this to apply to all variables.
		context->addArgument(name,v);
	}
}

void TreeEvaluator::visit(const AssignStatement& stmt)
{
	stmt.getVariable()->accept(*this);
	QString name = context->getCurrentName();

	Value* lvalue = context->getCurrentValue();

	Value* result=nullptr;
	Expression::Operator_e op=stmt.getOperation();
	switch(op) {
		case Expression::Increment:
		case Expression::Decrement:
			break;
		default: {
			Expression* expression = stmt.getExpression();
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
	if(!result) return;

	Variable::Storage_e c;
	c=lvalue->getStorage();
	result->setStorage(c);
	switch(c) {
		case Variable::Const:
			if(!context->addVariable(name,result))
				reporter.reportWarning(tr("attempt to alter constant variable '%1'").arg(name));
			break;
		case Variable::Param:
			if(!context->addVariable(name,result))
				reporter.reportWarning(tr("attempt to alter parametric variable '%1'").arg(name));
			break;
		default:
			context->setVariable(name,result);
			break;
	}
}

void TreeEvaluator::visit(const VectorExpression& exp)
{
	QList<Value*> childvalues;
	for(Expression* e: exp.getChildren()) {
		e->accept(*this);
		childvalues.append(context->getCurrentValue());
	}
	int commas=exp.getAdditionalCommas();
	if(commas>0)
		reporter.reportWarning(tr("%1 additional comma(s) found at the end of vector expression").arg(commas));

	Value* v = new VectorValue(childvalues);
	context->setCurrentValue(v);
}

void TreeEvaluator::visit(const RangeExpression& exp)
{
	exp.getStart()->accept(*this);
	Value* start = context->getCurrentValue();

	Value* increment = nullptr;
	Expression* step = exp.getStep();
	if(step) {
		step->accept(*this);
		increment=context->getCurrentValue();
	}

	exp.getFinish()->accept(*this);
	Value* finish=context->getCurrentValue();

	Value* result = new RangeValue(start,increment,finish);
	context->setCurrentValue(result);
}

void TreeEvaluator::visit(const UnaryExpression& exp)
{
	exp.getExpression()->accept(*this);
	Value* left=context->getCurrentValue();

	Value* result = Value::operation(left,exp.getOp());

	context->setCurrentValue(result);
}

void TreeEvaluator::visit(const ReturnStatement& stmt)
{
	Expression* e = stmt.getExpression();
	e->accept(*this);
	context->setReturnValue(context->getCurrentValue());
}

void TreeEvaluator::visit(const TernaryExpression& exp)
{
	exp.getCondition()->accept(*this);
	Value* v = context->getCurrentValue();
	if(v->isTrue())
		exp.getTrueExpression()->accept(*this);
	else
		exp.getFalseExpression()->accept(*this);

}

void TreeEvaluator::visit(const Invocation& stmt)
{
	QString name = stmt.getName();

	Scope* c=context->getCurrentScope();
	/* Process the arguments first. Arguments can themselves contain references
	 * to function calls so evaluate them in a different context */
	startContext(c);
	for(Argument* arg: stmt.getArguments())
		arg->accept(*this);
	auto arguments=context->getArguments();
	finishContext();

	Value* result=nullptr;
	/* Look up the layout which is currently in scope and then lookup the
	 * function in that layout */
	Layout* l = scopeLookup.value(c);
	const Function* func = l->lookupFunction(name);
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
		for(auto a: arguments)
			context->addArgument(a);

		for(Parameter* p: func->getParameters())
			p->accept(*this);

		/* Invoke the function whether it be a user defined function or a build
		 * in function */
		if(scp) {
			scp->accept(*this);
			result=context->getReturnValue();
		} else {
			result=func->evaluate(*context);
		}

		finishContext();

	} else {
		reporter.reportWarning(tr("cannot find function '%1'").arg(name));
	}

	if(!result)
		result=Value::undefined();

	context->setCurrentValue(result);
}

void TreeEvaluator::visit(Callback& c)
{
	Expression* e = c.getExpression();
	e->accept(*this);
	c.setResult(context->getCurrentValue());
}

QFileInfo TreeEvaluator::getFullPath(const QString &file)
{
	if(!importLocations.isEmpty())
		return QFileInfo(importLocations.top(),file);
	else
		return QFileInfo(file); /* relative to working dir */
}

void TreeEvaluator::visit(const ModuleImport& mi)
{
	auto* mod=new ImportModule(reporter);
	QFileInfo f=getFullPath(mi.getImport());
	mod->setImport(f.absoluteFilePath());
	mod->setName(mi.getName());
	//TODO global import args.

	/* Adding the import module to the current layout is ok here because we
	 * assume import statements are at the top level, and that we will be at
	 * the top level at this point */
	layout->addModule(*mod);
}

void TreeEvaluator::visit(const ScriptImport& sc)
{
	if(descendDone)
		return;

	QFileInfo f=getFullPath(sc.getImport());
	Script* s=new Script(reporter);
	s->parse(f);
	imports.append(s);
	/* Now recursively descend any modules functions or script imports within
	 * the imported script and add them to the main script */
	QDir loc=f.absoluteDir();
	importLocations.push(loc);
	descend(s);
	importLocations.pop();
}

void TreeEvaluator::visit(const Literal& lit)
{
	Value* v= lit.getValue();

	context->setCurrentValue(v);
}

void TreeEvaluator::visit(const Variable& var)
{
	QString name = var.getName();
	Variable::Storage_e oldStorage=var.getStorage();
	Variable::Storage_e currentStorage=oldStorage;
	Layout* l=scopeLookup.value(context->getCurrentScope());
	Value* v=context->lookupVariable(name,currentStorage,l);
	if(currentStorage!=oldStorage)
		switch(oldStorage) {
			case Variable::Const:
				reporter.reportWarning(tr("attempt to make previously non-constant variable '%1' constant").arg(name));
				break;
			case Variable::Param:
				reporter.reportWarning(tr("attempt to make previously non-parametric variable '%1' parametric").arg(name));
				break;
			default:
				break;
		}

	context->setCurrentValue(v);
	context->setCurrentName(name);
}

void TreeEvaluator::visit(const CodeDoc&)
{
}

void TreeEvaluator::visit(Script& sc)
{
	BuiltinCreator* b=BuiltinCreator::getInstance(reporter);
	b->initBuiltins(sc);

	/* Use the location of the current script as the root for all imports */
	QDir loc=sc.getFileLocation();
	importLocations.push(loc);

	Script* scp=&sc;
	descendDone=false;
	startLayout(scp);
	descend(scp);
	descendDone=true;

	startContext(scp);
	for(Declaration* d: sc.getDeclarations()) {
		d->accept(*this);
	}
	QList<Node*> childnodes=context->getCurrentNodes();

	if(context->getReturnValue())
		reporter.reportWarning(tr("return statement not valid inside global scope."));

	rootNode=UnionModule::createUnion(childnodes);

	/* Clean up all the imported scripts as its not the responsibility of the
	 * caller to do so as we created the imported script instances within this
	 * evaluator */
	for(Script* sc: imports)
		delete sc;

	b->saveBuiltins(sc);
}

void TreeEvaluator::visit(Product& p)
{
	Node* r=p.evaluate(context);
	QList<Node*> childnodes=context->getCurrentNodes();
	childnodes.append(r);
	context->setCurrentNodes(childnodes);
}

void TreeEvaluator::visit(const ComplexExpression& exp)
{
	Expression* real=exp.getReal();
	real->accept(*this);
	Value* result=context->getCurrentValue();

	VectorExpression* imaginary=exp.getImaginary();
	QList<Value*> childvalues;
	for(Expression* e: imaginary->getChildren()) {
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
