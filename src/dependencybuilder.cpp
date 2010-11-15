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

void DependencyBuilder::BuildScript(QVector<Declaration*>*)
{
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

Statement* DependencyBuilder::BuildStatement(Variable*,Expression*)
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

Declaration* DependencyBuilder::BuildModule(QString, QVector<Parameter*>*, Context*)
{
    return NULL;
}

Declaration* DependencyBuilder::BuildFunction(QString,QVector<Parameter*>*,Context*)
{
    return NULL;
}

Context* DependencyBuilder::BuildContext(QVector<Declaration*>*)
{
    return NULL;
}

Context* DependencyBuilder::BuildContext(Instance*)
{
    return NULL;
}

Context* DependencyBuilder::BuildContext(Expression*)
{
    return NULL;
}

Context* DependencyBuilder::BuildContext(QVector<Statement*>*)
{
    return NULL;
}

Instance* DependencyBuilder::BuildInstance(Instance*)
{
    return NULL;
}

Instance* DependencyBuilder::BuildInstance(QString,QVector<Argument*>*)
{
    return NULL;
}

Instance* DependencyBuilder::BuildInstance(Instance*,QVector<Instance*>*)
{
    return NULL;
}

QVector<Instance*>* DependencyBuilder::BuildInstances()
{
    return NULL;
}

QVector<Instance*>* DependencyBuilder::BuildInstances(Instance*)
{
    return NULL;
}

QVector<Instance*>* DependencyBuilder::BuildInstances(QVector<Instance*>*)
{
    return NULL;
}

QVector<Instance*>* DependencyBuilder::BuildInstances(QVector<Instance*>*,Instance*)
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

Parameter* DependencyBuilder::BuildParameter(QString)
{
    return NULL;
}

Parameter* DependencyBuilder::BuildParameter(QString,Expression*)
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

Argument* DependencyBuilder::BuildArgument(Variable*,Expression*)
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

Expression* DependencyBuilder::BuildLiteral(char*)
{
    return NULL;
}

Variable* DependencyBuilder::BuildVariable(QString)
{
    return NULL;
}

Expression* DependencyBuilder::BuildVariable(Variable*)
{
    return NULL;
}

Expression* DependencyBuilder::BuildExpression(Expression*,Expression::Operator_e, Expression*)
{
    return NULL;
}

