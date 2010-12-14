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
    workingvalue=NULL;
    this->script = sc;
    context_stack.push(new Context());
}

Evaluator::~Evaluator()
{
}

void Evaluator::visit(ModuleScope * scp)
{
    QVector<Declaration*> declarations = scp->getDeclarations();

    for(int i=0; i<declarations.size(); i++)
	    declarations.at(i)->accept(*this);
}

void Evaluator::visit(Instance * inst)
{
    QString name = inst->getName();
    if(name=="echo")
    {
	Argument* arg0 = inst->getArguments().at(0);
	arg0->accept(*this);


	Literal* lit = workingvalue->getValue();
	const char* t = lit->getValueString().toLocal8Bit();
	printf("ECHO: %s\n",t);

    }
}

void Evaluator::visit(Module* mod)
{
    workingname = mod->getName();
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
    QHash<QString,Value*>* vars = &context_stack.top()->variables;
    stmt->getVariable()->accept(*this);

    stmt->getExpression()->accept(*this);

    vars->insert(workingname,workingvalue);
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
    workingvalue = new Value();
    workingvalue->setValue(lit);
}

void Evaluator::visit(Variable * var)
{
    workingname = var->getName();
    QHash<QString,Value*> vars = context_stack.top()->variables;
    if(vars.contains(workingname)) {
	workingvalue=vars[workingname];
    }
}

void Evaluator::Evaluate()
{
    QVector<Declaration*> declarations = script->getDeclarations();

    for(int i=0; i<declarations.size(); i++)
	    declarations.at(i)->accept(*this);
}
