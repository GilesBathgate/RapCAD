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

#ifndef ABSTRACTSYNTAXTREEBUILDER_H
#define ABSTRACTSYNTAXTREEBUILDER_H

#include <QList>
#include "decimal.h"
#include "declaration.h"
#include "statement.h"
#include "variable.h"
#include "expression.h"
#include "argument.h"
#include "parameter.h"
#include "scope.h"
#include "instance.h"
#include "script.h"
#include "invocation.h"
#include "abstracttokenbuilder.h"

class AbstractSyntaxTreeBuilder
{
public:
	virtual ~AbstractSyntaxTreeBuilder() {}
	virtual void buildFileLocation(QDir)=0;
	virtual void buildScript(Declaration*)=0;
	virtual void buildScript(QList<Declaration*>*)=0;
	virtual void buildScript(QList<CodeDoc*>*)=0;
	virtual QList<CodeDoc*>* buildCodeDoc(QList<CodeDoc*>*)=0;
	virtual QList<CodeDoc*>* buildCodeDoc()=0;
	virtual QList<CodeDoc*>* buildCodeDoc(QString*,QList<CodeDoc*>*)=0;
	virtual QList<CodeDoc*>* buildCodeDoc(QString*,QString*,QList<CodeDoc*>*)=0;
	virtual Declaration* buildUse(QString*)=0;
	virtual Declaration* buildUse(QString*,QString*)=0;
	virtual Declaration* buildImport(QString*,QString*)=0;
	virtual Declaration* buildImport(QString*,QString*,QList<Parameter*>*)=0;
	virtual Declaration* buildDeclaration(Declaration*)=0;
	virtual QList<Declaration*>* buildDeclarations()=0;
	virtual QList<Declaration*>* buildDeclarations(Declaration*)=0;
	virtual QList<Declaration*>* buildDeclarations(QList<Declaration*>*)=0;
	virtual QList<Declaration*>* buildDeclarations(QList<Declaration*>*,Declaration*)=0;
	virtual Statement* buildStatement(Statement*)=0;
	virtual Statement* buildStatement(Variable*,Expression::Operator_e)=0;
	virtual Statement* buildStatement(Variable*,Expression::Operator_e,Expression*)=0;
	virtual Statement* buildStatement(QList<Statement*>*)=0;
	virtual Statement* buildStatement(Variable*,Expression*)=0;
	virtual Statement* buildStatement(QString*,Variable::Storage_e,Expression*)=0;
	virtual Statement* buildReturnStatement(Expression*)=0;
	virtual Statement* buildIfElseStatement(Expression*,Statement*)=0;
	virtual Statement* buildIfElseStatement(Expression*,Statement*,Statement*)=0;
	virtual Statement* buildForStatement(QList<Argument*>*,Statement*)=0;
	virtual QList<Statement*>* buildStatements()=0;
	virtual QList<Statement*>* buildStatements(Statement*)=0;
	virtual QList<Statement*>* buildStatements(QList<Statement*>*)=0;
	virtual QList<Statement*>* buildStatements(QList<Statement*>*,Statement*)=0;
	virtual Declaration* buildModule(QString*,QList<Parameter*>*,Scope*)=0;
	virtual Declaration* buildFunction(QString*,QList<Parameter*>*,Scope*)=0;
	virtual Scope* buildScope(QList<Declaration*>*)=0;
	virtual Scope* buildScope(Instance*)=0;
	virtual Scope* buildScope(Expression*)=0;
	virtual Scope* buildScope(QList<Statement*>*)=0;
	virtual Instance* buildInstance(Instance*)=0;
	virtual Instance* buildInstance(QString*,Instance*)=0;
	virtual Instance* buildInstance(Instance::Type_e,Instance*)=0;
	virtual Instance* buildInstance(Instance::Type_e,QString*,QList<Argument*>*)=0;
	virtual Instance* buildInstance(QString*,QList<Argument*>*)=0;
	virtual Instance* buildInstance(Instance*,QList<Statement*>*)=0;
	virtual QList<Parameter*>* buildParameters()=0;
	virtual QList<Parameter*>* buildParameters(Parameter*)=0;
	virtual QList<Parameter*>* buildParameters(QList<Parameter*>*,Parameter*)=0;
	virtual Parameter* buildParameter(QString*)=0;
	virtual Parameter* buildParameter(QString*,Expression*)=0;
	virtual QList<Argument*>* buildArguments()=0;
	virtual QList<Argument*>* buildArguments(Argument*)=0;
	virtual QList<Argument*>* buildArguments(QList<Argument*>*,unsigned int,Argument*)=0;
	virtual unsigned int buildOptionalCommas()=0;
	virtual unsigned int buildOptionalCommas(unsigned int)=0;
	virtual Argument* buildArgument(Expression*)=0;
	virtual Argument* buildArgument(Variable*,Expression*)=0;
	virtual Expression* buildLiteral()=0;
	virtual Expression* buildLiteral(bool)=0;
	virtual Expression* buildLiteral(decimal*)=0;
	virtual Expression* buildLiteral(decimal*,QString*)=0;
	virtual Expression* buildLiteral(QString*)=0;
	virtual Variable* buildVariable(QString*)=0;
	virtual Expression* buildVariable(Variable*)=0;
	virtual Variable* buildVariable(QString*,Variable::Storage_e)=0;
	virtual Expression* buildExpression(Expression*,QString*)=0;
	virtual Expression* buildExpression(Expression*)=0;
	virtual Expression* buildExpression(Expression::Operator_e,Expression*)=0;
	virtual Expression* buildExpression(Expression*,Expression::Operator_e,Expression*)=0;
	virtual Expression* buildExpression(Expression*,Expression*,Expression*)=0;
	virtual Expression* buildExpression(QList<Expression*>*,int)=0;
	virtual QList<Expression*>* buildVector()=0;
	virtual QList<Expression*>* buildVector(Expression*)=0;
	virtual QList<Expression*>* buildVector(QList<Expression*>*,unsigned int,Expression*)=0;
	virtual Expression* buildRange(Expression*,Expression*)=0;
	virtual Expression* buildRange(Expression*,Expression*,Expression*)=0;
	virtual Expression* buildComplex(Expression*,Expression*,Expression*,Expression*)=0;
	virtual Invocation* buildInvocation(QString*,QList<Argument*>*)=0;
	virtual Invocation* buildInvocation(QString*,Invocation*)=0;
	virtual void reportSyntaxError(QString)=0;
};

#endif // ABSTRACTSYNTAXTREEBUILDER_H
