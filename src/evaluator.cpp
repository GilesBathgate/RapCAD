/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010  Giles Bathgate
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

Evaluator::Evaluator()
{
    context=NULL;
    startcontext();
}

Evaluator::~Evaluator()
{
}

void Evaluator::startcontext()
{
    Context* parent = context;
    context = new Context();
    context->parent = parent;
    context_stack.push(context);
}

void Evaluator::finishcontext()
{
    context_stack.pop();
    context=context_stack.top();
}

void Evaluator::visit(ModuleScope * scp)
{
    QVector<Value*> arguments = context->arguments;
    QVector<Value*> parameters = context->parameters;

    startcontext();
    context->currentscope = scp;

    context->args(arguments,parameters);

    foreach(Declaration* d, scp->getDeclarations())
	d->accept(*this);

    finishcontext();
}

void Evaluator::visit(Instance * inst)
{
    QString name = inst->getName();
    Module* mod = context->lookupmodule(name);
    if(mod)
    {
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
    context->addmodule(mod);
}

void Evaluator::visit(Function * func)
{
}

void Evaluator::visit(FunctionScope * scp)
{
}

void Evaluator::visit(CompoundStatement * stmt)
{
    foreach(Statement* s, stmt->getChildren())
	s->accept(*this);
}

void Evaluator::visit(IfElseStatement * ifelse)
{
}

void Evaluator::visit(ForStatement * forstmt)
{
}

void Evaluator::visit(Parameter * param)
{
    QString name = param->getName();

    Value* v;
    Expression* e = param->getExpression();
    if(e) {
	e->accept(*this);
	v = context->currentvalue;
    } else {
	v = new Value();
    }

    v->setName(name);
    context->parameters.append(v);
}

void Evaluator::visit(BinaryExpression * exp)
{
}

void Evaluator::visit(Argument * arg)
{
    QString name;
    Variable* var = arg->getVariable();
    if(var) {
	var->accept(*this);
	name=context->currentname;
    } else {
	name="";
    }

    arg->getExpression()->accept(*this);
    Value* v = context->currentvalue;

    v->setName(name);
    context->arguments.append(v);
}

void Evaluator::visit(AssignStatement * stmt)
{
    stmt->getVariable()->accept(*this);
    QString name = context->currentname;

    stmt->getExpression()->accept(*this);
    Value* v = context->currentvalue;

    v->setName(name);
    context->addvariable(v);
}

void Evaluator::visit(VectorExpression * exp)
{
}

void Evaluator::visit(RangeExpression * exp)
{
}

void Evaluator::visit(UnaryExpression * exp)
{
}

void Evaluator::visit(ReturnStatement * stmt)
{
}

void Evaluator::visit(TernaryExpression * exp)
{
}

void Evaluator::visit(Invocation * stmt)
{
}

void Evaluator::visit(ModuleImport * decl)
{
}

void Evaluator::visit(Literal * lit)
{
    Value* v=context->currentvalue;
    if(!v)
	v=new Value();

    v->setValue(lit);

    context->currentvalue=v;
}

void Evaluator::visit(Variable * var)
{
    QString name = var->getName();
    Value* v=context->lookupvariable(name);

    context->currentvalue=v;
    context->currentname=name;
}

void Evaluator::visit(Script* sc)
{
    //TODO add our "builtin" here for now
    sc->addDeclaration(new EchoModule());

    context->currentscope = sc;
    foreach(Declaration* d, sc->getDeclarations())
	d->accept(*this);
}
