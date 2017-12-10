#include "qpathtextbuilder.h"
#include "onceonly.h"
#include <QPainterPath>
#include <QFontMetrics>
#include <QApplication>

QPathTextBuilder::QPathTextBuilder(Reporter* r) : reporter(r)
{
	size=0;
	headless = QFont().family().isEmpty();
}

QPathTextBuilder::~QPathTextBuilder()
{
}

void QPathTextBuilder::setText(QString value)
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

void QPathTextBuilder::setLocation(Point p)
{
#ifdef USE_CGAL
	location = QPointF(to_double(p.x()),to_double(p.y()));
#else
	location = QPointF(p.x(),p.y());
#endif
}

Primitive* QPathTextBuilder::buildPrimitive() const
{
	PrimitiveNode* n = buildPrimitiveNode();
	Primitive* p = n->getPrimitive();
	delete n;
	return p;
}

QFont QPathTextBuilder::getFont() const
{
	/*QFont with empty string chooses different font, so
	 * we have to call default constructor instead. */
	QFont f=family.isEmpty()?QFont():QFont(family);
	f.setPixelSize(size);
	return f;
}

PrimitiveNode* QPathTextBuilder::buildPrimitiveNode() const
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
