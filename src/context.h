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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QHash>
#include "value.h"
#include "module.h"
#include "function.h"
#include "scope.h"

class Context
{
public:
    Context();

    Context* parent;


    QVector<Value*> arguments;
    QVector<Value*> parameters;
    Value* currentvalue;
    Scope* currentscope;
    QString currentname;

    Value* lookupvariable(QString);
    void addvariable(Value*);

    Module* lookupmodule(QString);
    void addmodule(Module* mod);

    Function* lookupfunction(QString);
    void addfunction(Function*);

    void args(QVector<Value*>,QVector<Value*>);
private:
    QHash<QString, Value*> variables;
    QHash<QString,Module*> modules;
    QHash<QString,Function*> functions;
    bool contains(QVector<Value*>,QString);
};

#endif // CONTEXT_H
