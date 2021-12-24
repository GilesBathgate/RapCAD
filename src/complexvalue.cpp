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
#include "numbervalue.h"
#include "booleanvalue.h"
#include "rmath.h"
#include "valuefactory.h"

ComplexValue::ComplexValue(Value* r, const QList<Value*>& i) :
	real(r),
	imaginary(i)
{
}

QString ComplexValue::getValueString() const
{
	QString result;
	result.append("<");
	result.append(real->getValueString());
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
		auto* nw=dynamic_cast<NumberValue*>(real);
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
		Value* n=Value::operation(real,Operators::Multiply,real);
		for(Value* i: imaginary) {
			Value* r=Value::operation(i,Operators::Multiply,i);
			n=Value::operation(n,Operators::Add,r);
		}
		auto* l=dynamic_cast<NumberValue*>(n);
		if(l)
			return factory.createNumber(r_sqrt(l->getNumber()));
		return factory.createUndefined();
	}
	return *this;
}

Value& ComplexValue::operation(Value& v, Operators op)
{
	auto* c=dynamic_cast<ComplexValue*>(&v);
	if(c) {
		if(imaginary.size()>2&&c->imaginary.size()>2) {
			Value* w1=real;
			Value* w2=c->real;

			Value* x1=imaginary.at(0);
			Value* x2=c->imaginary.at(0);

			Value* y1=imaginary.at(1);
			Value* y2=c->imaginary.at(1);

			Value* z1=imaginary.at(2);
			Value* z2=c->imaginary.at(2);

			if(op==Operators::Multiply) {
				//(Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
				//(Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
				//(Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
				//(Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2)
				Value* w1w2 = Value::operation(w1,op,w2);
				Value* x1x2 = Value::operation(x1,op,x2);
				Value* y1y2 = Value::operation(y1,op,y2);
				Value* z1z2 = Value::operation(z1,op,z2);
				Value* w = Value::operation(w1w2,Operators::Subtract,x1x2);
				w = Value::operation(w,Operators::Subtract,y1y2);
				w = Value::operation(w,Operators::Subtract,z1z2);

				Value* w1x2 = Value::operation(w1,op,x2);
				Value* x1w2 = Value::operation(x1,op,w2);
				Value* y1z2 = Value::operation(y1,op,z2);
				Value* z1y2 = Value::operation(z1,op,y2);
				Value* x = Value::operation(w1x2,Operators::Add,x1w2);
				x = Value::operation(x,Operators::Add,y1z2);
				x = Value::operation(x,Operators::Subtract,z1y2);

				Value* w1y2 = Value::operation(w1,op,y2);
				Value* x1z2 = Value::operation(x1,op,z2);
				Value* y1w2 = Value::operation(y1,op,w2);
				Value* z1x2 = Value::operation(z1,op,x2);
				Value* y = Value::operation(w1y2,Operators::Subtract,x1z2);
				y = Value::operation(y,Operators::Add,y1w2);
				y = Value::operation(y,Operators::Add,z1x2);

				Value* w1z2 = Value::operation(w1,op,z2);
				Value* x1y2 = Value::operation(x1,op,y2);
				Value* y1x2 = Value::operation(y1,op,x2);
				Value* z1w2 = Value::operation(z1,op,w2);
				Value* z = Value::operation(w1z2,Operators::Subtract,x1y2);
				z = Value::operation(z,Operators::Add,y1x2);
				z = Value::operation(z,Operators::Add,z1w2);

				QList<Value*> i;
				i.append(x);
				i.append(y);
				i.append(z);

				return factory.createComplex(w,i);
			}
			if(op==Operators::Equal||op==Operators::NotEqual) {
				Value* eqRe=Value::operation(real,op,c->real);
				bool eq=eqRe->isTrue();
				if(op==Operators::NotEqual && !eq)
					return factory.createBoolean(true);
				if(eq)
					for(auto i=0; i<3; ++i) {
						Value* eqIm=Value::operation(imaginary.at(i),op,c->imaginary.at(i));
						if(op==Operators::NotEqual && eqIm->isTrue())
							return factory.createBoolean(true);
						if(eqIm->isFalse())
							eq=false;
					}
				return factory.createBoolean(eq);
			}
		}
	}

	auto* n=dynamic_cast<NumberValue*>(&v);
	if(n) {
		if(op==Operators::Divide) {
			Value* w=Value::operation(real,Operators::Divide,n);
			QList<Value*> result;
			for(Value* i: imaginary) {
				Value* a=Value::operation(i,Operators::Divide,n);
				result.append(a);
			}
			return factory.createComplex(w,result);
		}
	}

	return factory.createUndefined();
}
