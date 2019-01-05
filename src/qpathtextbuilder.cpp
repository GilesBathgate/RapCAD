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

#include "qpathtextbuilder.h"
#include <QPainterPath>
#include <QFontMetrics>
#include <QApplication>
#include "onceonly.h"
#include "node/primitivenode.h"

QPathTextBuilder::QPathTextBuilder(Reporter& r) :
	size(0),
	reporter(r)
{
	headless = QFont().family().isEmpty();
}

QPathTextBuilder::~QPathTextBuilder()
{
}

void QPathTextBuilder::setText(const QString& value)
{
	text = value;
}

void QPathTextBuilder::setFamily(const QString& value)
{
	family = value;
}

void QPathTextBuilder::setSize(int value)
{
	size = value;
}

decimal QPathTextBuilder::getHeight()
{
	QFontMetrics fm(getFont());
	return fm.height();
}

void QPathTextBuilder::setLocation(const Point& p)
{
#ifdef USE_CGAL
	location = QPointF(to_double(p.x()),to_double(p.y()));
#else
	location = QPointF(p.x(),p.y());
#endif
}

QFont QPathTextBuilder::getFont() const
{
	/*QFont with empty string chooses different font, so
	 * we have to call default constructor instead. */
	QFont f=family.isEmpty()?QFont():QFont(family);
	f.setPixelSize(size);
	return f;
}

Primitive* QPathTextBuilder::buildPrimitive() const
{
	QPainterPath path;
	if(headless) {
		/* Hack: in headless mode we need to initalise QApplication
		before we can use fonts */
		int c=0;
		QApplication a(c,nullptr,false);
		a.font(); //Just to ward off warning
		path.addText(location,getFont(),text);

	} else {
		path.addText(location,getFont(),text);
	}

	QList<QPolygonF> paths = path.toSubpathPolygons();

	int index=0;
	PrimitiveNode pn(reporter);
	Primitive* p=pn.createPrimitive();
	for(const auto& path: paths) {
		Polygon* pg=p->createPolygon();
		OnceOnly first;
		for(const auto& pt: path) {
			if(!first()) {
				p->createVertex(Point(pt.x(),-pt.y(),0.0));
				pg->append(index);
				index++;
			}
		}
	}
	return p;
}
