#if USE_CGAL
#include "cgalprimitive.h"
#include <QPair>
#include <CGAL/minkowski_sum_3.h>
#include <CGAL/convex_decomposition_3.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/bounding_box.h>

//Mesh simplification
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>

#include "cgalbuilder.h"
#include "cgalexplorer.h"
#include "onceonly.h"
#include "rmath.h"

CGAL::NefPolyhedron3* CGALPrimitive::singlePoint=NULL;

void CGALPrimitive::init()
{
	nUnion=NULL;
	type=Primitive::Volume;
}

CGALPrimitive::CGALPrimitive()
{
	init();
	nefPolyhedron=NULL;
}

CGALPrimitive::~CGALPrimitive()
{
	delete nefPolyhedron;

	qDeleteAll(polygons);
	polygons.clear();

	qDeleteAll(children);
	children.clear();
}

CGALPrimitive::CGALPrimitive(CGAL::Polyhedron3 poly)
{
	init();
	nefPolyhedron=new CGAL::NefPolyhedron3(poly);
}

void CGALPrimitive::setType(Primitive_t t)
{
	type=t;
}

typedef CGAL::Polyhedron3::Halfedge_handle HalfedgeHandle;

static CGAL::Scalar getLength(HalfedgeHandle h)
{
	return CGAL::squared_distance(h->vertex()->point(),h->opposite()->vertex()->point());
}

static void removeShortEdge(CGAL::Polyhedron3& p,HalfedgeHandle h)
{
	p.join_facet(h->next());
	p.join_facet(h->opposite()->next());
	p.join_vertex(h);
}

static void removeShortestEdges(CGAL::Polyhedron3& p,HalfedgeHandle h1,HalfedgeHandle h2,HalfedgeHandle h3)
{
	CGAL::Scalar l1=getLength(h1);
	CGAL::Scalar l2=getLength(h2);
	CGAL::Scalar l3=getLength(h3);

	if(l1<l2||l1<l3)
		p.join_facet(h1);
	if(l2<l1||l2<l3)
		p.join_facet(h2);
	if(l3<l1||l3<l2)
		p.join_facet(h3);
}

static void fixZeroTriangles(CGAL::Polyhedron3& p)
{
	typedef CGAL::Polyhedron3::Facet_iterator FacetIterator;
	for(FacetIterator f=p.facets_begin(); f!=p.facets_end(); f++) {
		HalfedgeHandle h1=f->halfedge();
		HalfedgeHandle h2=h1->next();
		HalfedgeHandle h3=h2->next();
		const CGAL::Point3& p1=h1->vertex()->point();
		const CGAL::Point3& p2=h2->vertex()->point();
		const CGAL::Point3& p3=h3->vertex()->point();
		if(CGAL::collinear(p1,p2,p3)) {
			removeShortestEdges(p,h1,h2,h3);
		}
	}
}

static void fixZeroEdges(CGAL::Polyhedron3& p)
{
	bool removed;
	do {
		removed=false;
		typedef CGAL::Polyhedron3::Halfedge_iterator HalfedgeIterator;
		for(HalfedgeIterator h=p.halfedges_begin(); h!=p.halfedges_end(); h++) {
			if(getLength(h)==0.0) {
				removeShortEdge(p,h);
				removed=true;
				break;
			}
		}

	} while(removed);
}

void CGALPrimitive::buildPrimitive()
{
	if(nefPolyhedron)
		return;

	switch(type) {
	case Primitive::Volume: {
		CGALBuilder b(this);
		CGAL::Polyhedron3 poly;
		poly.delegate(b);
		fixZeroEdges(poly);
		fixZeroTriangles(poly);
		nefPolyhedron=new CGAL::NefPolyhedron3(poly);
		return;
	}

	case Primitive::Skeleton: {
		OnceOnly first;
		foreach(CGALPolygon* p,polygons) {
			QVector<CGAL::Point3> pl;
			foreach(CGAL::Point3 pt, p->getPoints()) {
				pl.append(pt);
			}

			if(first()) {
				nefPolyhedron=createPolyline(pl);
			} else {
				const CGAL::NefPolyhedron3* np=createPolyline(pl);
				*nefPolyhedron=nefPolyhedron->join(*np);
			}
		}
		return;
	}

	default: {
		if(!singlePoint) {
			QVector<CGAL::Point3> pl1,pl2;
			CGAL::Point3 p1=CGAL::Point3(0.0,0.0,0.0);
			CGAL::Point3 p2=CGAL::Point3(1.0,0.0,0.0);
			CGAL::Point3 p3=CGAL::Point3(0.0,1.0,0.0);

			pl1.append(p1);
			pl1.append(p2);
			singlePoint=createPolyline(pl1);

			pl2.append(p1);
			pl2.append(p3);
			const CGAL::NefPolyhedron3* np=createPolyline(pl2);

			*singlePoint=singlePoint->intersection(*np);
		}

		CGAL::Point3 p=points.last();

		nefPolyhedron=new CGAL::NefPolyhedron3(*singlePoint);

		CGAL::AffTransformation3 t(
			1, 0, 0, p.x(),
			0, 1, 0, p.y(),
			0, 0, 1, p.z(), 1);
		nefPolyhedron->transform(t);
		return;
	}
	}
	nefPolyhedron=new CGAL::NefPolyhedron3();
}

CGAL::NefPolyhedron3* CGALPrimitive::createPolyline(QVector<CGAL::Point3> pl)
{
	typedef QPair<CGAL::Point3*,CGAL::Point3*>  PointRange;
	typedef QList<PointRange> PolyLine;

	PointRange p(pl.begin(),pl.end());
	PolyLine poly;
	poly.push_back(p);
	return new CGAL::NefPolyhedron3(poly.begin(), poly.end(), CGAL::NefPolyhedron3::Polylines_tag());
}

Polygon* CGALPrimitive::createPolygon()
{
	CGALPolygon* pg = new CGALPolygon(this);
	polygons.append(pg);
	return pg;
}

void CGALPrimitive::createVertex(Point pt)
{
	decimal x,y,z;
	pt.getXYZ(x,y,z);
	CGAL::Point3 p(x,y,z);
	points.append(p);
}

void CGALPrimitive::appendVertex(CGAL::Point3 p)
{
	if(!polygons.isEmpty()) {
		if(!points.contains(p))
			points.append(p);

		int i=points.indexOf(p);
		polygons.last()->append(i);
	}
}

void CGALPrimitive::prependVertex(CGAL::Point3 p)
{
	if(!polygons.isEmpty()) {
		if(!points.contains(p))
			points.append(p);

		int i=points.indexOf(p);
		polygons.last()->prepend(i);
	}
}

bool CGALPrimitive::overlaps(Primitive* pr)
{
	CGALPrimitive* that=static_cast<CGALPrimitive*>(pr);
	return CGAL::do_intersect(this->getBounds(),that->getBounds());
}


Primitive* CGALPrimitive::group(Primitive* pr)
{
	QList<Primitive*> primitives;
	primitives.append(this);
	primitives.append(pr);

	CGALPrimitive* cp=new CGALPrimitive();
	foreach(Primitive* pr, primitives) {
		CGALPrimitive* prim=static_cast<CGALPrimitive*>(pr);
		if(prim->nefPolyhedron) {
			CGALExplorer e(prim);
			prim=e.getPrimitive();
		}
		foreach(CGALPolygon* p, prim->getCGALPolygons()) {
			cp->createPolygon();
			foreach(CGAL::Point3 pt, p->getPoints()) {
				cp->appendVertex(pt);
			}
		}
	}
	return cp;
}

QList<CGALPolygon*> CGALPrimitive::getCGALPolygons() const
{
	return polygons;
}

QList<Point> CGALPrimitive::getPoints() const
{
	return QList<Point>();
}

QList<CGAL::Point3> CGALPrimitive::getCGALPoints() const
{
	return points;
}

CGAL::Cuboid3 CGALPrimitive::getBounds()
{
	if(nefPolyhedron) {
		CGALExplorer e(this);
		return e.getBounds();
	}

	return CGAL::bounding_box(points.begin(),points.end());
}

void CGALPrimitive::add(Primitive* pr,bool force)
{
	if(!nUnion) {
		nUnion=new CGAL::Nef_nary_union_3<Unionable>();
		nUnion->add_polyhedron(Unionable(this,force));
	}
	nUnion->add_polyhedron(Unionable(pr,force));
}

Primitive* CGALPrimitive::combine()
{
	if(nUnion) {
		Unionable un=nUnion->get_union();
		delete nUnion;
		return un.primitive;
	}
	return this;
}

Primitive* CGALPrimitive::join(Primitive* pr)
{
	CGALPrimitive* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->join(*that->nefPolyhedron);
	this->appendChild(that);
	return this;
}

Primitive* CGALPrimitive::intersection(Primitive* pr)
{
	CGALPrimitive* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->intersection(*that->nefPolyhedron);
	this->appendChild(that);
	return this;
}

Primitive* CGALPrimitive::difference(Primitive* pr)
{
	CGALPrimitive* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->difference(*that->nefPolyhedron);
	this->appendChild(that);
	return this;
}

Primitive* CGALPrimitive::symmetric_difference(Primitive* pr)
{
	CGALPrimitive* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->symmetric_difference(*that->nefPolyhedron);
	this->appendChild(that);
	return this;
}

Primitive* CGALPrimitive::minkowski(Primitive* pr)
{
	CGALPrimitive* that=dynamic_cast<CGALPrimitive*>(pr);
	if(!that) {
		pr->appendChild(this);
		return pr;
	}
	this->buildPrimitive();
	that->buildPrimitive();
	*nefPolyhedron=CGAL::minkowski_sum_3(*nefPolyhedron,*that->nefPolyhedron);
	this->appendChild(that);
	return this;
}

Primitive* CGALPrimitive::inset(const decimal amount)
{
	CGALBuilder b(this);
	CGALPrimitive* result=b.buildOffsetPolygons(amount);
	return result;
}

Primitive* CGALPrimitive::decompose()
{
	this->buildPrimitive();
	CGAL::convex_decomposition_3(*nefPolyhedron);

	typedef CGAL::NefPolyhedron3::Volume_const_iterator VolumeIterator;
	// the first volume is the outer volume, which is
	// ignored in the decomposition
	VolumeIterator ci;
	for(ci = ++nefPolyhedron->volumes_begin(); ci!=nefPolyhedron->volumes_end(); ++ci) {
		if(ci->mark()) {
			CGAL::Polyhedron3 p;
			nefPolyhedron->convert_inner_shell_to_polyhedron(ci->shells_begin(), p);
			children.append(new CGALPrimitive(p));
		}
	}
	return this;
}

Primitive* CGALPrimitive::complement()
{
	this->buildPrimitive();
	*nefPolyhedron=nefPolyhedron->complement();
	return this;
}

Primitive* CGALPrimitive::triangulate()
{
	CGALBuilder b(this);
	return b.triangulate();
}

Primitive* CGALPrimitive::simplify(int level)
{
#if CGAL_VERSION_NR > CGAL_VERSION_NUMBER(4,2,0)
	namespace SMS=CGAL::Surface_mesh_simplification;
	CGAL::Polyhedron3& p=*this->getPolyhedron();
	SMS::Count_stop_predicate<CGAL::Polyhedron3> stop(level);
	SMS::edge_collapse(p,stop,
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(4,7,0)
						CGAL::parameters::vertex_index_map(get(CGAL::vertex_external_index,p))
#else
						CGAL::vertex_index_map(get(CGAL::vertex_external_index,p))
#endif
						.halfedge_index_map(get(CGAL::halfedge_external_index,p))
						.get_cost(SMS::Edge_length_cost<CGAL::Polyhedron3>())
						.get_placement(SMS::Midpoint_placement<CGAL::Polyhedron3>()));
	return new CGALPrimitive(p);
#else
	return this;
#endif
}

Primitive* CGALPrimitive::copy()
{
	CGALPrimitive* p=new CGALPrimitive();
	this->buildPrimitive();
	p->nefPolyhedron=new CGAL::NefPolyhedron3(*nefPolyhedron);
	return p;
}


void CGALPrimitive::transform(TransformMatrix* matrix)
{
	decimal* m=matrix->getValues();
	CGAL::AffTransformation3 t(
		m[ 0], m[ 1], m[ 2], m[ 3],
		m[ 4], m[ 5], m[ 6], m[ 7],
		m[ 8], m[ 9], m[10], m[11],
	  /*m[12], m[13], m[14]*/m[15]);

	if(nefPolyhedron) {
		nefPolyhedron->transform(t);
	} else {
		QList<CGAL::Point3> nps;
		foreach(CGAL::Point3 pt, points)
			nps.append(pt.transform(t));
		points=nps;
	}

	foreach(Primitive* p, children)
		p->transform(matrix);
}

QList<Polygon*> CGALPrimitive::getPolygons() const
{
	return QList<Polygon*>();
}

const CGAL::NefPolyhedron3& CGALPrimitive::getNefPolyhedron()
{
	this->buildPrimitive();
	return *nefPolyhedron;
}

CGAL::Polyhedron3* CGALPrimitive::getPolyhedron()
{
	this->buildPrimitive();
	CGAL::Polyhedron3* poly = new CGAL::Polyhedron3();
	if(nefPolyhedron->is_simple())
		nefPolyhedron->convert_to_polyhedron(*poly);
	return poly;
}

bool CGALPrimitive::isEmpty()
{
	this->buildPrimitive();
	return nefPolyhedron->is_empty();
}

CGAL::Circle3 CGALPrimitive::getRadius()
{
	QList<CGAL::Point3> points3;
	if(nefPolyhedron) {
		CGALExplorer e(this);
		points3=e.getPoints();
	} else {
		points3=points;
	}

	typedef  CGAL::Min_circle_2_traits_2<CGAL::Kernel3> Traits;
	typedef  CGAL::Min_circle_2<Traits> Min_circle;

	QList<CGAL::Point2> points2;
	foreach(CGAL::Point3 pt3,points3) {
		CGAL::Point2 pt2(pt3.x(),pt3.y());
		points2.append(pt2);
	}

	Min_circle mc2(points2.begin(),points2.end(),true);
	Min_circle::Circle circle2=mc2.circle();
	CGAL::Point2 center2=circle2.center();
	CGAL::Point3 center3(center2.x(),center2.y(),0);
	CGAL::Scalar sq_r=circle2.squared_radius();

	return CGAL::Circle3(center3,sq_r,CGAL::Vector3(0,0,1));

}

CGALVolume CGALPrimitive::getVolume(bool calcMass)
{
	this->buildPrimitive();
	Primitive* p=copy();
	p=p->decompose();

	CGALExplorer e(p);
	return e.getVolume(calcMass);
}

bool CGALPrimitive::isFullyDimentional()
{
	this->buildPrimitive();
	//For fully dimentional polyhedra there are always two volumes the outer
	//volume and the inner volume. So check volumes > 1
	return nefPolyhedron->number_of_volumes()>1;
}

QList<Primitive*> CGALPrimitive::getChildren()
{
	return children;
}

void CGALPrimitive::appendChild(Primitive* p)
{
	children.append(p);
}

static CGAL::Point3 discretePoint(CGAL::Point3& pt,int places)
{
	CGAL::Scalar x,y,z;
	x=r_round(pt.x(),places);
	y=r_round(pt.y(),places);
	z=r_round(pt.z(),places);
	return CGAL::Point3(x,y,z);
}

class DiscreteNef : public CGAL::NefPolyhedron3
{
public:
	void discrete(int places) {
		Vertex_iterator v;
		CGAL_forall_vertices(v,snc()) {
			v->point()=discretePoint(v->point(),places);
		}
	}
};

void CGALPrimitive::discrete(int places)
{
	if(nefPolyhedron) {
		DiscreteNef* n=static_cast<DiscreteNef*>(nefPolyhedron);
		n->discrete(places);
	} else {
		QList<CGAL::Point3> nps;
		foreach(CGAL::Point3 pt, points) {
			nps.append(discretePoint(pt,places));
		}
		points=nps;
	}
}

CGALPrimitive::Unionable& CGALPrimitive::Unionable::operator+(Unionable& other)
{
	if(force||primitive->overlaps(other.primitive))
		primitive=primitive->join(other.primitive);
	else
		primitive=primitive->group(other.primitive);

	return *this;
}
#endif
