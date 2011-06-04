/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

VectorValue::VectorValue(QList<Value*> values)
{
	this->children=values;
}

QString VectorValue::getValueString() const
{
	QString result;
	result.append("[");
	for(int i=0; i<children.size(); i++) {
		if(i>0)
			result.append(",");
		result.append(children.at(i)->getValueString());
	}
	result.append("]");
	return result;
}

bool VectorValue::isTrue() const
{
	return this->children.size()>0;
}

VectorValue* VectorValue::toVector(int)
{
	return this;
}

Point VectorValue::getPoint() const
{

	NumberValue* nx=NULL,*ny=NULL,*nz=NULL;
	int s=children.size();
	if(s>0)
		nx=dynamic_cast<NumberValue*>(children.at(0));
	if(s>1)
		ny=dynamic_cast<NumberValue*>(children.at(1));
	if(s>2)
		nz=dynamic_cast<NumberValue*>(children.at(2));

	double x=0,y=0,z=0;
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

Iterator<Value*>* VectorValue::createIterator()
{
	return new VectorIterator(this->children);
}

QList<Value*> VectorValue::getChildren() const
{
	return this->children;
}

Value* VectorValue::operation(Expression::Operator_e e)
{
	QList<Value*> result;
	for(int i=0; i<this->children.size(); i++)
		result.append(Value::operation(this->children.at(i),e));
	return new VectorValue(result);
}

Value* VectorValue::operation(Value& v, Expression::Operator_e e)
{
	QList<Value*> result;
	VectorValue* that = dynamic_cast<VectorValue*>(&v);
	if(that) {
		if(e==Expression::OuterProduct) {
			for(int i=0; i<that->children.size(); i++)
				result.append(Value::operation(this,e,that->children.at(i)));
		} else if(e==Expression::Multiply || e==Expression::Divide) {
			//TODO implement multiply and divide
			return this;
		} else {
			e=convertOperation(e);
			for(int i=0; i<children.size() && i<that->children.size(); i++)
				result.append(Value::operation(this->children.at(i),e,that->children.at(i)));
		}
	}

	NumberValue* num = dynamic_cast<NumberValue*>(&v);
	if(num) {
		e=convertOperation(e);
		for(int i=0; i<children.size(); i++)
			result.append(Value::operation(this->children.at(i),e,num));
	}
	return new VectorValue(result);
}

Expression::Operator_e VectorValue::convertOperation(Expression::Operator_e e)
{
	switch(e) {
	case Expression::ComponentwiseMultiply:
		return Expression::Multiply;
	case Expression::ComponentwiseDivide:
		return Expression::Divide;
	case Expression::OuterProduct:
		return Expression::Multiply;
	default:
		return e;
	}
}
