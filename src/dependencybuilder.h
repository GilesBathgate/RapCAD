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

#ifndef DEPENDENCYBUILDER_H
#define DEPENDENCYBUILDER_H

#include "abstractsyntaxtreebuilder.h"

class DependencyBuilder : public AbstractSyntaxTreeBuilder
{
public:
    DependencyBuilder();
    ~DependencyBuilder();
    void BuildScript(QVector<Declaration*>*);
    QVector<Declaration*>* BuildDeclarations();
    QVector<Declaration*>* BuildDeclarations(Declaration*);
    QVector<Declaration*>* BuildDeclarations(QVector<Declaration*>*);
    QVector<Declaration*>* BuildDeclarations(QVector<Declaration*>*,Declaration*);
    Statement* BuildStatement(Statement*);
    Statement* BuildStatement(QVector<Statement*>*);
    Statement* BuildStatement(Value*,Expression*);
    Statement* BuildStatement(QString*,Value::Type_e,Expression*);
    Statement* BuildStatement(IfElseStatement*);
    ReturnStatement* BuildReturnStatement(Expression*);
    IfElseStatement* BuildIfElseStatement(Expression*,Statement*);
    IfElseStatement* BuildIfElseStatement(Expression*,Statement*,Statement*);
    ForStatement* BuildForStatement(QVector<Argument*>*,Statement*);
    QVector<Statement*>* BuildStatements();
    QVector<Statement*>* BuildStatements(Statement*);
    QVector<Statement*>* BuildStatements(QVector<Statement*>*);
    QVector<Statement*>* BuildStatements(QVector<Statement*>*,Statement*);
    Declaration* BuildModule(QString*,QVector<Parameter*>*,Context*);
    Declaration* BuildFunction(QString*,QVector<Parameter*>*,Context*);
    Context* BuildContext(QVector<Declaration*>*);
    Context* BuildContext(Instance*);
    Context* BuildContext(Expression*);
    Context* BuildContext(QVector<Statement*>*);
    Instance* BuildInstance(Instance*);
    Instance* BuildInstance(Instance::Type_e,Instance*);
    Instance* BuildInstance(QString*,QVector<Argument*>*);
    Instance* BuildInstance(Instance*,QVector<Instance*>*);
    QVector<Instance*>* BuildInstances();
    QVector<Instance*>* BuildInstances(Instance*);
    QVector<Instance*>* BuildInstances(QVector<Instance*>*);
    QVector<Instance*>* BuildInstances(QVector<Instance*>*,Instance*);
    QVector<Parameter*>* BuildParameters();
    QVector<Parameter*>* BuildParameters(Parameter*);
    QVector<Parameter*>* BuildParameters(QVector<Parameter*>*,Parameter*);
    Parameter* BuildParameter(QString*);
    Parameter* BuildParameter(QString*,Expression*);
    QVector<Argument*>* BuildArguments();
    QVector<Argument*>* BuildArguments(Argument*);
    QVector<Argument*>* BuildArguments(QVector<Argument*>*,unsigned int,Argument*);
    Argument* BuildArgument(Expression*);
    Argument* BuildArgument(Value*,Expression*);
    unsigned int BuildOptionalCommas();
    unsigned int BuildOptionalCommas(unsigned int);
    Expression* BuildLiteral();
    Expression* BuildLiteral(bool);
    Expression* BuildLiteral(double value);
    Expression* BuildLiteral(QString* value);
    Value* BuildVariable(QString* name);
    Expression* BuildVariable(Value*);
    Value* BuildVariable(QString*,Value::Type_e);
    Expression* BuildExpression(Expression*,QString*);
    Expression* BuildExpression(Expression*);
    Expression* BuildExpression(Expression::Operator_e,Expression*);
    Expression* BuildExpression(Expression*,Expression::Operator_e,Expression*);
    Expression* BuildTernaryExpression(Expression*,Expression*,Expression*);
    Expression* BuildExpression(QVector<Expression*>*);
    QVector<Expression*>* BuildVector();
    QVector<Expression*>* BuildVector(Expression*);
    QVector<Expression*>* BuildVector(QVector<Expression*>*,Expression*);
    Expression* BuildRange(Expression*,Expression*);
    Expression* BuildRange(Expression*,Expression*,Expression*);
    Expression* BuildInvocation(QString*,QVector<Argument*>*);
};

#endif // DEPENDENCYBUILDER_H
