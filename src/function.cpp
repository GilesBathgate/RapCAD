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

#include "function.h"
#include "functioncontext.h"

Function::Function()
{
    parameters=NULL;
    context=NULL;
}

Function::~Function()
{
    if(parameters)
	for(int i=0; i<parameters->size(); i++)
	    delete parameters->at(i);
    delete parameters;

    delete context;
}

QString Function::toString()
{
    QString result;
    result.append("Function: ");
    result.append(this->name);
    result.append("\n");
    result.append("Parameters: ");
    QVector<Parameter*>* params = this->parameters;
    if(params)
    {
	for(int i=0; i<params->size(); i++)
	    result.append(params->at(i)->toString());
    }
    result.append(" =\n");
    result.append(context->toString());
    result.append("\n");
    return result;
}

QString Function::getName()
{
    return this->name;
}

void Function::setName(QString name)
{
    this->name = name;
}


QVector<Parameter*>* Function::getParameters()
{
    return this->parameters;
}

void Function::setParameters(QVector<Parameter*>* params)
{
    this->parameters = params;
}

void Function::setContext(Context * ctx)
{
    this->context = ctx;
}
