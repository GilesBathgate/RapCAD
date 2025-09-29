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

#include "primitivemodule.h"

#include "rmath.h"

PrimitiveModule::PrimitiveModule(Reporter& r, const QString& n) : Module(r,n)
{
}

QList<Point> PrimitiveModule::getCircle(const decimal& r,int f,const decimal& z)
{
	return getSpiral(r,f,f,z,z);
}

QList<Point> PrimitiveModule::getSpiral(const decimal& r,int f,int n,const decimal& z1,const decimal& z2)
{
	QList<Point> circle;
	if(f<=0||n<=0||r<0.0) return circle;

	const auto dz=(z2-z1)/n;
	const auto dgz=dz>0.0;

	for(auto i=0; i<n; ++i) {
		const decimal& phi=(r_tau()*i)/f;
		const decimal x=r*r_cos(phi);
		const decimal y=r*r_sin(phi);
		const decimal z=dgz?z1+(i*dz):z1;
		const Point p(x,y,z);
		circle.append(p);
	}

	return circle;
}

QList<Point> PrimitiveModule::getPolygon(const decimal& a,const decimal& r,int n,const decimal& z)
{
	QList<Point> poly;
	if(n==6) {
		//TODO modify this to cater for all even values of n
		decimal x=0.0;
		decimal y=0.0;
		const decimal& s2=r*r_sin(r_pi()/n);
		for(auto i=0; i<n; ++i) {
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
					y=0.0;
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
					y=0.0;
					x=-r;
					break;
				}
			}
			poly.append(Point(x,y,z));
		}
		return poly;
	}
	return getCircle(r,n,z);
}

void PrimitiveModule::createTriangle(Primitive* p,int a,int b,int c)
{
	Polygon& pg=p->createPolygon();
	pg.append(a);
	pg.append(b);
	pg.append(c);
}

void PrimitiveModule::createQuad(Primitive* p,int a,int b,int c,int d)
{
	Polygon& pg=p->createPolygon();
	pg.append(a);
	pg.append(b);
	pg.append(c);
	pg.append(d);
}
