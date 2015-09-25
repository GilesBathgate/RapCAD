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

#ifndef DEPENDENCYBUILDER_H
#define DEPENDENCYBUILDER_H

#include "abstractsyntaxtreebuilder.h"

class DependencyBuilder : public AbstractSyntaxTreeBuilder
{
public:
	DependencyBuilder();
	~DependencyBuilder();
	void buildFileLocation(QString)=0;
	void buildScript(Declaration*);
	void buildScript(QList<Declaration*>*);
	void buildScript(QList<CodeDoc*>*);
	QList<CodeDoc*>* buildCodeDoc(QList<CodeDoc*>*);
	QList<CodeDoc*>* buildCodeDoc();
	QList<CodeDoc*>* buildCodeDoc(QString*,QList<CodeDoc*>*);
	QList<CodeDoc*>* buildCodeDoc(QString*,QString*,QList<CodeDoc*>*);
	Declaration* buildUse(QString*);
	Declaration* buildUse(QString*,QString*);
	Declaration* buildImport(QString*,QString*);
	Declaration* buildImport(QString*,QString*,QList<Parameter*>*);
	Declaration* buildDeclaration(Declaration*);
	QList<Declaration*>* buildDeclarations();
	QList<Declaration*>* buildDeclarations(Declaration*);
	QList<Declaration*>* buildDeclarations(QList<Declaration*>*);
	QList<Declaration*>* buildDeclarations(QList<Declaration*>*,Declaration*);
	Statement* buildStatement(Statement*);
	Statement* buildStatement(Variable*,Expression::Operator_e);
	Statement* buildStatement(Variable*,Expression::Operator_e,Expression*);
	Statement* buildStatement(QList<Statement*>*);
	Statement* buildStatement(Variable*,Expression*);
	Statement* buildStatement(QString*,Variable::Storage_e,Expression*);
	Statement* buildReturnStatement(Expression*);
	Statement* buildIfElseStatement(Expression*,Statement*);
	Statement* buildIfElseStatement(Expression*,Statement*,Statement*);
	Statement* buildForStatement(QList<Argument*>*,Statement*);
	QList<Statement*>* buildStatements();
	QList<Statement*>* buildStatements(Statement*);
	QList<Statement*>* buildStatements(QList<Statement*>*);
	QList<Statement*>* buildStatements(QList<Statement*>*,Statement*);
	Declaration* buildModule(QString*,QList<Parameter*>*,Scope*);
	Declaration* buildFunction(QString*,QList<Parameter*>*,Scope*);
	Scope* buildScope(QList<Declaration*>*);
	Scope* buildScope(Instance*);
	Scope* buildScope(Expression*);
	Scope* buildScope(QList<Statement*>*);
	Instance* buildInstance(Instance*);
	Instance* buildInstance(QString*,Instance*);
	Instance* buildInstance(Instance::Type_e,Instance*);
	Instance* buildInstance(Instance::Type_e,QString*,QList<Argument*>*);
	Instance* buildInstance(QString*,QList<Argument*>*);
	Instance* buildInstance(Instance*,QList<Statement*>*);
	QList<Parameter*>* buildParameters();
	QList<Parameter*>* buildParameters(Parameter*);
	QList<Parameter*>* buildParameters(QList<Parameter*>*,Parameter*);
	Parameter* buildParameter(QString*);
	Parameter* buildParameter(QString*,Expression*);
	QList<Argument*>* buildArguments();
	QList<Argument*>* buildArguments(Argument*);
	QList<Argument*>* buildArguments(QList<Argument*>*,unsigned int,Argument*);
	Argument* buildArgument(Expression*);
	Argument* buildArgument(Variable*,Expression*);
	unsigned int buildOptionalCommas();
	unsigned int buildOptionalCommas(unsigned int);
	Expression* buildLiteral();
	Expression* buildLiteral(bool);
	Expression* buildLiteral(decimal* value);
	Expression* buildLiteral(QString* value);
	Variable* buildVariable(QString* name);
	Expression* buildVariable(Variable*);
	Variable* buildVariable(QString*,Variable::Storage_e);
	Expression* buildExpression(Expression*,QString*);
	Expression* buildExpression(Expression*);
	Expression* buildExpression(Expression::Operator_e,Expression*);
	Expression* buildExpression(Expression*,Expression::Operator_e,Expression*);
	Expression* buildExpression(Expression*,Expression*,Expression*);
	Expression* buildExpression(QList<Expression*>*,int);
	QList<Expression*>* buildVector();
	QList<Expression*>* buildVector(Expression*);
	QList<Expression*>* buildVector(QList<Expression*>*,unsigned int,Expression*);
	Expression* buildRange(Expression*,Expression*);
	Expression* buildRange(Expression*,Expression*,Expression*);
	Invocation* buildInvocation(QString*,QList<Argument*>*);
	Invocation* buildInvocation(QString*,Invocation*);

	Script* getResult() const;
};

#endif // DEPENDENCYBUILDER_H
