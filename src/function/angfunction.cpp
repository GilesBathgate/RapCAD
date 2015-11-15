/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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
	addParameter("angle");
	addParameter("axis");
}

Value* AngFunction::getResult(decimal a,decimal x,decimal y,decimal z)
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

Value* AngFunction::evaluate(Context* ctx)
{

	decimal a=0.0;
	decimal x=0.0,y=0.0,z=0.0;
	NumberValue* numVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(numVal) {
		a=numVal->getNumber();

		VectorValue* vecVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,1));
		if(vecVal) {
			Value* n=Value::operation(vecVal,Expression::Length);
			Value* u=Value::operation(vecVal,Expression::Divide,n);
			VectorValue* unitVec=dynamic_cast<VectorValue*>(u);
			if(unitVec) {
				Point p=unitVec->getPoint();
				p.getXYZ(x,y,z);
			}
		} else {
			// Assume rotation around z axis when no axis given
			return getResult(a,0.0,0.0,1.0);
		}
	} else {
		NumberValue* xVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"x"));
		if(xVal) {
			a=xVal->getNumber();
			return getResult(a,1.0,0.0,0.0);
		}
		NumberValue* yVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"y"));
		if(yVal) {
			a=yVal->getNumber();
			return getResult(a,0.0,1.0,0.0);
		}
		NumberValue* zVal=dynamic_cast<NumberValue*>(ctx->getArgument(0,"z"));
		if(zVal) {
			a=zVal->getNumber();
			return getResult(a,0.0,0.0,1.0);
		}
	}

	return getResult(a,x,y,z);
}