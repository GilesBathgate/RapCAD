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

#ifndef ABSTRACTSYNTAXTREEBUILDER_H
#define ABSTRACTSYNTAXTREEBUILDER_H

#include <QVector>
#include "declaration.h"
#include "statement.h"
#include "variable.h"
#include "expression.h"
#include "argument.h"
#include "parameter.h"
#include "scope.h"
#include "instance.h"
#include "script.h"

class AbstractSyntaxTreeBuilder
{
public:
	virtual ~AbstractSyntaxTreeBuilder() {}
	virtual void buildScript(Declaration*)=0;
	virtual void buildScript(QVector<Declaration*>*)=0;
	virtual Declaration* buildUse(QString*)=0;
	virtual Declaration* buildUse(QString*,QString*)=0;
	virtual Declaration* buildDeclaration(Declaration*)=0;
	virtual QVector<Declaration*>* buildDeclarations()=0;
	virtual QVector<Declaration*>* buildDeclarations(Declaration*)=0;
	virtual QVector<Declaration*>* buildDeclarations(QVector<Declaration*>*)=0;
	virtual QVector<Declaration*>* buildDeclarations(QVector<Declaration*>*,Declaration*)=0;
	virtual Statement* buildStatement(Statement*)=0;
	virtual Statement* buildStatement(QVector<Statement*>*)=0;
	virtual Statement* buildStatement(Variable*,Expression*)=0;
	virtual Statement* buildStatement(QString*,Variable::Type_e,Expression*)=0;
	virtual Statement* buildReturnStatement(Expression*)=0;
	virtual Statement* buildIfElseStatement(Expression*,Statement*)=0;
	virtual Statement* buildIfElseStatement(Expression*,Statement*,Statement*)=0;
	virtual Statement* buildForStatement(QVector<Argument*>*,Statement*)=0;
	virtual QVector<Statement*>* buildStatements()=0;
	virtual QVector<Statement*>* buildStatements(Statement*)=0;
	virtual QVector<Statement*>* buildStatements(QVector<Statement*>*)=0;
	virtual QVector<Statement*>* buildStatements(QVector<Statement*>*,Statement*)=0;
	virtual Declaration* buildModule(QString*,QVector<Parameter*>*,Scope*)=0;
	virtual Declaration* buildFunction(QString*,QVector<Parameter*>*,Scope*)=0;
	virtual Scope* buildScope(QVector<Declaration*>*)=0;
	virtual Scope* buildScope(Instance*)=0;
	virtual Scope* buildScope(Expression*)=0;
	virtual Scope* buildScope(QVector<Statement*>*)=0;
	virtual Instance* buildInstance(Instance*)=0;
	virtual Instance* buildInstance(QString*,Instance*)=0;
	virtual Instance* buildInstance(Instance::Type_e,Instance*)=0;
	virtual Instance* buildInstance(QString*,QVector<Argument*>*)=0;
	virtual Instance* buildInstance(Instance*,QVector<Statement*>*)=0;
	virtual QVector<Parameter*>* buildParameters()=0;
	virtual QVector<Parameter*>* buildParameters(Parameter*)=0;
	virtual QVector<Parameter*>* buildParameters(QVector<Parameter*>*,Parameter*)=0;
	virtual Parameter* buildParameter(QString*)=0;
	virtual Parameter* buildParameter(QString*,Expression*)=0;
	virtual QVector<Argument*>* buildArguments()=0;
	virtual QVector<Argument*>* buildArguments(Argument*)=0;
	virtual QVector<Argument*>* buildArguments(QVector<Argument*>*,unsigned int,Argument*)=0;
	virtual unsigned int buildOptionalCommas()=0;
	virtual unsigned int buildOptionalCommas(unsigned int)=0;
	virtual Argument* buildArgument(Expression*)=0;
	virtual Argument* buildArgument(Variable*,Expression*)=0;
	virtual Expression* buildLiteral()=0;
	virtual Expression* buildLiteral(bool)=0;
	virtual Expression* buildLiteral(double value)=0;
	virtual Expression* buildLiteral(QString* value)=0;
	virtual Variable* buildVariable(QString* name)=0;
	virtual Expression* buildVariable(Variable*)=0;
	virtual Variable* buildVariable(QString*,Variable::Type_e)=0;
	virtual Expression* buildExpression(Expression*,QString*)=0;
	virtual Expression* buildExpression(Expression*)=0;
	virtual Expression* buildExpression(Expression::Operator_e,Expression*)=0;
	virtual Expression* buildExpression(Expression*,Expression::Operator_e,Expression*)=0;
	virtual Expression* buildExpression(Expression*,Expression*,Expression*)=0;
	virtual Expression* buildExpression(QVector<Expression*>*)=0;
	virtual QVector<Expression*>* buildVector()=0;
	virtual QVector<Expression*>* buildVector(Expression*)=0;
	virtual QVector<Expression*>* buildVector(QVector<Expression*>*,unsigned int,Expression*)=0;
	virtual Expression* buildRange(Expression*,Expression*)=0;
	virtual Expression* buildRange(Expression*,Expression*,Expression*)=0;
	virtual Expression* buildInvocation(QString*,QVector<Argument*>*)=0;

	virtual Script* getResult()=0;
};

#endif // ABSTRACTSYNTAXTREEBUILDER_H
