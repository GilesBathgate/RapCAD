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

#include "simpletextbuilder.h"
#include "polyhedron.h"

QMap<QChar,SimpleTextBuilder::Letter>* SimpleTextBuilder::characters=nullptr;

SimpleTextBuilder::SimpleTextBuilder() : location(0,0,0)
{
	if(!characters) {
		characters=new QMap<QChar,Letter>();

		Stroke stroke;

		Letter zero;
		stroke=Stroke();
		stroke.append(Point(0.25,1.6,0.0));
		stroke.append(Point(0.75,1.6,0.0));
		stroke.append(Point(1.0,1.4,0.0));
		stroke.append(Point(1.0,0.2,0.0));
		stroke.append(Point(0.75,0.0,0.0));
		stroke.append(Point(0.25,0.0,0.0));
		stroke.append(Point(0.0,0.2,0.0));
		stroke.append(Point(0.0,1.4,0.0));
		stroke.append(Point(0.25,1.6,0.0));
		zero.append(stroke);
		stroke=Stroke();
		stroke.append(Point(1.0,1.4,0.0));
		stroke.append(Point(0.0,0.2,0.0));
		zero.append(stroke);
		characters->insert('0',zero);

		Letter one;
		stroke=Stroke();
		stroke.append(Point(0.25,1.2,0.0));
		stroke.append(Point(0.5,1.6,0.0));
		stroke.append(Point(0.5,0.0,0.0));
		one.append(stroke);
		characters->insert('1',one);

		Letter two;
		stroke=Stroke();
		stroke.append(Point(0.0,1.4,0.0));
		stroke.append(Point(0.25,1.6,0.0));
		stroke.append(Point(0.75,1.6,0.0));
		stroke.append(Point(1.0, 1.4,0.0));
		stroke.append(Point(1.0, 1.0,0.0));
		stroke.append(Point(0.0, 0.0,0.0));
		stroke.append(Point(1.0, 0.0,0.0));
		two.append(stroke);
		characters->insert('2',two);

		Letter three;
		stroke=Stroke();
		stroke.append(Point(0.0, 1.4,0.0));
		stroke.append(Point(0.25,1.6,0.0));
		stroke.append(Point(0.75,1.6,0.0));
		stroke.append(Point(1.0, 1.4,0.0));
		stroke.append(Point(1.0, 1.0,0.0));
		stroke.append(Point(0.75,0.8,0.0));
		stroke.append(Point(1.0, 0.6,0.0));
		stroke.append(Point(1.0, 0.2,0.0));
		stroke.append(Point(0.75,0.0,0.0));
		stroke.append(Point(0.25,0.0,0.0));
		stroke.append(Point(0.0, 0.2,0.0));
		three.append(stroke);
		stroke=Stroke();
		stroke.append(Point(0.25,0.8,0.0));
		stroke.append(Point(0.75,0.8,0.0));
		three.append(stroke);
		characters->insert('3',three);

		Letter four;
		stroke=Stroke();
		stroke.append(Point(0.75,0.0,0.0));
		stroke.append(Point(0.75,1.6,0.0));
		stroke.append(Point(0.0,0.6,0.0));
		stroke.append(Point(1.0,0.6,0.0));
		four.append(stroke);
		characters->insert('4',four);

		Letter five;
		stroke=Stroke();
		stroke.append(Point(1.0, 1.6,0.0));
		stroke.append(Point(0.0, 1.6,0.0));
		stroke.append(Point(0.0, 0.8,0.0));
		stroke.append(Point(0.75,0.8,0.0));
		stroke.append(Point(1.0, 0.6,0.0));
		stroke.append(Point(1.0, 0.2,0.0));
		stroke.append(Point(0.75,0.0,0.0));
		stroke.append(Point(0.25,0.0,0.0));
		stroke.append(Point(0.0,0.2,0.0));
		five.append(stroke);
		characters->insert('5',five);

		Letter six;
		stroke=Stroke();
		stroke.append(Point(1.0, 1.4,0.0));
		stroke.append(Point(0.75,1.6,0.0));
		stroke.append(Point(0.25,1.6,0.0));
		stroke.append(Point(0.0, 1.4,0.0));
		stroke.append(Point(0.0, 0.2,0.0));
		stroke.append(Point(0.25,0.0,0.0));
		stroke.append(Point(0.75,0.0,0.0));
		stroke.append(Point(1.0, 0.2,0.0));
		stroke.append(Point(1.0, 0.6,0.0));
		stroke.append(Point(0.75,0.8,0.0));
		stroke.append(Point(0.0, 0.8,0.0));
		six.append(stroke);
		characters->insert('6',six);

		Letter seven;
		stroke=Stroke();
		stroke.append(Point(0.0, 1.6,0.0));
		stroke.append(Point(0.75,1.6,0.0));
		stroke.append(Point(1.0, 1.4,0.0));
		stroke.append(Point(0.0, 0.0,0.0));
		seven.append(stroke);
		characters->insert('7',seven);

		Letter eight;
		stroke=Stroke();
		stroke.append(Point(0.25,0.8,0.0));
		stroke.append(Point(0.0, 1.0,0.0));
		stroke.append(Point(0.0, 1.4,0.0));
		stroke.append(Point(0.25,1.6,0.0));
		stroke.append(Point(0.75,1.6,0.0));
		stroke.append(Point(1.0, 1.4,0.0));
		stroke.append(Point(1.0, 1.0,0.0));
		stroke.append(Point(0.75,0.8,0.0));
		stroke.append(Point(0.25,0.8,0.0));
		stroke.append(Point(0.0, 0.6,0.0));
		stroke.append(Point(0.0, 0.2,0.0));
		stroke.append(Point(0.25,0.0,0.0));
		stroke.append(Point(0.75,0.0,0.0));
		stroke.append(Point(1.0, 0.2,0.0));
		stroke.append(Point(1.0, 0.6,0.0));
		stroke.append(Point(0.75,0.8,0.0));
		eight.append(stroke);
		characters->insert('8',eight);

		Letter nine;
		stroke=Stroke();
		stroke.append(Point(1.0, 1.0,0.0));
		stroke.append(Point(0.75,0.8,0.0));
		stroke.append(Point(0.25,0.8,0.0));
		stroke.append(Point(0.0, 1.0,0.0));
		stroke.append(Point(0.0, 1.4,0.0));
		stroke.append(Point(0.25,1.6,0.0));
		stroke.append(Point(0.75,1.6,0.0));
		stroke.append(Point(1.0, 1.4,0.0));
		stroke.append(Point(1.0, 0.0,0.0));
		nine.append(stroke);
		characters->insert('9',nine);

		Letter point;
		stroke=Stroke();
		stroke.append(Point(0.0, 0.0,0.0));
		stroke.append(Point(0.0, 0.2,0.0));
		stroke.append(Point(0.25,0.2,0.0));
		stroke.append(Point(0.25,0.0,0.0));
		stroke.append(Point(0.0, 0.0,0.0));
		point.append(stroke);
		characters->insert('.',point);

		Letter minus;
		stroke=Stroke();
		stroke.append(Point(0.0,0.8,0.0));
		stroke.append(Point(1.0,0.8,0.0));
		minus.append(stroke);
		characters->insert('-',minus);
	}
}

void SimpleTextBuilder::setText(const QString& t)
{
	text=t;
}

decimal SimpleTextBuilder::getHeight()
{
	return 1.6;
}

void SimpleTextBuilder::setLocation(const Point& p)
{
	location=p;
}

Primitive* SimpleTextBuilder::buildPrimitive() const
{
	auto* ph=new Polyhedron();
	ph->setType(Primitive::Lines);

	int n=0;
	decimal x=location.x(),y=location.y(),z=location.z();
	for(QChar c: text) {
		Letter ch=characters->value(c);
		for(Stroke p: ch) {
			Polygon* pg=ph->createPolygon();
			for(const auto& pt: p) {
				decimal cx=pt.x(),cy=pt.y(),cz=pt.z();
				ph->createVertex(Point(cx+x,cy+y,cz+z));
				pg->append(n++);
			}
		}
		if(c=='.')
			x+=0.75;
		else
			x+=1.5;
	}

	return ph;
}
