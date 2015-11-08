/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

#include "complexvalue.h"
#include "numbervalue.h"

ComplexValue::ComplexValue(Value* r, QList<Value*> i)
{
	real=r;
	imaginary=i;
}

QString ComplexValue::getValueString() const
{
	QString result;
	result.append("<");
	result.append(real->getValueString());
	foreach(Value* v,imaginary) {
		result.append(",");
		result.append(v->getValueString());
	}
	result.append(">");
	return result;
}

void ComplexValue::toQuaternion(decimal &w,decimal &x,decimal &y,decimal &z)
{
	if(imaginary.size()>2) {
		NumberValue* nw=dynamic_cast<NumberValue*>(real);
		NumberValue* nx=dynamic_cast<NumberValue*>(imaginary.at(0));
		NumberValue* ny=dynamic_cast<NumberValue*>(imaginary.at(1));
		NumberValue* nz=dynamic_cast<NumberValue*>(imaginary.at(2));
		if(nw&&nx&&ny&&nz) {
			w=nw->getNumber();
			x=nx->getNumber();
			y=ny->getNumber();
			z=nz->getNumber();
			return;
		}
	}
	x=y=z=w=0;
}
