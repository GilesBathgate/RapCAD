/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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

#ifndef VECTORVALUE_H
#define VECTORVALUE_H

#include <QList>
#include "value.h"
#include "point.h"
#include "numbervalue.h"

class VectorValue : public Value
{
public:
	explicit VectorValue(const QList<Value*>&);
	QString getValueString() const override;
	bool isTrue() const override;
	VectorValue& toVector(int) override;
	Value& toNumber() override;
	Point getPoint() const;
	virtual Value& getIndex(NumberValue&);
	ValueIterator* createIterator() override;
	virtual const QList<Value*> getElements();
protected:
	VectorValue() = default;
	Value& operation(Operators) override;
	Value& operation(Value&,Operators) override;
	Value& operation(VectorValue&,Operators);
private:
	Value& operation(NumberValue&,Operators);
	static Operators convertOperation(Operators);
	QList<Value*> elements;
};

#endif // VECTORVALUE_H
