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

#include "angfunction.h"
#include "complexvalue.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"
#include "vectorvalue.h"

AngFunction::AngFunction() : Function("ang")
{
	addDescription(tr("Returns a quaternion with the given angle and axis."));
	addParameter("angle","num",tr("The angle."));
	addParameter("axis","vec3",tr("The axis."));
}

Value& AngFunction::getResult(const decimal& a,const decimal& x,const decimal& y,const decimal& z)
{
	decimal w=a/2.0;
	decimal c=r_right_cos(w);
	decimal s=r_right_sin(w);

	Value& angle=ValueFactory::createNumber(c);

	QList<Value*> axis;
	axis.append(&ValueFactory::createNumber(x*s));
	axis.append(&ValueFactory::createNumber(y*s));
	axis.append(&ValueFactory::createNumber(z*s));

	return ValueFactory::createComplex(angle,axis);
}

Value& AngFunction::evaluate(const Context& ctx) const
{
	VectorValue* vecVal1=dynamic_cast<VectorValue*>(ctx.getArgument(0,"v1"));
	VectorValue* vecVal2=dynamic_cast<VectorValue*>(ctx.getArgument(1,"v2"));
	if(vecVal1&&vecVal2) {

		// a = |v1|*|v2| + v1 . v2
		Value& norm=Value::evaluate(*vecVal1,Operators::Length,*vecVal2);
		Value& dot=Value::evaluate(*vecVal1,Operators::DotProduct,*vecVal2);
		Value& angle=Value::evaluate(norm,Operators::Add,dot);

		// [x,y,z] = v1 x v2
		Value& cross=Value::evaluate(*vecVal1,Operators::CrossProduct,*vecVal2);
		auto* axis=dynamic_cast<VectorValue*>(&cross);
		if(!axis) return ValueFactory::createUndefined();

		//Renormalise the quaternion
		Value& q=ValueFactory::createComplex(angle,axis->getElements());
		Value& l=Value::evaluate(q,Operators::Length);
		return Value::evaluate(q,Operators::Divide,l);
	}

	decimal a=0.0;
	decimal x=0.0;
	decimal y=0.0;
	decimal z=1.0;
	auto* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		a=numVal->getNumber();

		auto* vecVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,1));
		if(vecVal) {
			Value& n=Value::evaluate(*vecVal,Operators::Length);
			Value& u=Value::evaluate(*vecVal,Operators::Divide,n);
			auto* unitVec=dynamic_cast<VectorValue*>(&u);
			if(unitVec) {
				Point p=unitVec->getPoint();
				x=p.x();
				y=p.y();
				z=p.z();
			}
		}
		return getResult(a,x,y,z);
	}

	NumberValue* xVal=dynamic_cast<NumberValue*>(ctx.getArgument(0,"x"));
	if(xVal) {
		a=xVal->getNumber();
		return getResult(a,1.0,0.0,0.0);
	}
	NumberValue* yVal=dynamic_cast<NumberValue*>(ctx.getArgument(0,"y"));
	if(yVal) {
		a=yVal->getNumber();
		return getResult(a,0.0,1.0,0.0);
	}
	NumberValue* zVal=dynamic_cast<NumberValue*>(ctx.getArgument(0,"z"));
	if(zVal) {
		a=zVal->getNumber();
		return getResult(a,0.0,0.0,1.0);
	}

	return ValueFactory::createUndefined();
}
