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

Evaluator::Evaluator()
{
    startcontext();
}

Evaluator::~Evaluator()
{
}

void Evaluator::startcontext()
{
    context = new Context();
    context_stack.push(context);
}

void Evaluator::finishcontext()
{
    context_stack.pop();
    context=context_stack.top();
}

void Evaluator::visit(ModuleScope * scp)
{
    startcontext();
    context->currentscope = scp;

    foreach(Declaration* d, scp->getDeclarations())
	d->accept(*this);

    finishcontext();
}

void Evaluator::visit(Instance * inst)
{
    QString name = inst->getName();

    //TODO for now visit the first argument and leave the value
    //of the argument in context.currentvalue
    QVector<Argument*> args = inst->getArguments();
    if(args.size()>0)
    {
	Argument* arg0 = args.at(0);
	arg0->accept(*this);
    }

    Module* mod = context->lookupmodule(name);
    if(mod)
    {
	mod->evaluate(context);
	Scope* scp = mod->getScope();
	if(scp)
	    foreach(Declaration* d, scp->getDeclarations())
		d->accept(*this);
    }
}

void Evaluator::visit(Module* mod)
{

    context->currentname = mod->getName();
    context->addmodule(mod);
    Scope* scp = mod->getScope();
    if(scp)
	scp->accept(*this);
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
}

void Evaluator::visit(BinaryExpression * exp)
{
}

void Evaluator::visit(Argument * arg)
{
    //arg->getVariable()->accept(*this);
    arg->getExpression()->accept(*this);
}

void Evaluator::visit(AssignStatement * stmt)
{
    stmt->getVariable()->accept(*this);
    stmt->getExpression()->accept(*this);
    QHash<QString,Value*>* vars = &context_stack.top()->variables;
    vars->insert(context->currentname,context->currentvalue);
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
    context->currentvalue->setValue(lit);
}

void Evaluator::visit(Variable * var)
{
    context->currentname = var->getName();
    QHash<QString,Value*> vars = context_stack.top()->variables;
    if(vars.contains(context->currentname))
	context->currentvalue=vars[context->currentname];
    else
	context->currentvalue=new Value();
}

void Evaluator::visit(Script* sc)
{
    //TODO add our "builtin" here for now
    Module* echo = new Module();
    echo->setName("echo");
    sc->addDeclaration(echo);

    context->currentscope = sc;
    foreach(Declaration* d, sc->getDeclarations())
	d->accept(*this);
}
