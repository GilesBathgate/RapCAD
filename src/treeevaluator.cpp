/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#include "builtinmanager.h"
#include "complexvalue.h"
#include "module/unionmodule.h"
#include "rangevalue.h"
#include "valuefactory.h"
#include "valueiterator.h"
#include "vectorvalue.h"
#include <QScopedPointer>

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
	ValueFactory::getInstance().cleanupValues();
	qDeleteAll(scopeLookup);
	scopeLookup.clear();
	qDeleteAll(imports);
	imports.clear();
	qDeleteAll(modules);
	modules.clear();
	delete context;
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
	layout=layoutStack.constLast();
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
	context=contextStack.constLast();
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
	const QString& name = inst.getName();
	const bool aux=(inst.getType()==InstanceTypes::Auxilary);

	/* The first step for module invocations is to evaluate all the children if
	 * there are any, we do this in a seperate context because children can
	 * have children */
	Scope* c=context->getCurrentScope();
	QList<Node*> childnodes;
	const QList <Statement*> stmts = inst.getChildren();
	if(!stmts.empty()) {
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
	const auto arguments=context->getArguments();
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
		for(const auto& a: arguments)
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
		reporter.reportWarning(tr("cannot find module '%1%2'").arg(name,aux?"$":""));
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
		const auto& firstArg = args.at(0);
		const QString& name=firstArg.getName();
		Value* val=firstArg.getValue();

		QScopedPointer<ValueIterator> it(val->createIterator());
		for(Value& v: *it) {
			context->setVariable(name,&v);

			forstmt.getStatement()->accept(*this);
		}
	}
}

void TreeEvaluator::visit(const Parameter& param)
{
	const QString& name = param.getName();

	Value* v=nullptr;
	Expression* e = param.getExpression();
	if(e) {
		e->accept(*this);
		v = context->getCurrentValue();
	} else {
		v = &ValueFactory::createUndefined();
	}

	context->addParameter(name,v);
}

void TreeEvaluator::visit(const BinaryExpression& exp)
{
	exp.getLeft()->accept(*this);
	Value* left=context->getCurrentValue();

	bool shortc=false;
	const Operators op=exp.getOp();

	switch(op) {
		case Operators::LogicalAnd:
			shortc=left->isFalse();
			break;
		case Operators::LogicalOr:
			shortc=left->isTrue();
			break;
		default:
			break;
	}

	Value* result=nullptr;
	if(shortc) {
		result=left;
	} else {
		exp.getRight()->accept(*this);
		Value* right=context->getCurrentValue();
		result=Value::evaluate(left,op,right);
	}

	context->setCurrentValue(result);
}

void TreeEvaluator::visit(const Argument& arg)
{
	QString name;
	Storage c=Storage::Variable;
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
	const QString& name = context->getCurrentName();

	Value* lvalue = context->getCurrentValue();

	Value* result=nullptr;
	const Operators op=stmt.getOperation();
	switch(op) {
		case Operators::Increment:
		case Operators::Decrement: {
			result=Value::evaluate(lvalue,op);
			break;
		}
		default: {
			Expression* expression = stmt.getExpression();
			if(expression) {
				expression->accept(*this);
				result = context->getCurrentValue();
			}
		}
	}

	switch(op) {
		case Operators::Append:
		case Operators::AddAssign:
		case Operators::SubAssign: {
			result=Value::evaluate(lvalue,op,result);
			break;
		}
		default:
			break;
	}
	if(!result) return;

	auto c=lvalue->getStorage();
	result->setStorage(c);

	if(!context->updateVariable(name,result,c))
	{
		switch(c) {
			case Storage::Constant:
				reporter.reportWarning(tr("attempt to alter constant value '%1'").arg(name));
				break;
			case Storage::Parametric:
				reporter.reportWarning(tr("attempt to alter parametric value '%1'").arg(name));
				break;
			default:
				reporter.reportException(tr("unable to set value '%1'").arg(name));
				break;
		}
	}
}

void TreeEvaluator::visit(const VectorExpression& exp)
{
	QList<Value*> childvalues;
	for(Expression* e: exp.getChildren()) {
		e->accept(*this);
		childvalues.append(context->getCurrentValue());
	}
	const int commas=exp.getAdditionalCommas();
	if(commas>0)
		reporter.reportWarning(tr("%1 additional comma(s) found at the end of vector expression").arg(commas));

	Value& v = ValueFactory::createVector(childvalues);
	context->setCurrentValue(&v);
}

void TreeEvaluator::visit(const IntervalExpression& inv)
{
	auto& n = ValueFactory::createNumber(inv.getValue());

	inv.getMore()->accept(*this);
	Value* more = context->getCurrentValue();

	Expression* exp = inv.getLess();
	Value* less;
	if(exp) {
		exp->accept(*this);
		less = context->getCurrentValue();
	} else {
		less = more; // less is more ;)
	}

	Value& lower = n - (*less);
	Value& upper = n + (*more);
	Value& result = ValueFactory::createInterval(lower,upper);
	context->setCurrentValue(&result);
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

	if(increment) {
		Value& result = ValueFactory::createRange(*start,*increment,*finish);
		context->setCurrentValue(&result);
	} else {
		Value& result = ValueFactory::createRange(*start,*finish);
		context->setCurrentValue(&result);
	}
}

void TreeEvaluator::visit(const UnaryExpression& exp)
{
	exp.getExpression()->accept(*this);
	Value* left=context->getCurrentValue();

	Value* result = Value::evaluate(left,exp.getOp());

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
	const QString& name = stmt.getName();

	Scope* c=context->getCurrentScope();
	/* Process the arguments first. Arguments can themselves contain references
	 * to function calls so evaluate them in a different context */
	startContext(c);
	for(Argument* arg: stmt.getArguments())
		arg->accept(*this);
	const auto arguments=context->getArguments();
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
		for(const auto& a: arguments)
			context->addArgument(a);

		for(Parameter* p: func->getParameters())
			p->accept(*this);

		/* Invoke the function whether it be a user defined function or a build
		 * in function */
		if(scp) {
			scp->accept(*this);
			result=context->getReturnValue();
		} else {
			result=&func->evaluate(*context);
		}

		finishContext();

	} else {
		reporter.reportWarning(tr("cannot find function '%1'").arg(name));
	}

	if(!result)
		result=&ValueFactory::createUndefined();

	context->setCurrentValue(result);
}

void TreeEvaluator::visit(Callback& c)
{
	Expression* e = c.getExpression();
	e->accept(*this);
	c.setResult(context->getCurrentValue());
}

QFileInfo TreeEvaluator::getFullPath(const QString& file)
{
	if(!importLocations.isEmpty())
		return QFileInfo(importLocations.constLast(),file);

	return QFileInfo(file); /* relative to working dir */
}

void TreeEvaluator::visit(const ModuleImport& mi)
{
	auto* mod=new ImportModule(reporter);
	const QFileInfo& f=getFullPath(mi.getImport());
	mod->setImport(f.absoluteFilePath());
	mod->setName(mi.getName());
	modules.append(mod);
	//TODO global import args.

	/* Adding the import module to the current layout is ok here because we
	 * assume import statements are at the top level, and that we will be at
	 * the top level at this point */
	layout->addModule(*mod);
}

void TreeEvaluator::visit(const ScriptImport& sc)
{
	if(!descendDone) {
		const QFileInfo& f=getFullPath(sc.getImport());
		auto* s=new Script(reporter);
		s->parse(f);
		imports.insert(&sc,s);
		/* Now recursively descend any modules functions or script imports within
		 * the imported script and add them to the main script */
		const QDir& loc=f.absoluteDir();
		importLocations.push(loc);
		descend(s);
		importLocations.pop();
		return;
	}

	/* Evaluate the global variables of the imported scripts in the context
	 * of the main script */
	Script* scp=imports.value(&sc);
	if(scp) {
		for(Declaration* d: scp->getDeclarations()) {
			auto* i=dynamic_cast<ScriptImport*>(d);
			if(i)
				i->accept(*this);
			auto* a=dynamic_cast<AssignStatement*>(d);
			if(a)
				a->accept(*this);
		}
	}
}

void TreeEvaluator::visit(const Literal& lit)
{
	Value& v=lit.getValue();

	context->setCurrentValue(&v);
}

void TreeEvaluator::visit(const Variable& var)
{
	const QString& name = var.getName();
	const Storage oldStorage=var.getStorage();
	Storage currentStorage=oldStorage;
	Layout* l=scopeLookup.value(context->getCurrentScope());
	Value& v=context->lookupVariable(name,currentStorage,l);
	if(currentStorage!=oldStorage)
		switch(oldStorage) {
			case Storage::Constant:
				reporter.reportWarning(tr("attempt to make previously non-constant variable '%1' constant").arg(name));
				break;
			case Storage::Parametric:
				reporter.reportWarning(tr("attempt to make previously non-parametric variable '%1' parametric").arg(name));
				break;
			default:
				break;
		}

	context->setCurrentValue(&v);
	context->setCurrentName(name);
}

void TreeEvaluator::visit(const CodeDocParam&)
{
}

void TreeEvaluator::visit(const CodeDocDeclaration&)
{
}

void TreeEvaluator::visit(Script& sc)
{
	const BuiltinManager m(sc,reporter);

	/* Use the location of the current script as the root for all imports */
	const QDir& loc=sc.getFileLocation();
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
	const QList<Node*>& childnodes=context->getCurrentNodes();

	if(context->getReturnValue())
		reporter.reportWarning(tr("return statement not valid inside global scope."));

	rootNode=UnionModule::createUnion(childnodes);

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
	Value& v=ValueFactory::createComplex(*result,childvalues);
	context->setCurrentValue(&v);
}

Node* TreeEvaluator::getRootNode() const
{
	return rootNode;
}
