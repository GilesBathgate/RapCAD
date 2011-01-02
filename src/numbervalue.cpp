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

#include "numbervalue.h"
#include "vectorvalue.h"

NumberValue::NumberValue(double value)
{
	this->number=value;
}

QString NumberValue::getValueString()
{
	return QString().setNum(this->number,'g',16);
}

Value* NumberValue::operator+(Value& v)
{
    return operation(v,Expression::Add);
}

Value* NumberValue::operator-(Value& v)
{
    return operation(v,Expression::Subtract);
}

Value* NumberValue::operation(Value& v, Expression::Operator_e e)
{
    NumberValue* that = dynamic_cast<NumberValue*>(&v);
    if(that){
	double result=basicOperation<double>(this->number,e,that->number);
	return new NumberValue(result);
    }
    VectorValue* vec = dynamic_cast<VectorValue*>(&v);
    if(vec) {
	return Value::operation(vec,e,this);
    }
}
