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

#include "dependencybuilder.h"

DependencyBuilder::DependencyBuilder()
{
}

DependencyBuilder::~DependencyBuilder()
{
}

void DependencyBuilder::BuildScript(Declaration*)
{
}

void DependencyBuilder::BuildScript(QVector<Declaration*>*)
{
}

Declaration* DependencyBuilder::BuildUse(QString*)
{
    return NULL;
}

Declaration* DependencyBuilder::BuildUse(QString*,QString*)
{
    return NULL;
}

QVector<Declaration*>* DependencyBuilder::BuildDeclarations()
{
    return NULL;
}

QVector<Declaration*>* DependencyBuilder::BuildDeclarations(QVector<Declaration*>*,Declaration*)
{
    return NULL;
}

QVector<Declaration*>* DependencyBuilder::BuildDeclarations(QVector<Declaration*>*)
{
    return NULL;
}

QVector<Declaration*>* DependencyBuilder::BuildDeclarations(Declaration*)
{
    return NULL;
}

Statement* DependencyBuilder::BuildStatement(Statement*)
{
    return NULL;
}

Statement* DependencyBuilder::BuildStatement(QVector<Statement*>*)
{
    return NULL;
}

Statement* DependencyBuilder::BuildStatement(Value*,Expression*)
{
    return NULL;
}

Statement* DependencyBuilder::BuildStatement(QString*,Value::Type_e,Expression*)
{
    return NULL;
}

Statement* DependencyBuilder::BuildStatement(IfElseStatement*)
{
    return NULL;
}

Statement* DependencyBuilder::BuildReturnStatement(Expression*)
{
    return NULL;
}

Statement* DependencyBuilder::BuildIfElseStatement(Expression*,Statement*)
{
    return NULL;
}

Statement* DependencyBuilder::BuildIfElseStatement(Expression*,Statement*,Statement*)
{
    return NULL;
}

Statement* DependencyBuilder::BuildForStatement(QVector<Argument*>*,Statement*)
{
    return NULL;
}

QVector<Statement*>* DependencyBuilder::BuildStatements()
{
    return NULL;
}

QVector<Statement*>* DependencyBuilder::BuildStatements(Statement*)
{
    return NULL;
}

QVector<Statement*>* DependencyBuilder::BuildStatements(QVector<Statement*>*)
{
    return NULL;
}

QVector<Statement*>* DependencyBuilder::BuildStatements(QVector<Statement*>*,Statement*)
{
    return NULL;
}

Declaration* DependencyBuilder::BuildModule(QString*, QVector<Parameter*>*, Scope*)
{
    return NULL;
}

Declaration* DependencyBuilder::BuildFunction(QString*,QVector<Parameter*>*,Scope*)
{
    return NULL;
}

Scope* DependencyBuilder::BuildScope(QVector<Declaration*>*)
{
    return NULL;
}

Scope* DependencyBuilder::BuildScope(Instance*)
{
    return NULL;
}

Scope* DependencyBuilder::BuildScope(Expression*)
{
    return NULL;
}

Scope* DependencyBuilder::BuildScope(QVector<Statement*>*)
{
    return NULL;
}

Instance* DependencyBuilder::BuildInstance(Instance*)
{
    return NULL;
}

Instance* DependencyBuilder::BuildInstance(Instance::Type_e,Instance*)
{
    return NULL;
}

Instance* DependencyBuilder::BuildInstance(QString*,QVector<Argument*>*)
{
    return NULL;
}

Instance* DependencyBuilder::BuildInstance(Instance*,QVector<Statement*>*)
{
    return NULL;
}

QVector<Parameter*>* DependencyBuilder::BuildParameters()
{
    return NULL;
}

QVector<Parameter*>* DependencyBuilder::BuildParameters(Parameter*)
{
    return NULL;
}

QVector<Parameter*>* DependencyBuilder::BuildParameters(QVector<Parameter*>*,Parameter*)
{
    return NULL;
}

Parameter* DependencyBuilder::BuildParameter(QString*)
{
    return NULL;
}

Parameter* DependencyBuilder::BuildParameter(QString*,Expression*)
{
    return NULL;
}

QVector<Argument*>* DependencyBuilder::BuildArguments()
{
    return NULL;
}

QVector<Argument*>* DependencyBuilder::BuildArguments(Argument*)
{
    return NULL;
}

QVector<Argument*>* DependencyBuilder::BuildArguments(QVector<Argument*>*,unsigned int,Argument*)
{
    return NULL;
}

Argument* DependencyBuilder::BuildArgument(Expression*)
{
    return NULL;
}

Argument* DependencyBuilder::BuildArgument(Value*,Expression*)
{
    return NULL;
}

unsigned int DependencyBuilder::BuildOptionalCommas()
{
    return NULL;
}

unsigned int DependencyBuilder::BuildOptionalCommas(unsigned int)
{
    return NULL;
}

Expression* DependencyBuilder::BuildLiteral()
{
    return NULL;
}

Expression* DependencyBuilder::BuildLiteral(bool)
{
    return NULL;
}

Expression* DependencyBuilder::BuildLiteral(double)
{
    return NULL;
}

Expression* DependencyBuilder::BuildLiteral(QString*)
{
    return NULL;
}

Value* DependencyBuilder::BuildVariable(QString*)
{
    return NULL;
}

Expression* DependencyBuilder::BuildVariable(Value*)
{
    return NULL;
}

Value* DependencyBuilder::BuildVariable(QString*,Value::Type_e)
{
    return NULL;
}

Expression* DependencyBuilder::BuildExpression(Expression*,QString*)
{
    return NULL;
}

Expression* DependencyBuilder::BuildExpression(Expression*)
{
    return NULL;
}

Expression* DependencyBuilder::BuildExpression(Expression::Operator_e,Expression*)
{
    return NULL;
}

Expression* DependencyBuilder::BuildExpression(Expression*,Expression::Operator_e, Expression*)
{
    return NULL;
}

Expression* DependencyBuilder::BuildTernaryExpression(Expression*,Expression*,Expression*)
{
    return NULL;
}

Expression* DependencyBuilder::BuildExpression(QVector<Expression*>*)
{
    return NULL;
}

QVector<Expression*>* DependencyBuilder::BuildVector()
{
    return NULL;
}

QVector<Expression*>* DependencyBuilder::BuildVector(Expression*)
{
    return NULL;
}

QVector<Expression*>* DependencyBuilder::BuildVector(QVector<Expression*>*,unsigned int,Expression*)
{
    return NULL;
}

Expression* DependencyBuilder::BuildRange(Expression*,Expression*)
{
    return NULL;
}

Expression* DependencyBuilder::BuildRange(Expression*,Expression*,Expression*)
{
    return NULL;
}

Expression* DependencyBuilder::BuildInvocation(QString*,QVector<Argument*>*)
{
    return NULL;
}

Script* DependencyBuilder::getResult()
{
    return NULL;
}
