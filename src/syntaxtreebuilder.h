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

#ifndef SYNTAXTREEBUILDER_H
#define SYNTAXTREEBUILDER_H

#include "abstractsyntaxtreebuilder.h"

class SyntaxTreeBuilder : public AbstractSyntaxTreeBuilder
{
public:
    SyntaxTreeBuilder();
    void BuildScript(QVector<Declaration*>*);
    QVector<Declaration*>* BuildDeclarations();
    QVector<Declaration*>* BuildDeclarations(Declaration*);
    QVector<Declaration*>* BuildDeclarations(QVector<Declaration*>*);
    QVector<Declaration*>* BuildDeclarations(QVector<Declaration*>*,Declaration*);
    Declaration* BuildModule(QString name,QVector<Parameter*>*,Context*);
    Context* BuildContext(QVector<Declaration*>*);
    Context* BuildContext(Instance*);
    Instance* BuildInstance(Instance*);
    Instance* BuildInstance(QString,QVector<Argument*>*);
    QVector<Parameter*>* BuildParameters();
    QVector<Parameter*>* BuildParameters(Parameter*);
    QVector<Parameter*>* BuildParameters(QVector<Parameter*>*,Parameter*);
    Parameter* BuildParameter(QString);
    Parameter* BuildParameter(QString,Expression*);
    QVector<Argument*>* BuildArguments();
    QVector<Argument*>* BuildArguments(Argument*);
    QVector<Argument*>* BuildArguments(QVector<Argument*>*,Argument*);
    Argument* BuildArgument(Expression*);
    Argument* BuildArgument(Variable*,Expression*);
    Expression* BuildLiteral();
    Expression* BuildLiteral(bool);
    Expression* BuildLiteral(double value);
    Expression* BuildLiteral(char* value);
    Variable* BuildVariable(QString name);
    Expression* BuildVariable(Variable*);
    Expression* BuildExpression(Expression*,Expression::Operator_e,Expression*);

    void Print();
private:
    QVector<Declaration*>* declarations;
};

#endif // SYNTAXTREEBUILDER_H
