/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

void DependencyBuilder::buildFileLocation(QString)
{
}

void DependencyBuilder::buildScript(Declaration*)
{
}

void DependencyBuilder::buildScript(QList<Declaration*>*)
{
}

void DependencyBuilder::buildScript(QList<CodeDoc*>*)
{
}

QList<CodeDoc*>* DependencyBuilder::buildCodeDoc(QList<CodeDoc*>*)
{
	return NULL;
}

QList<CodeDoc*>* DependencyBuilder::buildCodeDoc()
{
	return NULL;
}

QList<CodeDoc*>* DependencyBuilder::buildCodeDoc(QString*,QList<CodeDoc*>*)
{
	return NULL;
}

QList<CodeDoc*>* DependencyBuilder::buildCodeDoc(QString*,QString*,QList<CodeDoc*>*)
{
	return NULL;
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

Declaration* DependencyBuilder::buildImport(QString*,QString*,QList<Parameter*>*)
{
	return NULL;
}

Declaration* DependencyBuilder::buildDeclaration(Declaration*)
{
	return NULL;
}

QList<Declaration*>* DependencyBuilder::buildDeclarations()
{
	return NULL;
}

QList<Declaration*>* DependencyBuilder::buildDeclarations(QList<Declaration*>*,Declaration*)
{
	return NULL;
}

QList<Declaration*>* DependencyBuilder::buildDeclarations(QList<Declaration*>*)
{
	return NULL;
}

QList<Declaration*>* DependencyBuilder::buildDeclarations(Declaration*)
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

Statement* DependencyBuilder::buildStatement(Variable*,Expression::Operator_e,Expression*)
{
	return NULL;
}

Statement* DependencyBuilder::buildStatement(QList<Statement*>*)
{
	return NULL;
}

Statement* DependencyBuilder::buildStatement(Variable*,Expression*)
{
	return NULL;
}

Statement* DependencyBuilder::buildStatement(QString*,Variable::Storage_e,Expression*)
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

Statement* DependencyBuilder::buildForStatement(QList<Argument*>*,Statement*)
{
	return NULL;
}

QList<Statement*>* DependencyBuilder::buildStatements()
{
	return NULL;
}

QList<Statement*>* DependencyBuilder::buildStatements(Statement*)
{
	return NULL;
}

QList<Statement*>* DependencyBuilder::buildStatements(QList<Statement*>*)
{
	return NULL;
}

QList<Statement*>* DependencyBuilder::buildStatements(QList<Statement*>*,Statement*)
{
	return NULL;
}

Declaration* DependencyBuilder::buildModule(QString*, QList<Parameter*>*, Scope*)
{
	return NULL;
}

Declaration* DependencyBuilder::buildFunction(QString*,QList<Parameter*>*,Scope*)
{
	return NULL;
}

Scope* DependencyBuilder::buildScope(QList<Declaration*>*)
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

Scope* DependencyBuilder::buildScope(QList<Statement*>*)
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

Instance* DependencyBuilder::buildInstance(Instance::Type_e,QString*,QList<Argument*>*)
{
	return NULL;
}

Instance* DependencyBuilder::buildInstance(QString*,QList<Argument*>*)
{
	return NULL;
}

Instance* DependencyBuilder::buildInstance(Instance*,QList<Statement*>*)
{
	return NULL;
}

QList<Parameter*>* DependencyBuilder::buildParameters()
{
	return NULL;
}

QList<Parameter*>* DependencyBuilder::buildParameters(Parameter*)
{
	return NULL;
}

QList<Parameter*>* DependencyBuilder::buildParameters(QList<Parameter*>*,Parameter*)
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

QList<Argument*>* DependencyBuilder::buildArguments()
{
	return NULL;
}

QList<Argument*>* DependencyBuilder::buildArguments(Argument*)
{
	return NULL;
}

QList<Argument*>* DependencyBuilder::buildArguments(QList<Argument*>*,unsigned int,Argument*)
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
	return 1;
}

unsigned int DependencyBuilder::buildOptionalCommas(unsigned int c)
{
	return c;
}

Expression* DependencyBuilder::buildLiteral()
{
	return NULL;
}

Expression* DependencyBuilder::buildLiteral(bool)
{
	return NULL;
}

Expression* DependencyBuilder::buildLiteral(decimal*)
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

Variable* DependencyBuilder::buildVariable(QString*,Variable::Storage_e)
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

Expression* DependencyBuilder::buildExpression(QList<Expression*>*,int)
{
	return NULL;
}

QList<Expression*>* DependencyBuilder::buildVector()
{
	return NULL;
}

QList<Expression*>* DependencyBuilder::buildVector(Expression*)
{
	return NULL;
}

QList<Expression*>* DependencyBuilder::buildVector(QList<Expression*>*,unsigned int,Expression*)
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

Invocation* DependencyBuilder::buildInvocation(QString*,QList<Argument*>*)
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
