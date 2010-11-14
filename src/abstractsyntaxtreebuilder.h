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
#include "module.h"
#include "parameter.h"
#include "literal.h"
#include "variable.h"
#include "expression.h"
#include "binaryexpression.h"
#include "declaration.h"
#include "context.h"
#include "modulecontext.h"
#include "instance.h"

class AbstractSyntaxTreeBuilder
{
public:
    virtual void BuildScript(QVector<Declaration*>*)=0;
    virtual QVector<Declaration*>* BuildDeclarations()=0;
    virtual QVector<Declaration*>* BuildDeclarations(Declaration*)=0;
    virtual QVector<Declaration*>* BuildDeclarations(QVector<Declaration*>*)=0;
    virtual QVector<Declaration*>* BuildDeclarations(QVector<Declaration*>*,Declaration*)=0;
    virtual Declaration* BuildModule(QString name,QVector<Parameter*>*,Context*)=0;
    virtual Context* BuildContext(QVector<Declaration*>*)=0;
    virtual Context* BuildContext(Instance*)=0;
    virtual Instance* BuildInstance(Instance*)=0;
    virtual QVector<Parameter*>* BuildParameters()=0;
    virtual QVector<Parameter*>* BuildParameters(Parameter*)=0;
    virtual QVector<Parameter*>* BuildParameters(QVector<Parameter*>*,Parameter*)=0;
    virtual Parameter* BuildParameter(QString)=0;
    virtual Parameter* BuildParameter(QString,Expression*)=0;
    virtual Expression* BuildLiteral()=0;
    virtual Expression* BuildLiteral(bool)=0;
    virtual Expression* BuildLiteral(double value)=0;
    virtual Expression* BuildLiteral(char* value)=0;
    virtual Variable* BuildVariable(QString name)=0;
    virtual Expression* BuildVariable(Variable*)=0;
    virtual Expression* BuildExpression(Expression*,Expression::Operator_e,Expression*)=0;
};

#endif // ABSTRACTSYNTAXTREEBUILDER_H
