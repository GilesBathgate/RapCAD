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

#include "complexvalue.h"
#include "booleanvalue.h"
#include "numbervalue.h"
#include "rmath.h"
#include "valuefactory.h"

ComplexValue::ComplexValue(Value& r, const QList<Value*>& i) :
	real(r),
	imaginary(i)
{
}

QString ComplexValue::getValueString() const
{
	QString result;
	result.append("<");
	result.append(real.getValueString());
	for(Value* v: imaginary) {
		result.append(",");
		result.append(v->getValueString());
	}
	result.append(">");
	return result;
}

void ComplexValue::toQuaternion(decimal& w,decimal& x,decimal& y,decimal& z)
{
	if(imaginary.size()>2) {
		auto* nw=dynamic_cast<NumberValue*>(&real);
		auto* nx=dynamic_cast<NumberValue*>(imaginary.at(0));
		auto* ny=dynamic_cast<NumberValue*>(imaginary.at(1));
		auto* nz=dynamic_cast<NumberValue*>(imaginary.at(2));
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

Value& ComplexValue::operation(Operators e)
{
	if(e==Operators::Length) {
		//l = sqrt(w^2+x^2+y^2,z^2)
		Value* n=Value::evaluate(&real,Operators::Multiply,&real);
		for(Value* i: getImaginary()) {
			Value& r=Value::evaluate(*i,Operators::Multiply,*i);
			n=Value::evaluate(n,Operators::Add,&r);
		}
		auto* l=dynamic_cast<NumberValue*>(n);
		if(l)
			return factory.createNumber(r_sqrt(l->getNumber()));
		return factory.createUndefined();
	}
	return *this;
}

Value& ComplexValue::operation(Value& v, Operators e)
{
	auto* c=dynamic_cast<ComplexValue*>(&v);
	if(c)
		return operation(*c,e);

	auto* n=dynamic_cast<NumberValue*>(&v);
	if(n)
		return operation(*n,e);

	return Value::operation(v,e);
}

Value& ComplexValue::operation(ComplexValue& c,Operators e)
{
	if(imaginary.size()>2&&c.imaginary.size()>2) {
		Value& w1=real;
		Value& w2=c.real;

		Value& x1=*imaginary.at(0);
		Value& x2=*c.imaginary.at(0);

		Value& y1=*imaginary.at(1);
		Value& y2=*c.imaginary.at(1);

		Value& z1=*imaginary.at(2);
		Value& z2=*c.imaginary.at(2);

		if(e==Operators::Multiply) {
			//(Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
			//(Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
			//(Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
			//(Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2)
			Value& w = (w1 * w2) - (x1 * x2) - (y1 * y2) - (z1 * z2);
			Value& x = (w1 * x2) + (x1 * w2) + (y1 * z2) - (z1 * y2);
			Value& y = (w1 * y2) - (x1 * z2) + (y1 * w2) + (z1 * x2);
			Value& z = (w1 * z2) + (x1 * y2) - (y1 * x2) + (z1 * w2);

			QList<Value*> i {&x,&y,&z};
			return factory.createComplex(w,i);
		}
		if(e==Operators::Equal||e==Operators::NotEqual) {
			Value& eqRe=Value::evaluate(w1,e,w2);
			bool eq=eqRe.isTrue();
			if(e==Operators::NotEqual && !eq)
				return factory.createBoolean(true);
			if(eq)
				for(auto i=0; i<3; ++i) {
					Value& eqIm=Value::evaluate(*imaginary.at(i),e,*c.imaginary.at(i));
					if(e==Operators::NotEqual && eqIm.isTrue())
						return factory.createBoolean(true);
					if(eqIm.isFalse())
						eq=false;
				}
			return factory.createBoolean(eq);
		}
	}

	return Value::operation(c,e);
}

Value& ComplexValue::operation(NumberValue& n,Operators e)
{
	if(e==Operators::Divide) {
		Value& w=Value::evaluate(real,Operators::Divide,n);
		QList<Value*> result;
		for(Value* i: getImaginary()) {
			Value& a=Value::evaluate(*i,Operators::Divide,n);
			result.append(&a);
		}
		return factory.createComplex(w,result);
	}

	return Value::operation(n,e);
}

const QList<Value*> ComplexValue::getImaginary() const
{
	return imaginary;
}
