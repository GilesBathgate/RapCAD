/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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
#include "intervalvalue.h"
#include "valuefactory.h"

IntervalValue::IntervalValue(Value& l, Value& u) :
	lower(l),
	upper(u)
{
}

QString IntervalValue::getValueString() const
{
	auto& two = ValueFactory::createNumber(2);
	auto& t = (upper-lower)/two;
	auto& n = upper-t;
	return QString("%1\u00B1%2").arg(n.getValueString(),t.getValueString());
}

Value& IntervalValue::operation(Operators op)
{
	if(op==Operators::Add) {
		return *this;
	}
	if(op==Operators::Subtract) {
		Value& a=Value::evaluate(upper,op);
		Value& b=Value::evaluate(lower,op);
		return ValueFactory::createInterval(a,b);
	}

	return ValueFactory::createUndefined();
}

Value& IntervalValue::operation(Value& v,Operators op)
{
	auto* i=dynamic_cast<IntervalValue*>(&v);
	if(i)
		return operation(*i,op);

	return Value::operation(v,op);
}

Value& IntervalValue::operation(IntervalValue& i,Operators op)
{
	/* Interval arithmetic has the following rules:
	 * x+y = [a+c,b+d]
	 * x-y = [a-d,b-c]
	 * x*y = [min(a*c,a*d,b*c,b*d),max(a*c,a*d,b*c,b*d)]
	 * x/y = [min(a/c,a/d,b/c,b/d),max(a/c,a/d,b/c,b/d)]
	 */
	Value& a=lower;
	Value& b=upper;
	Value& c=i.lower;
	Value& d=i.upper;
	if(op==Operators::Equal) {
		bool result=compare(a,op,c)&&compare(b,op,d);
		return ValueFactory::createBoolean(result);
	}
	if(op==Operators::NotEqual) {
		bool result=compare(a,op,c)||compare(b,op,d);
		return ValueFactory::createBoolean(result);
	}
	if(op==Operators::Add) {
		return ValueFactory::createInterval(a+c,b+d);
	}
	if(op==Operators::Subtract) {
		return ValueFactory::createInterval(a-d,b-c);
	}
	if(op==Operators::Multiply||op==Operators::Divide) {

		Value& ac=Value::evaluate(a,op,c);
		Value& ad=Value::evaluate(a,op,d);
		Value& bc=Value::evaluate(b,op,c);
		Value& bd=Value::evaluate(b,op,d);
		QList<Value*> vals {&ac,&ad,&bc,&bd};
		Value& lower=compareAll(vals,Operators::LessThan);
		Value& upper=compareAll(vals,Operators::GreaterThan);

		return ValueFactory::createInterval(lower,upper);
	}

	return ValueFactory::createUndefined();
}
