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

#include "vectorvalue.h"
#include "numbervalue.h"
#include "vectoriterator.h"
#include "rangevalue.h"
#include "booleanvalue.h"
#include "rmath.h"
#include "onceonly.h"

VectorValue::VectorValue()
{
}

VectorValue::VectorValue(const QList<Value*>& values) : children(values)
{
}

QString VectorValue::getValueString() const
{
	QString result;
	result.append("[");
	OnceOnly first;
	for(Value* v: children) {
		if(!first())
			result.append(",");
		result.append(v->getValueString());
	}
	result.append("]");
	return result;
}

bool VectorValue::isTrue() const
{
	return children.size()>0;
}

VectorValue* VectorValue::toVector(int)
{
	return this;
}

Value* VectorValue::toNumber()
{
	if(children.size()==1)
		return children.at(0)->toNumber();
	return Value::undefined();
}

Point VectorValue::getPoint() const
{

	NumberValue* nx=nullptr,*ny=nullptr,*nz=nullptr;
	int s=children.size();
	if(s>0)
		nx=dynamic_cast<NumberValue*>(children.at(0));
	if(s>1)
		ny=dynamic_cast<NumberValue*>(children.at(1));
	if(s>2)
		nz=dynamic_cast<NumberValue*>(children.at(2));

	decimal x=0,y=0,z=0;
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
	if(i<0||i>=children.size()) return undefined();
	return children.at(i);
}

ValueIterator* VectorValue::createIterator()
{
	return new VectorIterator(children);
}

QList<Value*> VectorValue::getChildren()
{
	return children;
}

Value* VectorValue::operation(Expression::Operator_e e)
{
	if(e==Expression::Length) {
		Value* v=Value::operation(this,Expression::Multiply,this);
		auto* n=dynamic_cast<NumberValue*>(v);
		if(n)
			return new NumberValue(r_sqrt(n->getNumber()));
		return Value::undefined();
	}
	QList<Value*> result;
	for(Value* c: children)
		result.append(Value::operation(c,e));
	return new VectorValue(result);
}

Value* VectorValue::operation(Value& v, Expression::Operator_e e)
{
	QList<Value*> result;
	auto* vec=dynamic_cast<VectorValue*>(&v);
	if(vec) {
		QList<Value*> a=getChildren();
		QList<Value*> b=vec->getChildren();

		if(e==Expression::CrossProduct) {
			int s=a.size();
			if(s<2||s>3||s!=b.size())
				return Value::undefined();

			//[a1*b2 - a2*b1, a2*b0 - a0*b2, a0*b1 - a1*b0]
			Value* a0b1=Value::operation(a.at(0),Expression::Multiply,b.at(1));
			Value* a1b0=Value::operation(a.at(1),Expression::Multiply,b.at(0));
			Value* z=Value::operation(a0b1,Expression::Subtract,a1b0);

			if(s==2)
				return z;

			Value* a1b2=Value::operation(a.at(1),Expression::Multiply,b.at(2));
			Value* a2b1=Value::operation(a.at(2),Expression::Multiply,b.at(1));
			Value* a2b0=Value::operation(a.at(2),Expression::Multiply,b.at(0));
			Value* a0b2=Value::operation(a.at(0),Expression::Multiply,b.at(2));
			Value* x=Value::operation(a1b2,Expression::Subtract,a2b1);
			Value* y=Value::operation(a2b0,Expression::Subtract,a0b2);

			result.append(x);
			result.append(y);
			result.append(z);
			return new VectorValue(result);

		} else if(e==Expression::Multiply||e==Expression::DotProduct) {
			int s=std::min(a.size(),b.size());
			if(s<=0)
				return Value::undefined();
			Value* total=new NumberValue(0.0);
			for(auto i=0; i<s; ++i) {
				Value* r=Value::operation(a.at(i),Expression::Multiply,b.at(i));
				total=Value::operation(total,Expression::Add,r);
			}
			return total;
		} else if(e==Expression::Divide) {
			//TODO vector division?
			return this;
		} else if(e==Expression::Length) {
			Value* a=Value::operation(this,Expression::Multiply,this);
			Value* b=Value::operation(&v,Expression::Multiply,&v);
			Value* n=Value::operation(a,Expression::Multiply,b);
			auto* l=dynamic_cast<NumberValue*>(n);
			if(l)
				return new NumberValue(r_sqrt(l->getNumber()));
			return Value::undefined();
		} else if(e==Expression::Concatenate) {
			result=a;
			result.append(b);
		} else if(e==Expression::Equal||e==Expression::NotEqual) {
			bool eq=(a.size()==b.size());
			if(e==Expression::NotEqual && !eq)
				return new BooleanValue(true);
			if(eq)
				for(auto i=0; i<a.size(); ++i) {
					Value* eqVec=Value::operation(a.at(i),e,b.at(i));
					if(e==Expression::NotEqual && eqVec->isTrue())
						return new BooleanValue(true);
					if(eqVec->isFalse())
						eq=false;
				}
			return new BooleanValue(eq);
		} else {
			//Apply componentwise operations
			e=convertOperation(e);
			int as=a.size();
			int bs=b.size();
			for(auto i=0; i<as||i<bs; ++i) {
				Value* r;
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
		return new VectorValue(result);
	}

	auto* num = dynamic_cast<NumberValue*>(&v);
	if(num) {
		if(e==Expression::Concatenate) {
			QList<Value*> a=getChildren();
			result=a;
			result.append(num);
		} else if(e==Expression::Exponent) {
			QList<Value*> a=getChildren();
			Value* total=new NumberValue(0);
			for(Value* c: a) {
				Value* r=Value::operation(c,e,num);
				total=Value::operation(total,Expression::Add,r);
			}
			return total;
		} else if(e==Expression::Index) {
			return getIndex(num);
		} else {
			QList<Value*> a=getChildren();
			e=convertOperation(e);
			for(Value* c: a)
				result.append(Value::operation(c,e,num));
		}
		return new VectorValue(result);
	}

	return Value::operation(v,e);
}

Expression::Operator_e VectorValue::convertOperation(Expression::Operator_e e)
{
	switch(e) {
		case Expression::ComponentwiseMultiply:
			return Expression::Multiply;
		case Expression::ComponentwiseDivide:
			return Expression::Divide;
		default:
			return e;
	}
}
