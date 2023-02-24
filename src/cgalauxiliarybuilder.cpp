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
#ifdef USE_CGAL
#include "cgalauxiliarybuilder.h"

#include "cgalexplorer.h"
#include "cgalprimitive.h"
#include "module/cubemodule.h"
#include "polyhedron.h"
#include "rmath.h"
#include "simpletextbuilder.h"
#include <CGAL/centroid.h>


CGALAuxiliaryBuilder::CGALAuxiliaryBuilder(Reporter& r) :
	reporter(r)
{
}

Primitive* CGALAuxiliaryBuilder::buildBoundsPrimitive(Primitive* pr)
{
	auto* cp=dynamic_cast<CGALPrimitive*>(pr);
	if(!cp) return pr;
	const CGAL::Cuboid3& b=cp->getBounds();

	const CGAL::Scalar& xmin=b.xmin();
	const CGAL::Scalar& ymin=b.ymin();
	const CGAL::Scalar& xmax=b.xmax();
	const CGAL::Scalar& ymax=b.ymax();
	const CGAL::Scalar& zmin=b.zmin();
	const CGAL::Scalar& zmax=b.zmax();

	if(zmin!=0.0) {
		const QString& pos=to_string(zmin);
		const QString& where = zmin<0.0?tr("below"):tr("above");
		reporter.reportWarning(tr("the model is %1 %2 the build platform.").arg(pos,where));

		SimpleTextBuilder t;
		t.setText(pos);
		const CGAL::Scalar& h=t.getHeight()+0.2;
		t.setLocation(CGAL::Point3(xmin,ymin-h,zmin));
		Primitive* c=t.buildPrimitive();
		cp->appendChild(c);
	}

	Primitive* a=new Polyhedron();
	a->setType(PrimitiveTypes::Lines);
	CubeModule::createCuboid<CGAL::Point3>(a,xmin,xmax,ymin,ymax,zmin,zmax);

	cp->appendChild(a);

	const CGAL::Point3 lower(xmin,ymin,zmin);
	const CGAL::Point3 upper(xmax,ymax,zmax);
	const CGAL::Point3 size(xmax-xmin,ymax-ymin,zmax-zmin);
	reporter.reportMessage(tr("Bounds: [%1,%2]\nSize: %3").arg(to_string(lower),to_string(upper),to_string(size)));

	return cp;
}

Primitive* CGALAuxiliaryBuilder::buildNormalsPrimitive(Primitive* pr)
{
	CGALExplorer e(pr);
	CGALPrimitive* prim=e.getPrimitive();
	if(!prim) return pr;
	auto* a=new Polyhedron();
	a->setType(PrimitiveTypes::Lines);
	int i=0;
	for(CGALPolygon* pg: prim->getCGALPolygons()) {
		const CGAL::Vector3& v=pg->getNormal();
		const CGAL::Plane3& p=pg->getPlane();
		CGAL::Vector3 b=p.base1();
		QList<CGAL::Point3> pts=pg->getPoints();
		CGAL::Point3 c=CGAL::centroid(pts.begin(),pts.end());

		CGAL::Scalar l=r_sqrt(v.squared_length());
		const CGAL::Point3& norm=c+(v/l);
		l=r_sqrt(b.squared_length());
		b=b/(l*8.0);

		Polygon& np=a->createPolygon();
		a->createVertex(c+b);
		a->createVertex(norm);
		a->createVertex(c-b);
		np.append(i++);
		np.append(i++);
		np.append(i++);
	}
	delete prim;

	pr->appendChild(a);

	return pr;
}

Primitive* CGALAuxiliaryBuilder::buildDatumsPrimitive(Primitive* pr,Axis axis)
{
	auto* cp=dynamic_cast<CGALPrimitive*>(pr);
	if(!cp) return pr;
	const CGAL::Cuboid3& b=cp->getBounds();
	const CGAL::Point3 size(b.xmax()-b.xmin(),b.ymax()-b.ymin(),b.zmax()-b.zmin());
	const CGAL::Scalar& l=r_max(size.x(),r_max(size.y(),size.z()))*2.0;

	Primitive* a=new Polyhedron();
	a->setType(PrimitiveTypes::Surface);
	switch(axis) {
		case Axis::X: {
			a->createVertex(Point(0,-l,-l));
			a->createVertex(Point(0,-l,+l));
			a->createVertex(Point(0,+l,+l));
			a->createVertex(Point(0,+l,-l));
		}
		break;
		case Axis::Y: {
			a->createVertex(Point(-l,0,-l));
			a->createVertex(Point(-l,0,+l));
			a->createVertex(Point(+l,0,+l));
			a->createVertex(Point(+l,0,-l));
		}
		break;
		case Axis::Z: {
			a->createVertex(Point(-l,-l,0));
			a->createVertex(Point(-l,+l,0));
			a->createVertex(Point(+l,+l,0));
			a->createVertex(Point(+l,-l,0));
		}
		break;
		default:
			break;
	}
	Polygon& p=a->createPolygon();
	p.append(0);
	p.append(1);
	p.append(2);
	p.append(3);
	pr->appendChild(a);

	return pr;
}

Primitive* CGALAuxiliaryBuilder::buildRadialsPrimitive(Primitive* pr)
{
	auto* cp=dynamic_cast<CGALPrimitive*>(pr);
	if(!cp) return pr;
	const CGAL::Circle3& circle=cp->getRadius();
	const CGAL::Scalar& r=r_sqrt(circle.squared_radius());
	QString rs=to_string(r);
	reporter.reportMessage(tr("Radius: %1").arg(rs));

	const CGAL::Point3& c=circle.center();
	const CGAL::Scalar& a=c.x();
	const CGAL::Scalar& b=c.y();

	SimpleTextBuilder t;
	t.setText(rs);
	t.setLocation(CGAL::Point3(a,b,0));
	Primitive* tp=t.buildPrimitive();
	pr->appendChild(tp);

	auto* p = new Polyhedron();
	p->setType(PrimitiveTypes::Lines);
	Polygon& pg=p->createPolygon();

	const int f=90;
	for(auto i=0; i<=f; ++i) {
		const CGAL::Scalar& phi = (r_tau()*i) / f;
		const CGAL::Scalar& x = a+r*r_cos(phi);
		const CGAL::Scalar& y = b+r*r_sin(phi);

		p->createVertex(CGAL::Point3(x,y,0));
		pg.append(i);
	}

	pr->appendChild(p);
	return pr;
}

Primitive* CGALAuxiliaryBuilder::buildVolumesPrimitive(Primitive* pr,bool calcMass)
{
	auto* cp=dynamic_cast<CGALPrimitive*>(pr);
	if(!cp) return pr;
	const CGALVolume& v=cp->getVolume(calcMass);
	const QString vs=v.getSizeString();
	reporter.reportMessage(tr("Volume: %1").arg(vs));

	if(calcMass)
		reporter.reportMessage(tr("Center of Mass: %1").arg(v.getCenterString()));

	const CGAL::Cuboid3& b=v.getBounds();
	const CGAL::Scalar& x=b.xmax()+((b.xmax()-b.xmin())/10.0);
	const CGAL::Scalar& y=b.ymax()+((b.ymax()-b.ymin())/10.0);
	const CGAL::Scalar& z=b.zmax()+((b.zmax()-b.zmin())/10.0);
	const CGAL::Point3 tr(x,y,z);

	const CGAL::Point3& c=v.getCenter();
	auto* p = new Polyhedron();
	p->setType(PrimitiveTypes::Lines);
	Polygon& pg=p->createPolygon();
	p->createVertex(c);
	p->createVertex(tr);
	pg.append(0);
	pg.append(1);
	pr->appendChild(p);

	SimpleTextBuilder t;
	t.setText(vs);
	t.setLocation(tr);
	Primitive* tp=t.buildPrimitive();
	pr->appendChild(tp);
	return pr;
}

#endif
