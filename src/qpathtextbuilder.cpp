#include "qpathtextbuilder.h"
#include "onceonly.h"
#include <QPainterPath>
#include <QFontMetrics>

QPathTextBuilder::QPathTextBuilder(Reporter* r) : reporter(r)
{
	font = new QFont();
}

QPathTextBuilder::~QPathTextBuilder()
{
	delete font;
}

void QPathTextBuilder::setText(QString value)
{
	text = value;
}

decimal QPathTextBuilder::getHeight()
{
	QFontMetrics fm(*font);
	return fm.height();
}

void QPathTextBuilder::setLocation(Point value)
{
	decimal x,y;
	value.getXY(x,y);
#if USE_CGAL
	location = QPointF(to_double(x),to_double(y));
#else
	location = QPointF(x,y);
#endif
}

Primitive* QPathTextBuilder::buildPrimitive() const
{
	PrimitiveNode* p = buildPrimitiveNode();
	return p->getPrimitive();
}

PrimitiveNode* QPathTextBuilder::buildPrimitiveNode() const
{
	QPainterPath path;
	path.addText(location,*font,text);
	QList<QPolygonF> paths = path.toSubpathPolygons();

	int index=0;
	PrimitiveNode* p = new PrimitiveNode(reporter);
	for(const auto& path: paths) {
		Polygon* pg=p->createPolygon();
		OnceOnly first;
		for(const auto& pt: path) {
			if(!first()) {
				p->createVertex(pt.x(),-pt.y(),0.0);
				pg->append(index);
				index++;
			}
		}
	}
	return p;
}
