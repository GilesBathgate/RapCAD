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

#include "syntaxtreebuilder.h"
#include <stdio.h>

SyntaxTreeBuilder::SyntaxTreeBuilder()
{
    declarations=NULL;
}

void SyntaxTreeBuilder::BuildScript(QVector<Declaration*>* decls)
{
    declarations = decls;
}

QVector<Declaration*>* SyntaxTreeBuilder::BuildDeclarations()
{
    return NULL;
}

QVector<Declaration*>* SyntaxTreeBuilder::BuildDeclarations(QVector<Declaration*>* decls,Declaration* decl)
{
    decls->append(decl);
    return decls;
}

QVector<Declaration*>* SyntaxTreeBuilder::BuildDeclarations(QVector<Declaration*>* decls)
{
    return decls;
}

QVector<Declaration*>* SyntaxTreeBuilder::BuildDeclarations(Declaration* decl)
{
    QVector<Declaration*>* result = new QVector<Declaration*>();
    result->append(decl);
    return result;
}

Declaration* SyntaxTreeBuilder::BuildModule(QString name, QVector<Parameter*>* params, Context* ctx)
{
    Module* result = new Module();
    result->setName(name);
    result->setParameters(params);
    result->setContext(ctx);
    return result;
}

Context* SyntaxTreeBuilder::BuildContext(QVector<Declaration*>* decls)
{
    ModuleContext* result = new ModuleContext();
    result->setDeclarations(decls);
    return result;
}

Context* SyntaxTreeBuilder::BuildContext(Instance*)
{
    //TODO implement
    return NULL;
}

Instance* SyntaxTreeBuilder::BuildInstance(Instance* inst)
{
    return inst;
}

QVector<Parameter*>* SyntaxTreeBuilder::BuildParameters()
{
    return NULL;
}

QVector<Parameter*>* SyntaxTreeBuilder::BuildParameters(Parameter* param)
{
    QVector<Parameter*>* result = new QVector<Parameter*>();
    result->append(param);
    return result;
}

QVector<Parameter*>* SyntaxTreeBuilder::BuildParameters(QVector<Parameter*>* params,Parameter* param)
{
    params->append(param);
    return params;
}

Parameter* SyntaxTreeBuilder::BuildParameter(QString name)
{
    Parameter* result = new Parameter();
    result->setName(name);
    return result;
}

Parameter* SyntaxTreeBuilder::BuildParameter(QString name,Expression* expr)
{
    Parameter* result = new Parameter();
    result->setName(name);
    result->setExpression(expr);
    return result;
}

Expression* SyntaxTreeBuilder::BuildLiteral()
{
    return new Literal();
}

Expression* SyntaxTreeBuilder::BuildLiteral(bool value)
{
    Literal* result = new Literal();
    result->setValue(value);
    return result;
}

Expression* SyntaxTreeBuilder::BuildLiteral(double value)
{
    Literal* result = new Literal();
    result->setValue(value);
    return result;
}

Expression* SyntaxTreeBuilder::BuildLiteral(char* value)
{
    Literal* result = new Literal();
    result->setValue(QString(value));
    return result;
}

Variable* SyntaxTreeBuilder::BuildVariable(QString name)
{
    Variable* result = new Variable();
    result->setName(name);
    return result;
}

Expression* SyntaxTreeBuilder::BuildVariable(Variable* var)
{
    return var;
}

Expression* SyntaxTreeBuilder::BuildExpression(Expression* left ,Expression::Operator_e op, Expression* right)
{
    BinaryExpression* result = new BinaryExpression();
    result->setLeft(left);
    result->setRight(right);
    result->setOp(op);
    return result;
}

void SyntaxTreeBuilder::Print()
{
    if(declarations)
	for(int i=0; i<declarations->size(); i++)
	    printf("%s",declarations->at(i)->toString().toAscii().constData());

    printf("\nDone.\n");
}
