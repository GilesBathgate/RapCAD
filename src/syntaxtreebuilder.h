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

#ifndef SYNTAXTREEBUILDER_H
#define SYNTAXTREEBUILDER_H

#include "abstractsyntaxtreebuilder.h"
#include "moduleimport.h"
#include "scriptimport.h"
#include "compoundstatement.h"
#include "assignstatement.h"
#include "ifelsestatement.h"
#include "returnstatement.h"
#include "forstatement.h"
#include "module.h"
#include "modulescope.h"
#include "function.h"
#include "functionscope.h"
#include "literal.h"
#include "binaryexpression.h"
#include "unaryexpression.h"
#include "ternaryexpression.h"
#include "vectorexpression.h"
#include "rangeexpression.h"
#include "invocation.h"

class SyntaxTreeBuilder : public AbstractSyntaxTreeBuilder
{
public:
	SyntaxTreeBuilder();
	~SyntaxTreeBuilder();
	void buildScript(Declaration*);
	void buildScript(QVector<Declaration*>*);
	Declaration* buildUse(QString*);
	Declaration* buildUse(QString*,QString*);
	Declaration* buildImport(QString*,QString*);
	Declaration* buildImport(QString*,QString*,QVector<Parameter*>*);
	Declaration* buildDeclaration(Declaration*);
	QVector<Declaration*>* buildDeclarations();
	QVector<Declaration*>* buildDeclarations(Declaration*);
	QVector<Declaration*>* buildDeclarations(QVector<Declaration*>*);
	QVector<Declaration*>* buildDeclarations(QVector<Declaration*>*,Declaration*);
	Statement* buildStatement(Statement*);
	Statement* buildStatement(QVector<Statement*>*);
	Statement* buildStatement(Variable*,Expression*);
	Statement* buildStatement(QString*,Variable::Type_e,Expression*);
	Statement* buildReturnStatement(Expression*);
	Statement* buildIfElseStatement(Expression*,Statement*);
	Statement* buildIfElseStatement(Expression*,Statement*,Statement*);
	Statement* buildForStatement(QVector<Argument*>*,Statement*);
	QVector<Statement*>* buildStatements();
	QVector<Statement*>* buildStatements(Statement*);
	QVector<Statement*>* buildStatements(QVector<Statement*>*);
	QVector<Statement*>* buildStatements(QVector<Statement*>*,Statement*);
	Declaration* buildModule(QString*,QVector<Parameter*>*,Scope*);
	Declaration* buildFunction(QString*,QVector<Parameter*>*,Scope*);
	Scope* buildScope(QVector<Declaration*>*);
	Scope* buildScope(Instance*);
	Scope* buildScope(Expression*);
	Scope* buildScope(QVector<Statement*>*);
	Instance* buildInstance(Instance*);
	Instance* buildInstance(QString*,Instance*);
	Instance* buildInstance(Instance::Type_e,Instance*);
	Instance* buildInstance(QString*,QVector<Argument*>*);
	Instance* buildInstance(Instance*,QVector<Statement*>*);
	QVector<Parameter*>* buildParameters();
	QVector<Parameter*>* buildParameters(Parameter*);
	QVector<Parameter*>* buildParameters(QVector<Parameter*>*,Parameter*);
	Parameter* buildParameter(QString*);
	Parameter* buildParameter(QString*,Expression*);
	QVector<Argument*>* buildArguments();
	QVector<Argument*>* buildArguments(Argument*);
	QVector<Argument*>* buildArguments(QVector<Argument*>*,unsigned int,Argument*);
	Argument* buildArgument(Expression*);
	Argument* buildArgument(Variable*,Expression*);
	unsigned int buildOptionalCommas();
	unsigned int buildOptionalCommas(unsigned int);
	Expression* buildLiteral();
	Expression* buildLiteral(bool);
	Expression* buildLiteral(double value);
	Expression* buildLiteral(QString* value);
	Variable* buildVariable(QString* name);
	Expression* buildVariable(Variable*);
	Variable* buildVariable(QString*,Variable::Type_e);
	Expression* buildExpression(Expression*,QString*);
	Expression* buildExpression(Expression*);
	Expression* buildExpression(Expression::Operator_e,Expression*);
	Expression* buildExpression(Expression*,Expression::Operator_e,Expression*);
	Expression* buildExpression(Expression*,Expression*,Expression*);
	Expression* buildExpression(QVector<Expression*>*);
	QVector<Expression*>* buildVector();
	QVector<Expression*>* buildVector(Expression*);
	QVector<Expression*>* buildVector(QVector<Expression*>*,unsigned int,Expression*);
	Expression* buildRange(Expression*,Expression*);
	Expression* buildRange(Expression*,Expression*,Expression*);
	Expression* buildInvocation(QString*,QVector<Argument*>*);

	Script* getResult();
private:
	Script* script;
};

#endif // SYNTAXTREEBUILDER_H
