/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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

void DependencyBuilder::buildFileLocation(QDir)
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
	return nullptr;
}

QList<CodeDoc*>* DependencyBuilder::buildCodeDoc()
{
	return nullptr;
}

QList<CodeDoc*>* DependencyBuilder::buildCodeDoc(QString*,QList<CodeDoc*>*)
{
	return nullptr;
}

QList<CodeDoc*>* DependencyBuilder::buildCodeDoc(QString*,QString*,QList<CodeDoc*>*)
{
	return nullptr;
}

Declaration* DependencyBuilder::buildUse(QString*)
{
	return nullptr;
}

Declaration* DependencyBuilder::buildUse(QString*,QString*)
{
	return nullptr;
}

Declaration* DependencyBuilder::buildImport(QString*,QString*)
{
	return nullptr;
}

Declaration* DependencyBuilder::buildImport(QString*,QString*,QList<Parameter*>*)
{
	return nullptr;
}

Declaration* DependencyBuilder::buildDeclaration(Declaration*)
{
	return nullptr;
}

QList<Declaration*>* DependencyBuilder::buildDeclarations()
{
	return nullptr;
}

QList<Declaration*>* DependencyBuilder::buildDeclarations(QList<Declaration*>*,Declaration*)
{
	return nullptr;
}

QList<Declaration*>* DependencyBuilder::buildDeclarations(QList<Declaration*>*)
{
	return nullptr;
}

QList<Declaration*>* DependencyBuilder::buildDeclarations(Declaration*)
{
	return nullptr;
}

Statement* DependencyBuilder::buildStatement(Statement*)
{
	return nullptr;
}

Statement* DependencyBuilder::buildStatement(Variable*,Expression::Operator_e)
{
	return nullptr;
}

Statement* DependencyBuilder::buildStatement(Variable*,Expression::Operator_e,Expression*)
{
	return nullptr;
}

Statement* DependencyBuilder::buildStatement(QList<Statement*>*)
{
	return nullptr;
}

Statement* DependencyBuilder::buildStatement(Variable*,Expression*)
{
	return nullptr;
}

Statement* DependencyBuilder::buildStatement(QString*,Variable::Storage_e,Expression*)
{
	return nullptr;
}

Statement* DependencyBuilder::buildReturnStatement(Expression*)
{
	return nullptr;
}

Statement* DependencyBuilder::buildIfElseStatement(Expression*,Statement*)
{
	return nullptr;
}

Statement* DependencyBuilder::buildIfElseStatement(Expression*,Statement*,Statement*)
{
	return nullptr;
}

Statement* DependencyBuilder::buildForStatement(QList<Argument*>*,Statement*)
{
	return nullptr;
}

QList<Statement*>* DependencyBuilder::buildStatements()
{
	return nullptr;
}

QList<Statement*>* DependencyBuilder::buildStatements(Statement*)
{
	return nullptr;
}

QList<Statement*>* DependencyBuilder::buildStatements(QList<Statement*>*)
{
	return nullptr;
}

QList<Statement*>* DependencyBuilder::buildStatements(QList<Statement*>*,Statement*)
{
	return nullptr;
}

Declaration* DependencyBuilder::buildModule(QString*, QList<Parameter*>*, Scope*)
{
	return nullptr;
}

Declaration* DependencyBuilder::buildFunction(QString*,QList<Parameter*>*,Scope*)
{
	return nullptr;
}

Scope* DependencyBuilder::buildScope(QList<Declaration*>*)
{
	return nullptr;
}

Scope* DependencyBuilder::buildScope(Instance*)
{
	return nullptr;
}

Scope* DependencyBuilder::buildScope(Expression*)
{
	return nullptr;
}

Scope* DependencyBuilder::buildScope(QList<Statement*>*)
{
	return nullptr;
}

Instance* DependencyBuilder::buildInstance(Instance*)
{
	return nullptr;
}

Instance* DependencyBuilder::buildInstance(QString*,Instance*)
{
	return nullptr;
}

Instance* DependencyBuilder::buildInstance(Instance::Type_e,Instance*)
{
	return nullptr;
}

Instance* DependencyBuilder::buildInstance(Instance::Type_e,QString*,QList<Argument*>*)
{
	return nullptr;
}

Instance* DependencyBuilder::buildInstance(QString*,QList<Argument*>*)
{
	return nullptr;
}

Instance* DependencyBuilder::buildInstance(Instance*,QList<Statement*>*)
{
	return nullptr;
}

QList<Parameter*>* DependencyBuilder::buildParameters()
{
	return nullptr;
}

QList<Parameter*>* DependencyBuilder::buildParameters(Parameter*)
{
	return nullptr;
}

QList<Parameter*>* DependencyBuilder::buildParameters(QList<Parameter*>*,Parameter*)
{
	return nullptr;
}

Parameter* DependencyBuilder::buildParameter(QString*)
{
	return nullptr;
}

Parameter* DependencyBuilder::buildParameter(QString*,Expression*)
{
	return nullptr;
}

QList<Argument*>* DependencyBuilder::buildArguments()
{
	return nullptr;
}

QList<Argument*>* DependencyBuilder::buildArguments(Argument*)
{
	return nullptr;
}

QList<Argument*>* DependencyBuilder::buildArguments(QList<Argument*>*,unsigned int,Argument*)
{
	return nullptr;
}

Argument* DependencyBuilder::buildArgument(Expression*)
{
	return nullptr;
}

Argument* DependencyBuilder::buildArgument(Variable*,Expression*)
{
	return nullptr;
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
	return nullptr;
}

Expression* DependencyBuilder::buildLiteral(bool)
{
	return nullptr;
}

Expression* DependencyBuilder::buildLiteral(decimal*)
{
	return nullptr;
}

Expression* DependencyBuilder::buildLiteral(decimal*,QString*)
{
	return nullptr;
}

Expression* DependencyBuilder::buildLiteral(QString*)
{
	return nullptr;
}

Variable* DependencyBuilder::buildVariable(QString*)
{
	return nullptr;
}

Expression* DependencyBuilder::buildVariable(Variable*)
{
	return nullptr;
}

Variable* DependencyBuilder::buildVariable(QString*,Variable::Storage_e)
{
	return nullptr;
}

Expression* DependencyBuilder::buildExpression(Expression*,QString*)
{
	return nullptr;
}

Expression* DependencyBuilder::buildExpression(Expression*)
{
	return nullptr;
}

Expression* DependencyBuilder::buildExpression(Expression::Operator_e,Expression*)
{
	return nullptr;
}

Expression* DependencyBuilder::buildExpression(Expression*,Expression::Operator_e, Expression*)
{
	return nullptr;
}

Expression* DependencyBuilder::buildExpression(Expression*,Expression*,Expression*)
{
	return nullptr;
}

Expression* DependencyBuilder::buildExpression(QList<Expression*>*,int)
{
	return nullptr;
}

QList<Expression*>* DependencyBuilder::buildVector()
{
	return nullptr;
}

QList<Expression*>* DependencyBuilder::buildVector(Expression*)
{
	return nullptr;
}

QList<Expression*>* DependencyBuilder::buildVector(QList<Expression*>*,unsigned int,Expression*)
{
	return nullptr;
}

Expression* DependencyBuilder::buildRange(Expression*,Expression*)
{
	return nullptr;
}

Expression* DependencyBuilder::buildRange(Expression*,Expression*,Expression*)
{
	return nullptr;
}

Expression*DependencyBuilder::buildComplex(Expression*, Expression*, Expression*, Expression*)
{
	return nullptr;
}

Invocation* DependencyBuilder::buildInvocation(QString*,QList<Argument*>*)
{
	return nullptr;
}

Invocation* DependencyBuilder::buildInvocation(QString*,Invocation*)
{
	return nullptr;
}

void DependencyBuilder::reportSyntaxError(QString)
{
}
