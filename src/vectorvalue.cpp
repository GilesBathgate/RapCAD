/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#include "booleanvalue.h"
#include "numbervalue.h"
#include "onceonly.h"
#include "point.h"
#include "rmath.h"
#include "valuefactory.h"
#include "vectoriterator.h"

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

VectorValue& VectorValue::toVector(int)
{
	return *this;
}

Value& VectorValue::toNumber()
{
	if(elements.size()==1)
		return elements.at(0)->toNumber();
	return ValueFactory::createUndefined();
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

Value& VectorValue::getIndex(NumberValue& n)
{
	int i=n.toInteger();
	if(i<0||i>=elements.size()) return ValueFactory::createUndefined();
	return *elements.at(i);
}

ValueIterator* VectorValue::createIterator()
{
	return new VectorIterator(elements);
}

const QList<Value*> VectorValue::getElements()
{
	return elements;
}

Value& VectorValue::operation(Operators e)
{
	if(e==Operators::Length) {
		Value& v=Value::evaluate(*this,Operators::Multiply,*this);
		auto* n=dynamic_cast<NumberValue*>(&v);
		if(n)
			return ValueFactory::createNumber(r_sqrt(n->getNumber()));
		return ValueFactory::createUndefined();
	}
	QList<Value*> result;
	for(Value* c: getElements())
		result.append(Value::evaluate(c,e));
	return ValueFactory::createVector(result);
}

Value& VectorValue::operation(Value& v,Operators e)
{
	auto* vec=dynamic_cast<VectorValue*>(&v);
	if(vec)
		return operation(*vec,e);

	auto* num=dynamic_cast<NumberValue*>(&v);
	if(num)
		return operation(*num,e);

	return Value::operation(v,e);
}

Value& VectorValue::operation(NumberValue& num,Operators e)
{
	QList<Value*> result;
	if(e==Operators::Concatenate) {
		result=getElements();
		result.append(&num);
		return ValueFactory::createVector(result);
	}
	if(e==Operators::Exponent) {
		const QList<Value*> a=getElements();
		Value* total=&ValueFactory::createNumber(0.0);
		for(Value* c: a) {
			Value& r=Value::evaluate(*c,e,num);
			total=Value::evaluate(total,Operators::Add,&r);
		}
		return *total;
	}
	if(e==Operators::Index) {
		return getIndex(num);
	}
	if(e==Operators::CrossProduct) {
		return ValueFactory::createUndefined();
	}

	const QList<Value*> a=getElements();
	e=convertOperation(e);
	for(Value* c: a)
		result.append(Value::evaluate(c,e,&num));

	return ValueFactory::createVector(result);
}

Value& VectorValue::operation(VectorValue& vec,Operators e)
{
	QList<Value*> result;
	QList<Value*> a=getElements();
	QList<Value*> b=vec.getElements();

	if(e==Operators::CrossProduct) {
		int s=a.size();
		if(s<2||s>3||s!=b.size())
			return ValueFactory::createUndefined();

		//[a1*b2 - a2*b1, a2*b0 - a0*b2, a0*b1 - a1*b0]
		Value& a0=*a.at(0);
		Value& b0=*b.at(0);
		Value& a1=*a.at(1);
		Value& b1=*b.at(1);
		Value& z = (a0 * b1) - (a1 * b0);

		if(s==2)
			return z;

		Value& a2=*a.at(2);
		Value& b2=*b.at(2);
		Value& x = (a1 * b2) - (a2 * b1);
		Value& y = (a2 * b0) - (a0 * b2);

		result.append(&x);
		result.append(&y);
		result.append(&z);
		return ValueFactory::createVector(result);

	}
	if(e==Operators::Multiply||e==Operators::DotProduct) {
		int s=std::min(a.size(),b.size());
		if(s<=0)
			return ValueFactory::createUndefined();
		Value* total=&ValueFactory::createNumber(0.0);
		for(auto i=0; i<s; ++i) {
			Value& r=Value::evaluate(*a.at(i),Operators::Multiply,*b.at(i));
			total=Value::evaluate(total,Operators::Add,&r);
		}
		return *total;
	}
	if(e==Operators::Divide) {
		//TODO vector division?
		return ValueFactory::createUndefined();
	}
	if(e==Operators::Length) {
		Value& a2=Value::evaluate(*this,Operators::Multiply,*this);
		Value& b2=Value::evaluate(vec,Operators::Multiply,vec);
		Value& n=Value::evaluate(a2,Operators::Multiply,b2);
		auto* l=dynamic_cast<NumberValue*>(&n);
		if(l)
			return ValueFactory::createNumber(r_sqrt(l->getNumber()));
		return ValueFactory::createUndefined();
	}
	if(e==Operators::Concatenate) {
		result=a;
		result.append(b);
	} else if(e==Operators::Equal||e==Operators::NotEqual) {
		bool eq=(a.size()==b.size());
		if(e==Operators::NotEqual && !eq)
			return ValueFactory::createBoolean(true);
		if(eq)
			for(auto i=0; i<a.size(); ++i) {
				Value& eqVec=Value::evaluate(*a.at(i),e,*b.at(i));
				if(e==Operators::NotEqual && eqVec.isTrue())
					return ValueFactory::createBoolean(true);
				if(eqVec.isFalse())
					eq=false;
			}
		return ValueFactory::createBoolean(eq);
	} else {
		//Apply componentwise operations
		e=convertOperation(e);
		int as=a.size();
		int bs=b.size();
		for(auto i=0; i<as||i<bs; ++i) {
			if(as<bs&&i>=as) {
				result.append(b.at(i));
			} else if(bs<as&&i>=bs) {
				result.append(a.at(i));
			} else {
				Value& r=Value::evaluate(*a.at(i),e,*b.at(i));
				result.append(&r);
			}
		}
	}
	return ValueFactory::createVector(result);
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
