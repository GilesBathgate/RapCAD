/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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

#ifndef COMPLEXVALUE_H
#define COMPLEXVALUE_H

#include "value.h"
#include <QList>

class ComplexValue : public Value
{
public:
	ComplexValue(Value*,const QList<Value*>&);
	QString getValueString() const override;
	void toQuaternion(decimal&,decimal&,decimal&,decimal&);
private:
	ComplexValue();
	Value* operation(Expression::Operator_e) override;
	Value* operation(Value&,Expression::Operator_e) override;
	Value* real;
	QList<Value*> imaginary;
};

#endif // COMPLEXVALUE_H
