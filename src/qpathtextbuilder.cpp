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

#include "qpathtextbuilder.h"
#include "node/primitivenode.h"
#include "onceonly.h"
#include <QApplication>
#include <QFontMetrics>
#include <QPainterPath>
#include <QMutexLocker>

QPathTextBuilder::QPathTextBuilder() :
	size(0)
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

/* Hack: in headless mode we need to initalise QApplication
 * before we can use fonts */
static QCoreApplication* headlessOverride()
{
	int c=0;
	static auto* instance {
		QApplication::instance() ?: new QApplication(c,nullptr)
	};
	return instance;
}

Primitive* QPathTextBuilder::buildPrimitive() const
{
	headlessOverride();

	QPainterPath painterPath;
	painterPath.addText(location,getFont(),text);

	const QList<QPolygonF> paths = painterPath.toSubpathPolygons();

	int index=0;
	PrimitiveNode pn;
	Primitive* p=pn.createPrimitive();
	for(const auto& path: paths) {
		Polygon& pg=p->createPolygon();
		OnceOnly first;
		for(const auto& pt: path) {
			if(!first()) {
				p->createVertex(Point(pt.x(),-pt.y(),0.0));
				pg.append(index);
				index++;
			}
		}
	}
	return p;
}
