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

#ifndef RANGEVALUE_H
#define RANGEVALUE_H

#include "vectorvalue.h"

class RangeValue : public VectorValue
{
public:
	RangeValue(Value&,Value&);
	RangeValue(Value&,Value&,Value&);
	QString getValueString() const override;
	Value& getIndex(NumberValue&) override;
	ValueIterator* createIterator() override;
	QList<Value*> getElements() override;

	Value& getStart() const;
	Value& getFinish() const;
	bool inRange(Value&);
private:
	Value& operation(Operators) override;
	Value& operation(Value&,Operators) override;
	Value& operation(RangeValue&,Operators);
	bool getReverse();
	Value& defaultStep();
	Value& start;
	Value& finish;
	bool reverse;
	Value& step;
};

#endif // RANGEVALUE_H
