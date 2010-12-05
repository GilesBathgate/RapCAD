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

#include "prettyprinter.h"

PrettyPrinter::PrettyPrinter()
{
}

PrettyPrinter::~PrettyPrinter()
{
}

void PrettyPrinter::visit(ModuleScope * scp)
{
    result.append("\nScope: (\n");
    QVector<Declaration*> declarations = scp->getDeclarations();
	for(int i=0; i<declarations.size(); i++)
	{
	    declarations.at(i)->accept(this);
	    result.append("\n");
	}
    result.append(")\n");
}

void PrettyPrinter::visit(Instance * inst)
{
    switch(inst->getType())
    {
    case Instance::Root:
	result.append("Root ");
	break;
    case Instance::Debug:
	result.append("Debug ");
	break;
    case Instance::Background:
	result.append("Background ");
	break;
    case Instance::Disable:
	result.append("Disabled ");
	break;
    default:
	break;
    }

    result.append("Instance: ");
    result.append(inst->getName());
    result.append(" ");
    QVector<Argument*> arguments = inst->getArguments();
    for(int i=0; i<arguments.size(); i++)
	arguments.at(i)->accept(this);

    QVector<Statement*> children = inst->getChildren();
    result.append("Children: ( ");
    for(int i=0; i<children.size(); i++)
	children.at(i)->accept(this);

    result.append(")");
}

void PrettyPrinter::visit(Module* mod)
{
    result.append("Module: ");
    result.append(mod->getName());
    result.append(" {\n");
    result.append("Parameters: ");
    QVector<Parameter*> parameters = mod->getParameters();
    for(int i=0; i<parameters.size(); i++)
	parameters.at(i)->accept(this);

    mod->getScope()->accept(this);
    result.append("}\n");
}

void PrettyPrinter::visit(Function * func)
{
    result.append("Function: ");
    result.append(func->getName());
    result.append("\n");
    result.append("Parameters: ");
    QVector<Parameter*> parameters = func->getParameters();
    for(int i=0; i<parameters.size(); i++)
	    parameters.at(i)->accept(this);

    result.append(" =\n");
    func->getScope()->accept(this);
    result.append("\n");
}

void PrettyPrinter::visit(FunctionScope * scp)
{
    QVector<Statement*> statements = scp->getStatements();
    for(int i=0; i<statements.size(); i++)
	statements.at(i)->accept(this);

    Expression* expression = scp->getExpression();
    if(expression)
	expression->accept(this);
}

void PrettyPrinter::visit(CompoundStatement * stmt)
{
    QVector<Statement*> children = stmt->getChildren();
    for(int i=0; i<children.size(); i++)
	children.at(i)->accept(this);
}

void PrettyPrinter::visit(IfElseStatement * ifelse)
{
    result.append("If: (");
    Expression* expression = ifelse->getExpression();
    if(expression)
	expression->accept(this);
    result.append(") ");

    Statement* trueStatement = ifelse->getTrueStatement();
    if(trueStatement)
	trueStatement->accept(this);

    Statement* falseStatement = ifelse->getFalseStatement();
    if(falseStatement)
    {
	result.append("Else: ");
	falseStatement->accept(this);
    }
}

void PrettyPrinter::visit(ForStatement * forstmt)
{
    result.append("For: (");
    QVector<Argument*> arguments = forstmt->getArguments();
    for(int i=0; i<arguments.size(); i++)
	arguments[i]->accept(this);
    result.append(") ");
    Statement* statement = forstmt->getStatement();
    statement->accept(this);
}

void PrettyPrinter::visit(Parameter * param)
{
    result.append("Param: ");
    result.append(param->getName());
    result.append(" ");
    Expression* expression = param->getExpression();
    if(expression)
	expression->accept(this);
}

void PrettyPrinter::visit(BinaryExpression * exp)
{
    result.append("Expression: (");
    exp->getLeft()->accept(this);
    result.append("Operator: ");
    result.append(exp->getOpString());
    result.append(" ");
    exp->getRight()->accept(this);
    result.append(") ");
}

void PrettyPrinter::visit(Argument * arg)
{
    result.append("Argument: ");
    Value* variable = arg->getVariable();
    if(variable)
	variable->accept(this);

    result.append(" ");
    Expression* expression = arg->getExpression();
    if(expression)
	expression->accept(this);
}

void PrettyPrinter::visit(AssignStatement * stmt)
{
    result.append("Assign: ");

    Value* value = stmt->getValue();
    if(value)
	value->accept(this);

    Expression* expression = stmt->getExpression();
    if(expression)
	expression->accept(this);
}

void PrettyPrinter::visit(VectorExpression * exp)
{
    result.append("Vector: ");
    QVector<Expression*> children = exp->getChildren();
    for(int i=0; i<children.size(); i++)
	children.at(i)->accept(this);
}

void PrettyPrinter::visit(RangeExpression * exp)
{
    result.append("Range: [ ");
    Expression* start = exp->getStart();
    if(start)
	start->accept(this);

    Expression* step = exp->getStep();
    if(step)
	step->accept(this);

    Expression* finish = exp->getFinish();
    if(finish)
	finish->accept(this);
    result.append("] ");
}

void PrettyPrinter::visit(UnaryExpression * exp)
{
    result.append("Unary: ");
    result.append(exp->getOpString());
    result.append(" ");
    Expression* expression = exp->getExpression();
    expression->accept(this);
}

void PrettyPrinter::visit(ReturnStatement * stmt)
{
    result.append("Return: ");
    Expression* expression = stmt->getExpression();
    if(expression)
	expression->accept(this);
}

void PrettyPrinter::visit(TernaryExpression * exp)
{
    result.append("Ternary: ");
    Expression* condition = exp->getCondition();
    if(condition)
	condition->accept(this);

    Expression* trueExpression = exp->getTrueExpression();
    if(trueExpression)
	trueExpression->accept(this);

    Expression* falseExpression = exp->getFalseExpression();
    if(falseExpression)
	falseExpression->accept(this);
}

void PrettyPrinter::visit(Invocation * stmt)
{
    result.append("Invocation: ");
    result.append(stmt->getName());
    result.append(" (");
    QVector<Argument*> arguments = stmt->getArguments();
    for(int i=0; i<arguments.size(); i++)
	arguments.at(i)->accept(this);
    result.append(")\n");
}

void PrettyPrinter::visit(ModuleImport * decl)
{
    result.append("Use: <");
    result.append(decl->getImport());
    result.append(">\n");
}

void PrettyPrinter::visit(Literal * lit)
{
    result.append("Literal: ");
    result.append(lit->getValueString());
    result.append(" ");
}

void PrettyPrinter::visit(Value * val)
{
    switch(val->getType())
    {
    case Value::Const:
	result.append("Constant: ");
	break;
    case Value::Param:
	result.append("Parametric: ");
	break;
    default:
	result.append("Variable: ");
	break;
    }
    if(val->getType()==Value::Special)
	result.append("$");
    result.append(val->getName());
    result.append(" ");
}

QString PrettyPrinter::getResult()
{
    return result;
}
