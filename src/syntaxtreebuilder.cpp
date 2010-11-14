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

Context* SyntaxTreeBuilder::BuildContext(Instance* inst)
{
    ModuleContext* result = new ModuleContext();
    QVector<Declaration*>* decls = new QVector<Declaration*>();
    decls->append(inst);
    result->setDeclarations(decls);
    return result;
}

Instance* SyntaxTreeBuilder::BuildInstance(Instance* inst)
{
    return inst;
}

Instance* SyntaxTreeBuilder::BuildInstance(QString name,QVector<Argument*>* args)
{
    Instance* result = new Instance();
    result->setName(name);
    result->setArguments(args);
    return result;
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

QVector<Argument*>* SyntaxTreeBuilder::BuildArguments()
{
    return NULL;
}

QVector<Argument*>* SyntaxTreeBuilder::BuildArguments(Argument* arg)
{
    QVector<Argument*>* result = new QVector<Argument*>();
    result->append(arg);
    return result;
}

QVector<Argument*>* SyntaxTreeBuilder::BuildArguments(QVector<Argument*>* args,unsigned int count,Argument* arg)
{
    for(unsigned int i=0; i<count; i++)
	args->append(new Argument());
    args->append(arg);
    return args;
}

Argument* SyntaxTreeBuilder::BuildArgument(Expression* exp)
{
    Argument* result = new Argument();
    result->setExpression(exp);
    return result;
}

Argument* SyntaxTreeBuilder::BuildArgument(Variable* var,Expression* exp)
{
    Argument* result = new Argument();
    result->setVariable(var);
    result->setExpression(exp);
    return result;
}

unsigned int SyntaxTreeBuilder::BuildOptionalCommas()
{
    return 0;
}

unsigned int SyntaxTreeBuilder::BuildOptionalCommas(unsigned int count)
{
    return count+1;
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
