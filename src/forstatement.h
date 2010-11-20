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

#ifndef FORSTATEMENT_H
#define FORSTATEMENT_H

#include <QString>
#include <QVector>
#include "statement.h"
#include "argument.h"

class ForStatement : public Statement
{
public:
    ForStatement();
    ~ForStatement();

    void setArguments(QVector<Argument*>);
    void setStatement(Statement*);
    QString toString();
private:
    QVector<Argument*> arguments;
    Statement* statement;

};

#endif // FORSTATEMENT_H
