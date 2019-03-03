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

#include "angfunction.h"
#include "context.h"
#include "numbervalue.h"
#include "vectorvalue.h"
#include "complexvalue.h"
#include "rmath.h"

AngFunction::AngFunction() : Function("ang")
{
	addDescription(tr("Returns a quaternion with the given angle and axis."));
	addParameter("angle");
	addParameter("axis");
}

Value* AngFunction::getResult(const decimal& a,const decimal& x,const decimal& y,const decimal& z) const
{
	decimal w=a/2.0;
	decimal c=r_right_cos(w);
	decimal s=r_right_sin(w);

	Value* angle=new NumberValue(c);

	QList<Value*> axis;
	axis.append(new NumberValue(x*s));
	axis.append(new NumberValue(y*s));
	axis.append(new NumberValue(z*s));

	return new ComplexValue(angle,axis);
}

Value* AngFunction::evaluate(const Context& ctx) const
{
	VectorValue* vecVal1=dynamic_cast<VectorValue*>(ctx.getArgument(0,"v1"));
	VectorValue* vecVal2=dynamic_cast<VectorValue*>(ctx.getArgument(1,"v2"));
	if(vecVal1&&vecVal2) {

		// a = |v1|*|v2| + v1 . v2
		Value* norm=Value::operation(vecVal1,Expression::Length,vecVal2);
		Value* dot=Value::operation(vecVal1,Expression::DotProduct,vecVal2);
		Value* angle=Value::operation(norm,Expression::Add,dot);

		// [x,y,z] = v1 x v2
		Value* cross=Value::operation(vecVal1,Expression::CrossProduct,vecVal2);
		auto* axis=dynamic_cast<VectorValue*>(cross);
		if(!axis) return Value::undefined();

		//Renormalise the quaternion
		Value* q=new ComplexValue(angle,axis->getChildren());
		Value* l=Value::operation(q,Expression::Length);
		return Value::operation(q,Expression::Divide,l);
	}

	decimal a=0.0;
	decimal x=0.0,y=0.0,z=1.0;
	auto* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		a=numVal->getNumber();

		auto* vecVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,1));
		if(vecVal) {
			Value* n=Value::operation(vecVal,Expression::Length);
			Value* u=Value::operation(vecVal,Expression::Divide,n);
			auto* unitVec=dynamic_cast<VectorValue*>(u);
			if(unitVec) {
				Point p=unitVec->getPoint();
				x=p.x(); y=p.y(); z=p.z();
			}
		}
		return getResult(a,x,y,z);
	} else {
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
	}

	return Value::undefined();
}
