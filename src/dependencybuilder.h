/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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

#ifndef DEPENDENCYBUILDER_H
#define DEPENDENCYBUILDER_H

#include "abstractsyntaxtreebuilder.h"

class DependencyBuilder : public AbstractSyntaxTreeBuilder
{
public:
	DependencyBuilder() = default;
	~DependencyBuilder() override = default;
	void buildFileLocation(QDir) override;
	void buildScript(Declaration*) override;
	void buildScript(QList<Declaration*>*) override;
	void buildScript(QList<CodeDoc*>*) override;
	QList<CodeDoc*>* buildCodeDoc(QList<CodeDoc*>*) override;
	QList<CodeDoc*>* buildCodeDoc() override;
	QList<CodeDoc*>* buildCodeDoc(QString*,QList<CodeDoc*>*) override;
	QList<CodeDoc*>* buildCodeDoc(QString*,QString*,QList<CodeDoc*>*) override;
	Declaration* buildUse(QString*) override;
	Declaration* buildUse(QString*,QString*) override;
	Declaration* buildImport(QString*,QString*) override;
	Declaration* buildImport(QString*,QString*,QList<Parameter*>*) override;
	Declaration* buildDeclaration(Declaration*) override;
	QList<Declaration*>* buildDeclarations() override;
	QList<Declaration*>* buildDeclarations(Declaration*) override;
	QList<Declaration*>* buildDeclarations(QList<Declaration*>*) override;
	QList<Declaration*>* buildDeclarations(QList<Declaration*>*,Declaration*) override;
	Statement* buildStatement(Statement*) override;
	Statement* buildStatement(Variable*,Operators) override;
	Statement* buildStatement(Variable*,Operators,Expression*) override;
	Statement* buildStatement(QList<Statement*>*) override;
	Statement* buildStatement(Variable*,Expression*) override;
	Statement* buildStatement(QString*,Storage,Expression*) override;
	Statement* buildReturnStatement(Expression*) override;
	Statement* buildIfElseStatement(Expression*,Statement*) override;
	Statement* buildIfElseStatement(Expression*,Statement*,Statement*) override;
	Statement* buildForStatement(QList<Argument*>*,Statement*) override;
	QList<Statement*>* buildStatements() override;
	QList<Statement*>* buildStatements(Statement*) override;
	QList<Statement*>* buildStatements(QList<Statement*>*) override;
	QList<Statement*>* buildStatements(QList<Statement*>*,Statement*) override;
	Declaration* buildModule(QString*,QList<Parameter*>*,Scope*) override;
	Declaration* buildFunction(QString*,QList<Parameter*>*,Scope*) override;
	Scope* buildScope(QList<Declaration*>*) override;
	Scope* buildScope(Instance*) override;
	Scope* buildScope(Expression*) override;
	Scope* buildScope(QList<Statement*>*) override;
	Instance* buildInstance(Instance*) override;
	Instance* buildInstance(QString*,Instance*) override;
	Instance* buildInstance(InstanceTypes,Instance*) override;
	Instance* buildInstance(InstanceTypes,QString*,QList<Argument*>*) override;
	Instance* buildInstance(QString*,QList<Argument*>*) override;
	Instance* buildInstance(Instance*,QList<Statement*>*) override;
	QList<Parameter*>* buildParameters() override;
	QList<Parameter*>* buildParameters(Parameter*) override;
	QList<Parameter*>* buildParameters(QList<Parameter*>*,Parameter*) override;
	Parameter* buildParameter(QString*) override;
	Parameter* buildParameter(QString*,Expression*) override;
	QList<Argument*>* buildArguments() override;
	QList<Argument*>* buildArguments(Argument*) override;
	QList<Argument*>* buildArguments(QList<Argument*>*,unsigned int,Argument*) override;
	Argument* buildArgument(Expression*) override;
	Argument* buildArgument(Variable*,Expression*) override;
	unsigned int buildOptionalCommas() override;
	unsigned int buildOptionalCommas(unsigned int) override;
	Expression* buildLiteral() override;
	Expression* buildLiteral(bool) override;
	Expression* buildLiteral(decimal*) override;
	Expression* buildLiteral(QString*) override;
	Variable* buildVariable(QString*) override;
	Expression* buildVariable(Variable*) override;
	Variable* buildVariable(QString*,Storage) override;
	Expression* buildExpression(Expression*,QString*) override;
	Expression* buildExpression(Expression*) override;
	Expression* buildExpression(Operators,Expression*) override;
	Expression* buildExpression(Expression*,Operators,Expression*) override;
	Expression* buildExpression(Expression*,Expression*,Expression*) override;
	Expression* buildExpression(QList<Expression*>*,int) override;
	QList<Expression*>* buildVector() override;
	QList<Expression*>* buildVector(Expression*) override;
	QList<Expression*>* buildVector(QList<Expression*>*,unsigned int,Expression*) override;
	Expression* buildRange(Expression*,Expression*) override;
	Expression* buildRange(Expression*,Expression*,Expression*) override;
	Expression* buildComplex(Expression*,Expression*,Expression*,Expression*) override;
	Invocation* buildInvocation(QString*,QList<Argument*>*) override;
	Invocation* buildInvocation(QString*,Invocation*) override;
	void reportSyntaxError(QString) override;
};

#endif // DEPENDENCYBUILDER_H
