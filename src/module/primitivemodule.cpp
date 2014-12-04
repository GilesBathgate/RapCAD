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

#include <math.h>
#include "primitivemodule.h"
#include "tau.h"
#include "context.h"
#include "numbervalue.h"

PrimitiveModule::PrimitiveModule(const QString n) : Module(n)
{
}

Fragment PrimitiveModule::getSpecialVariables(Context* ctx)
{
	decimal fn=0.0;
	decimal fs=2.0;
	decimal fa=12.0;
	NumberValue* fnVal=dynamic_cast<NumberValue*>(ctx->getArgumentSpecial("fn"));
	if(fnVal)
		fn=fnVal->getNumber();
	NumberValue* fsVal=dynamic_cast<NumberValue*>(ctx->getArgumentSpecial("fs"));
	if(fsVal)
		fs=fsVal->getNumber();
	NumberValue* faVal=dynamic_cast<NumberValue*>(ctx->getArgumentSpecial("fa"));
	if(faVal)
		fa=faVal->getNumber();

	return Fragment(fn,fs,fa);
}

QList<Point> PrimitiveModule::getCircle(decimal r, decimal f, decimal z)
{
	QList<Point> circle;
	for(int i=0; i<f; i++) {
		decimal phi = (M_TAU*i) / f;
		decimal x,y;
		if(r > 0) {
			x = r*cos(phi);
			y = r*sin(phi);
		} else {
			x=0;
			y=0;
		}
		Point p(x,y,z);
		circle.append(p);
	}

	return circle;
}

QList<Point> PrimitiveModule::getPolygon(decimal a,decimal r, decimal n, decimal z)
{
	QList<Point> poly;
	if(n==6) {
		//TODO modify this to cater for all even values of n
		decimal x=0,y=0;
		decimal s2=r*sin(M_PI/n);
		for(int i=0; i<n; i++) {
			switch(i) {
			case 0: {
				y=a;
				x=-s2;
				break;
			}
			case 1: {
				x=s2;
				break;
			}
			case 2: {
				y=0;
				x=r;
				break;
			}
			case 3: {
				y=-a;
				x=s2;
				break;
			}
			case 4: {
				x=-s2;
				break;
			}
			case 5: {
				y=0;
				x=-r;
				break;
			}
			}
			poly.append(Point(x,y,z));
		}
		return poly;
	} else {
		return getCircle(r,n,z);
	}
}
