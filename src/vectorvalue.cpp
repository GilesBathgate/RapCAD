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

#include "vectorvalue.h"
#include "numbervalue.h"
#include "vectoriterator.h"
#include "rangevalue.h"
#include "booleanvalue.h"
#include "rmath.h"
#include "onceonly.h"
#include "valuefactory.h"

VectorValue::VectorValue(const QList<Value*>& values) : elements(values)
{
}

QString VectorValue::getValueString() const
{
	QString result;
	result.append("[");
	OnceOnly first;
	for(Value* v: elements) {
		if(!first())
			result.append(",");
		result.append(v->getValueString());
	}
	result.append("]");
	return result;
}

bool VectorValue::isTrue() const
{
	return !elements.empty();
}

VectorValue* VectorValue::toVector(int)
{
	return this;
}

Value* VectorValue::toNumber()
{
	if(elements.size()==1)
		return elements.at(0)->toNumber();
	return factory.createUndefined();
}

Point VectorValue::getPoint() const
{

	NumberValue* nx=nullptr;
	NumberValue* ny=nullptr;
	NumberValue* nz=nullptr;
	int s=elements.size();
	if(s>0)
		nx=dynamic_cast<NumberValue*>(elements.at(0));
	if(s>1)
		ny=dynamic_cast<NumberValue*>(elements.at(1));
	if(s>2)
		nz=dynamic_cast<NumberValue*>(elements.at(2));

	decimal x=0.0;
	decimal y=0.0;
	decimal z=0.0;
	if(nx)  {
		x=nx->getNumber();
	}
	if(ny) {
		y=ny->getNumber();
	}
	if(nz) {
		z=nz->getNumber();
	}
	return Point(x,y,z);
}

Value* VectorValue::getIndex(NumberValue* n)
{
	int i=n->toInteger();
	if(i<0||i>=elements.size()) return factory.createUndefined();
	return elements.at(i);
}

ValueIterator* VectorValue::createIterator()
{
	return new VectorIterator(elements);
}

QList<Value*> VectorValue::getElements()
{
	return elements;
}

Value* VectorValue::operation(Operators e)
{
	if(e==Operators::Length) {
		Value* v=Value::operation(this,Operators::Multiply,this);
		auto* n=dynamic_cast<NumberValue*>(v);
		if(n)
			return factory.createNumber(r_sqrt(n->getNumber()));
		return factory.createUndefined();
	}
	QList<Value*> result;
	for(Value* c: elements)
		result.append(Value::operation(c,e));
	return factory.createVector(result);
}

Value* VectorValue::operation(Value& v, Operators e)
{
	QList<Value*> result;
	auto* vec=dynamic_cast<VectorValue*>(&v);
	if(vec) {
		QList<Value*> a=getElements();
		QList<Value*> b=vec->getElements();

		if(e==Operators::CrossProduct) {
			int s=a.size();
			if(s<2||s>3||s!=b.size())
				return factory.createUndefined();

			//[a1*b2 - a2*b1, a2*b0 - a0*b2, a0*b1 - a1*b0]
			Value* a0b1=Value::operation(a.at(0),Operators::Multiply,b.at(1));
			Value* a1b0=Value::operation(a.at(1),Operators::Multiply,b.at(0));
			Value* z=Value::operation(a0b1,Operators::Subtract,a1b0);

			if(s==2)
				return z;

			Value* a1b2=Value::operation(a.at(1),Operators::Multiply,b.at(2));
			Value* a2b1=Value::operation(a.at(2),Operators::Multiply,b.at(1));
			Value* a2b0=Value::operation(a.at(2),Operators::Multiply,b.at(0));
			Value* a0b2=Value::operation(a.at(0),Operators::Multiply,b.at(2));
			Value* x=Value::operation(a1b2,Operators::Subtract,a2b1);
			Value* y=Value::operation(a2b0,Operators::Subtract,a0b2);

			result.append(x);
			result.append(y);
			result.append(z);
			return factory.createVector(result);

		}
		if(e==Operators::Multiply||e==Operators::DotProduct) {
			int s=std::min(a.size(),b.size());
			if(s<=0)
				return factory.createUndefined();
			Value* total=factory.createNumber(0.0);
			for(auto i=0; i<s; ++i) {
				Value* r=Value::operation(a.at(i),Operators::Multiply,b.at(i));
				total=Value::operation(total,Operators::Add,r);
			}
			return total;
		}
		if(e==Operators::Divide) {
			//TODO vector division?
			return factory.createUndefined();
		}
		if(e==Operators::Length) {
			Value* a2=Value::operation(this,Operators::Multiply,this);
			Value* b2=Value::operation(&v,Operators::Multiply,&v);
			Value* n=Value::operation(a2,Operators::Multiply,b2);
			auto* l=dynamic_cast<NumberValue*>(n);
			if(l)
				return factory.createNumber(r_sqrt(l->getNumber()));
			return factory.createUndefined();
		}
		if(e==Operators::Concatenate) {
			result=a;
			result.append(b);
		} else if(e==Operators::Equal||e==Operators::NotEqual) {
			bool eq=(a.size()==b.size());
			if(e==Operators::NotEqual && !eq)
				return factory.createBoolean(true);
			if(eq)
				for(auto i=0; i<a.size(); ++i) {
					Value* eqVec=Value::operation(a.at(i),e,b.at(i));
					if(e==Operators::NotEqual && eqVec->isTrue())
						return factory.createBoolean(true);
					if(eqVec->isFalse())
						eq=false;
				}
			return factory.createBoolean(eq);
		} else {
			//Apply componentwise operations
			e=convertOperation(e);
			int as=a.size();
			int bs=b.size();
			for(auto i=0; i<as||i<bs; ++i) {
				Value* r=nullptr;
				if(as<bs&&i>=as) {
					r=b.at(i);
				} else if(bs<as&&i>=bs) {
					r=a.at(i);
				} else {
					r=Value::operation(a.at(i),e,b.at(i));
				}
				result.append(r);
			}
		}
		return factory.createVector(result);
	}

	auto* num = dynamic_cast<NumberValue*>(&v);
	if(num) {
		if(e==Operators::Concatenate) {
			QList<Value*> a=getElements();
			result=a;
			result.append(num);
			return factory.createVector(result);
		}
		if(e==Operators::Exponent) {
			QList<Value*> a=getElements();
			Value* total=factory.createNumber(0.0);
			for(Value* c: a) {
				Value* r=Value::operation(c,e,num);
				total=Value::operation(total,Operators::Add,r);
			}
			return total;
		}
		if(e==Operators::Index) {
			return getIndex(num);
		}
		if(e==Operators::CrossProduct) {
			return factory.createUndefined();
		}

		QList<Value*> a=getElements();
		e=convertOperation(e);
		for(Value* c: a)
			result.append(Value::operation(c,e,num));

		return factory.createVector(result);

	}

	return Value::operation(v,e);
}

Operators VectorValue::convertOperation(Operators e)
{
	switch(e) {
		case Operators::ComponentwiseMultiply:
			return Operators::Multiply;
		case Operators::ComponentwiseDivide:
			return Operators::Divide;
		default:
			return e;
	}
}
