/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

void DependencyBuilder::buildScript(Declaration*)
{
}

void DependencyBuilder::buildScript(QVector<Declaration*>*)
{
}

Declaration* DependencyBuilder::buildUse(QString*)
{
	return NULL;
}

Declaration* DependencyBuilder::buildUse(QString*,QString*)
{
	return NULL;
}

Declaration* DependencyBuilder::buildImport(QString*,QString*)
{
	return NULL;
}

Declaration* DependencyBuilder::buildImport(QString*,QString*,QVector<Parameter*>*)
{
	return NULL;
}

Declaration* DependencyBuilder::buildDeclaration(Declaration*)
{
	return NULL;
}

QVector<Declaration*>* DependencyBuilder::buildDeclarations()
{
	return NULL;
}

QVector<Declaration*>* DependencyBuilder::buildDeclarations(QVector<Declaration*>*,Declaration*)
{
	return NULL;
}

QVector<Declaration*>* DependencyBuilder::buildDeclarations(QVector<Declaration*>*)
{
	return NULL;
}

QVector<Declaration*>* DependencyBuilder::buildDeclarations(Declaration*)
{
	return NULL;
}

Statement* DependencyBuilder::buildStatement(Statement*)
{
	return NULL;
}

Statement* DependencyBuilder::buildStatement(Variable*,Expression::Operator_e)
{
	return NULL;
}

Statement* DependencyBuilder::buildStatement(QVector<Statement*>*)
{
	return NULL;
}

Statement* DependencyBuilder::buildStatement(Variable*,Expression*)
{
	return NULL;
}

Statement* DependencyBuilder::buildStatement(QString*,Variable::Type_e,Expression*)
{
	return NULL;
}

Statement* DependencyBuilder::buildReturnStatement(Expression*)
{
	return NULL;
}

Statement* DependencyBuilder::buildIfElseStatement(Expression*,Statement*)
{
	return NULL;
}

Statement* DependencyBuilder::buildIfElseStatement(Expression*,Statement*,Statement*)
{
	return NULL;
}

Statement* DependencyBuilder::buildForStatement(QVector<Argument*>*,Statement*)
{
	return NULL;
}

QVector<Statement*>* DependencyBuilder::buildStatements()
{
	return NULL;
}

QVector<Statement*>* DependencyBuilder::buildStatements(Statement*)
{
	return NULL;
}

QVector<Statement*>* DependencyBuilder::buildStatements(QVector<Statement*>*)
{
	return NULL;
}

QVector<Statement*>* DependencyBuilder::buildStatements(QVector<Statement*>*,Statement*)
{
	return NULL;
}

Declaration* DependencyBuilder::buildModule(QString*, QVector<Parameter*>*, Scope*)
{
	return NULL;
}

Declaration* DependencyBuilder::buildFunction(QString*,QVector<Parameter*>*,Scope*)
{
	return NULL;
}

Scope* DependencyBuilder::buildScope(QVector<Declaration*>*)
{
	return NULL;
}

Scope* DependencyBuilder::buildScope(Instance*)
{
	return NULL;
}

Scope* DependencyBuilder::buildScope(Expression*)
{
	return NULL;
}

Scope* DependencyBuilder::buildScope(QVector<Statement*>*)
{
	return NULL;
}

Instance* DependencyBuilder::buildInstance(Instance*)
{
	return NULL;
}

Instance* DependencyBuilder::buildInstance(QString*,Instance*)
{
	return NULL;
}

Instance* DependencyBuilder::buildInstance(Instance::Type_e,Instance*)
{
	return NULL;
}

Instance* DependencyBuilder::buildInstance(QString*,QVector<Argument*>*)
{
	return NULL;
}

Instance* DependencyBuilder::buildInstance(Instance*,QVector<Statement*>*)
{
	return NULL;
}

QVector<Parameter*>* DependencyBuilder::buildParameters()
{
	return NULL;
}

QVector<Parameter*>* DependencyBuilder::buildParameters(Parameter*)
{
	return NULL;
}

QVector<Parameter*>* DependencyBuilder::buildParameters(QVector<Parameter*>*,Parameter*)
{
	return NULL;
}

Parameter* DependencyBuilder::buildParameter(QString*)
{
	return NULL;
}

Parameter* DependencyBuilder::buildParameter(QString*,Expression*)
{
	return NULL;
}

QVector<Argument*>* DependencyBuilder::buildArguments()
{
	return NULL;
}

QVector<Argument*>* DependencyBuilder::buildArguments(Argument*)
{
	return NULL;
}

QVector<Argument*>* DependencyBuilder::buildArguments(QVector<Argument*>*,unsigned int,Argument*)
{
	return NULL;
}

Argument* DependencyBuilder::buildArgument(Expression*)
{
	return NULL;
}

Argument* DependencyBuilder::buildArgument(Variable*,Expression*)
{
	return NULL;
}

unsigned int DependencyBuilder::buildOptionalCommas()
{
	return NULL;
}

unsigned int DependencyBuilder::buildOptionalCommas(unsigned int)
{
	return NULL;
}

Expression* DependencyBuilder::buildLiteral()
{
	return NULL;
}

Expression* DependencyBuilder::buildLiteral(bool)
{
	return NULL;
}

Expression* DependencyBuilder::buildLiteral(double)
{
	return NULL;
}

Expression* DependencyBuilder::buildLiteral(QString*)
{
	return NULL;
}

Variable* DependencyBuilder::buildVariable(QString*)
{
	return NULL;
}

Expression* DependencyBuilder::buildVariable(Variable*)
{
	return NULL;
}

Variable* DependencyBuilder::buildVariable(QString*,Variable::Type_e)
{
	return NULL;
}

Expression* DependencyBuilder::buildExpression(Expression*,QString*)
{
	return NULL;
}

Expression* DependencyBuilder::buildExpression(Expression*)
{
	return NULL;
}

Expression* DependencyBuilder::buildExpression(Expression::Operator_e,Expression*)
{
	return NULL;
}

Expression* DependencyBuilder::buildExpression(Expression*,Expression::Operator_e, Expression*)
{
	return NULL;
}

Expression* DependencyBuilder::buildExpression(Expression*,Expression*,Expression*)
{
	return NULL;
}

Expression* DependencyBuilder::buildExpression(QVector<Expression*>*)
{
	return NULL;
}

QVector<Expression*>* DependencyBuilder::buildVector()
{
	return NULL;
}

QVector<Expression*>* DependencyBuilder::buildVector(Expression*)
{
	return NULL;
}

QVector<Expression*>* DependencyBuilder::buildVector(QVector<Expression*>*,unsigned int,Expression*)
{
	return NULL;
}

Expression* DependencyBuilder::buildRange(Expression*,Expression*)
{
	return NULL;
}

Expression* DependencyBuilder::buildRange(Expression*,Expression*,Expression*)
{
	return NULL;
}

Invocation* DependencyBuilder::buildInvocation(QString*,QVector<Argument*>*)
{
	return NULL;
}

Invocation* DependencyBuilder::buildInvocation(QString*,Invocation*)
{
	return NULL;
}

Script* DependencyBuilder::getResult() const
{
	return NULL;
}
