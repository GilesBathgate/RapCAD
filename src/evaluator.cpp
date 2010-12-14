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
#include <stdio.h>

Evaluator::Evaluator(Script* sc)
{
    this->script = sc;
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
    if(name=="echo")
    {
	Argument* arg0 = inst->getArguments().at(0);
	arg0->accept(*this);


	Literal* lit = context->currentvalue->getValue();
	const char* t = lit->getValueString().toLocal8Bit();
	printf("ECHO: %s\n",t);

    }

    Module* mod = context->lookupmodule(name);


    foreach(Statement* s, inst->getChildren())
	s->accept(*this);
}

void Evaluator::visit(Module* mod)
{
    context->currentname = mod->getName();
    mod->getScope()->accept(*this);
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

void Evaluator::Evaluate()
{
    foreach(Declaration* d, script->getDeclarations())
	d->accept(*this);
}
