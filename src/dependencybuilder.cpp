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

Declaration* DependencyBuilder::BuildModule(QString, QVector<Parameter*>*, Context*)
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

Instance* DependencyBuilder::BuildInstance(Instance*)
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

