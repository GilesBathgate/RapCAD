// Copyright (c) 1997-2002  Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0+
//
//
// Author(s)     : Peter Hachenberger <hachenberger@mpi-sb.mpg.de>

#ifndef CGAL_NEF_OPENGL_HELPER_H
#define CGAL_NEF_OPENGL_HELPER_H

#include <CGAL/Nef_S2/OGL_base_object.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Nef_3/SNC_decorator.h>
#include <qgl.h>
#include <CGAL/glu.h>
#include <cstdlib>

#define CGAL_NEF3_MARKED_VERTEX_COLOR 183,232,92
#define CGAL_NEF3_MARKED_EDGE_COLOR 171,216,86
#define CGAL_NEF3_MARKED_FACET_COLOR  157,203,81

#define CGAL_NEF3_UNMARKED_VERTEX_COLOR 255,246,124
#define CGAL_NEF3_UNMARKED_EDGE_COLOR 255,236,94
#define CGAL_NEF3_UNMARKED_FACET_COLOR 249,215,44

#ifdef _WIN32
#define CGAL_GLU_TESS_CALLBACK CALLBACK
#else
#define CGAL_GLU_TESS_CALLBACK
#endif

#ifdef __APPLE__
#  include <AvailabilityMacros.h>
#endif

#if defined __APPLE__ && !defined MAC_OS_X_VERSION_10_5
    #define CGAL_GLU_TESS_DOTS ...
#else
    #define CGAL_GLU_TESS_DOTS
#endif


namespace CGAL {

namespace OGL {

// ----------------------------------------------------------------------------
// Drawable float types:
// ----------------------------------------------------------------------------

  typedef CGAL::Simple_cartesian<float> DKernel;
  typedef DKernel::Point_3               float_point;
  typedef DKernel::Vector_3              float_vector;
  typedef DKernel::Segment_3             float_segment;
  typedef DKernel::Aff_transformation_3  Affine_3;

  // DPoint = a float point including a mark
  class DPoint : public float_point {
    bool m_;
  public:
    DPoint() {}
    DPoint(const float_point& p, bool m) : float_point(p) { m_ = m; }
    DPoint(const DPoint& p) : float_point(p) { m_ = p.m_; }
    DPoint& operator=(const DPoint& p)
    { float_point::operator=(p); m_ = p.m_; return *this; }
    bool mark() const { return m_; }
  };

  // DSegment = a float segment including a mark
  class DSegment : public float_segment {
    bool m_;
  public:
    DSegment() {}
    DSegment(const float_segment& s, bool m) : float_segment(s) { m_ = m; }
    DSegment(const DSegment& s) : float_segment(s) { m_ = s.m_; }
    DSegment& operator=(const DSegment& s)
    { float_segment::operator=(s); m_ = s.m_; return *this; }
    bool mark() const { return m_; }
  };

  // float_triple = a class that stores a triple of float
  // coordinates; we need a pointer to the coordinates in a C array
  // for OpenGL
  class float_triple {
    typedef float*       float_ptr;
    typedef const float* const_float_ptr;
    float coords_[3];
  public:
    float_triple()
    { coords_[0]=coords_[1]=coords_[2]=0.0; }
    float_triple(float x, float y, float z)
    { coords_[0]=x; coords_[1]=y; coords_[2]=z; }
    float_triple(const float_triple& t)
    { coords_[0]=t.coords_[0];
      coords_[1]=t.coords_[1];
      coords_[2]=t.coords_[2];
    }
    float_triple& operator=(const float_triple& t)
    { coords_[0]=t.coords_[0];
      coords_[1]=t.coords_[1];
      coords_[2]=t.coords_[2];
      return *this; }
    operator float_ptr() const
    { return const_cast<float_triple&>(*this).coords_; }
    float operator[](unsigned i)
    { CGAL_assertion(i<3); return coords_[i]; }
  }; // float_triple

  static std::ostream& operator << (std::ostream& os,
				    const float_triple& t)
  { os << "(" << t[0] << "," << t[1] << "," << t[2] << ")";
    return os; }


  // DFacet stores the facet cycle vertices in a continuus C array
  // of three float components, this is necessary due to the OpenGL
  // tesselator input format !
  class DFacet {
    typedef std::vector<float_triple>   Coord_vector;
    typedef std::vector<unsigned>        Cycle_vector;
    Coord_vector    coords_;  // stores all vertex coordinates
    Cycle_vector    fc_ends_; // stores entry points of facet cycles
    float_triple   normal_; // stores normal and mark of facet
    bool            mark_;

  public:
    typedef Coord_vector::iterator Coord_iterator;
    typedef Coord_vector::const_iterator Coord_const_iterator;

    DFacet()
    { mark_ = false; }

    void push_back_vertex(float x, float y, float z)
    { coords_.push_back(float_triple(x,y,z)); }

    DFacet(const DFacet& f)
    { coords_  = f.coords_;
      fc_ends_ = f.fc_ends_;
      normal_  = f.normal_;
      mark_    = f.mark_;
    }

    DFacet& operator=(const DFacet& f)
    { coords_ =  f.coords_;
      fc_ends_ = f.fc_ends_;
      normal_ =  f.normal_;
      mark_    = f.mark_;
      return *this;
    }

    ~DFacet()
    { coords_.clear(); fc_ends_.clear(); }

    void push_back_vertex(const float_point& p)
    { push_back_vertex(p.x(),p.y(),p.z()); }

    void set_normal(double x, double y, double z, bool m)
    { double l = sqrt(x*x + y*y + z*z);
      normal_ = float_triple(x/l,y/l,z/l); mark_ = m; }

    float dx() const { return normal_[0]; }
    float dy() const { return normal_[1]; }
    float dz() const { return normal_[2]; }
    bool mark() const { return mark_; }
    float* normal() const
    { return static_cast<float*>(normal_); }

    void new_facet_cycle()
    { fc_ends_.push_back((unsigned)coords_.size()); }

    unsigned number_of_facet_cycles() const
    { return (unsigned)fc_ends_.size(); }

    Coord_iterator facet_cycle_begin(unsigned i)
    { CGAL_assertion(i<number_of_facet_cycles());
      if (i==0) return coords_.begin();
      else return coords_.begin()+fc_ends_[i]; }

    Coord_iterator facet_cycle_end(unsigned i)
    { CGAL_assertion(i<number_of_facet_cycles());
      if (i<fc_ends_.size()-1) return coords_.begin()+fc_ends_[i+1];
      else return coords_.end(); }

    Coord_const_iterator facet_cycle_begin(unsigned i) const
    { CGAL_assertion(i<number_of_facet_cycles());
      if (i==0) return coords_.begin();
      else return coords_.begin()+fc_ends_[i]; }

    Coord_const_iterator facet_cycle_end(unsigned i) const
    { CGAL_assertion(i<number_of_facet_cycles());
      if (i<fc_ends_.size()-1) return coords_.begin()+fc_ends_[i+1];
      else return coords_.end(); }

    void debug(std::ostream& os = std::cerr) const
    { os << "DFacet, normal=" << normal_ << ", mark=" << mark() << std::endl;
      for(unsigned i=0; i<number_of_facet_cycles(); ++i) {
	os << "  facet cycle ";
	// put all vertices in facet cycle into contour:
	Coord_const_iterator cit;
	for(cit = facet_cycle_begin(i); cit != facet_cycle_end(i); ++cit)
	  os << *cit;
	os << std::endl;
      }
    }

  }; // DFacet


// ----------------------------------------------------------------------------
// OGL Drawable Polyhedron:
// ----------------------------------------------------------------------------

  inline void CGAL_GLU_TESS_CALLBACK beginCallback(GLenum which)
  { glBegin(which); }

  inline void CGAL_GLU_TESS_CALLBACK endCallback(void)
  { glEnd(); }

  inline void CGAL_GLU_TESS_CALLBACK errorCallback(GLenum errorCode)
  { const GLubyte *estring;
    estring = gluErrorString(errorCode);
    fprintf(stderr, "Tessellation Error: %s\n", estring);
    //std::exit (0);
  }

  inline void CGAL_GLU_TESS_CALLBACK vertexCallback(GLvoid* vertex,
			                            GLvoid* user)
  { GLfloat* pc(static_cast<GLfloat*>(vertex));
    GLfloat* pu(static_cast<GLfloat*>(user));
    //    CGAL_NEF_TRACEN("vertexCallback coord  "<<pc[0]<<","<<pc[1]<<","<<pc[2]);
    //    CGAL_NEF_TRACEN("vertexCallback normal "<<pu[0]<<","<<pu[1]<<","<<pu[2]);
    glNormal3fv(pu);
    glVertex3fv(pc);
  }

  inline void CGAL_GLU_TESS_CALLBACK combineCallback(GLdouble coords[3], GLvoid *[4], GLfloat [4], GLvoid **dataOut)
  { static std::list<GLfloat*> pcache;
    if (dataOut) {
        GLfloat *n = new GLfloat[3];
        n[0] = coords[0];
        n[1] = coords[1];
        n[2] = coords[2];
        pcache.push_back(n);
        *dataOut = n;
    } else {
        for (std::list<GLfloat*>::const_iterator i = pcache.begin(); i != pcache.end(); i++)
            delete[] *i;
        pcache.clear();
    }
  }

 enum { SNC_AXES};
 enum { SNC_BOUNDARY, SNC_SKELETON };

 class Polyhedron : public OGL_base_object {
 protected:
    std::list<DPoint>    vertices_;
    std::list<DSegment>  edges_;
    std::list<DFacet>    halffacets_;

    GLuint         object_list_;
    bool init_;

    Bbox_3  bbox_;

    int style;
    std::vector<bool> switches;

    typedef std::list<DPoint>::const_iterator   Vertex_iterator;
    typedef std::list<DSegment>::const_iterator Edge_iterator;
    typedef std::list<DFacet>::const_iterator   Halffacet_iterator;

  public:
    Polyhedron() : bbox_(-1,-1,-1,1,1,1), switches(1) {
      object_list_ = 0;
      init_ = false;
      style = SNC_BOUNDARY;
      switches[SNC_AXES] = false;
    }

    /*
    Polyhedron(const Polyhedron& P) :
      object_list_(0),
      init_(false),
      bbox_(P.bbox_),
      style(P.style),
      switches(2) {

      switches[SNC_AXES] = P.switches[SNC_AXES];

      Vertex_iterator v;
      for(v=P.vertices_.begin();v!=P.vertices_.end();++v)
	vertices_.push_back(*v);
      Edge_iterator e;
      for(e=P.edges_.begin();e!=P.edges_.end();++e)
	edges_.push_back(*e);
      Halffacet_iterator f;
      for(f=P.halffacets_.begin();f!=P.halffacets_.end();++f)
	halffacets_.push_back(*f);
    }

    Polyhedron& operator=(const Polyhedron& P) {
      if (object_list_) glDeleteLists(object_list_, 4);
      object_list_ = 0;
      init_ = false;
      style = P.style;
      switches[SNC_AXES] = P.switches[SNC_AXES];

      Vertex_iterator v;
      vertices_.clear();
      for(v=P.vertices_.begin();v!=P.vertices_.end();++v)
	vertices_.push_back(*v);
      Edge_iterator e;
      edges_.clear();
      for(e=P.edges_.begin();e!=P.edges_.end();++e)
	edges_.push_back(*e);
      Halffacet_iterator f;
      halffacets_.clear();
      for(f=P.halffacets_.begin();f!=P.halffacets_.end();++f)
	halffacets_.push_back(*f);
      init();
      return *this;
    }
    */
    ~Polyhedron()
    { if (object_list_) glDeleteLists(object_list_, 4); }

    void push_back(const float_point& p, bool m) {
        vertices_.push_back(DPoint(p,m));
    }
    void push_back(const float_segment& s, bool m)
    { edges_.push_back(DSegment(s,m)); }
    void push_back(const DFacet& f)
    { halffacets_.push_back(f); }

    void toggle(int index) {
      switches[index] = !switches[index];
    }

    void set_style(int index) {
      style = index;
    }

    bool is_initialized() const { return init_; }

    Bbox_3  bbox() const { return bbox_; }
    Bbox_3& bbox()       { return bbox_; }

    virtual CGAL::Color getVertexColor(bool mark) const
    {
	CGAL::Color cf(CGAL_NEF3_MARKED_VERTEX_COLOR),
	  ct(CGAL_NEF3_UNMARKED_VERTEX_COLOR); // more blue-ish
	CGAL::Color c = mark ? ct : cf;
	return c;
    }

    virtual float getVertexSize() const
    {
        return 10;
    }

    void draw(Vertex_iterator v) const {
      //      CGAL_NEF_TRACEN("drawing vertex "<<*v);
      float p = getVertexSize();
      if(p==0) return;
      CGAL::Color c = getVertexColor(v->mark());
      glPointSize(p);
      glColor3ub(c.red(), c.green(), c.blue());
      glBegin(GL_POINTS);
      glVertex3f(v->x(),v->y(),v->z());
#ifdef CGAL_NEF_EMPHASIZE_VERTEX
      glColor3ub(255,0,0);
      glVertex3f(CGAL_NEF_EMPHASIZE_VERTEX);
#endif
      glEnd();
    }

    virtual CGAL::Color getEdgeColor(bool mark) const
    {
	CGAL::Color cf(CGAL_NEF3_MARKED_EDGE_COLOR),
	  ct(CGAL_NEF3_UNMARKED_EDGE_COLOR); // more blue-ish
	CGAL::Color c = mark ? ct : cf;
	return c;
    }

    virtual float getEdgeSize() const
    {
        return 5;
    }

    void draw(Edge_iterator e) const {
      //      CGAL_NEF_TRACEN("drawing edge "<<*e);
      float w = getEdgeSize();
      if(w==0) return;
      float_point p = e->source(), q = e->target();
      CGAL::Color c = getEdgeColor(e->mark());
      glLineWidth(w);
      glColor3ub(c.red(),c.green(),c.blue());
      glBegin(GL_LINE_STRIP);
      glVertex3f(p.x(), p.y(), p.z());
      glVertex3f(q.x(), q.y(), q.z());
      glEnd();
    }

    virtual CGAL::Color getFacetColor(bool mark) const
    {
	CGAL::Color cf(CGAL_NEF3_MARKED_FACET_COLOR),
	  ct(CGAL_NEF3_UNMARKED_FACET_COLOR); // more blue-ish
	CGAL::Color c = (mark ? ct : cf);
	return c;
    }

    void draw(Halffacet_iterator f) const {
      //      CGAL_NEF_TRACEN("drawing facet "<<(f->debug(),""));
      GLUtesselator* tess_ = gluNewTess();
      gluTessCallback(tess_, GLenum(GLU_TESS_VERTEX_DATA),
		      (GLvoid (CGAL_GLU_TESS_CALLBACK *)(CGAL_GLU_TESS_DOTS)) &vertexCallback);
      gluTessCallback(tess_, GLenum(GLU_TESS_COMBINE),
		      (GLvoid (CGAL_GLU_TESS_CALLBACK *)(CGAL_GLU_TESS_DOTS)) &combineCallback);
      gluTessCallback(tess_, GLenum(GLU_TESS_BEGIN),
		      (GLvoid (CGAL_GLU_TESS_CALLBACK *)(CGAL_GLU_TESS_DOTS)) &beginCallback);
      gluTessCallback(tess_, GLenum(GLU_TESS_END),
		      (GLvoid (CGAL_GLU_TESS_CALLBACK *)(CGAL_GLU_TESS_DOTS)) &endCallback);
      gluTessCallback(tess_, GLenum(GLU_TESS_ERROR),
		      (GLvoid (CGAL_GLU_TESS_CALLBACK *)(CGAL_GLU_TESS_DOTS)) &errorCallback);
      gluTessProperty(tess_, GLenum(GLU_TESS_WINDING_RULE),
		      GLU_TESS_WINDING_POSITIVE);

      DFacet::Coord_const_iterator cit;
      CGAL::Color c = getFacetColor(f->mark());
      glColor3ub(c.red(),c.green(),c.blue());
      gluTessBeginPolygon(tess_,f->normal());
      //      CGAL_NEF_TRACEN(" ");
      //      CGAL_NEF_TRACEN("Begin Polygon");
      gluTessNormal(tess_,f->dx(),f->dy(),f->dz());
      // forall facet cycles of f:
      for(unsigned i = 0; i < f->number_of_facet_cycles(); ++i) {
        gluTessBeginContour(tess_);
	//	CGAL_NEF_TRACEN("  Begin Contour");
	// put all vertices in facet cycle into contour:
	for(cit = f->facet_cycle_begin(i);
	    cit != f->facet_cycle_end(i); ++cit) {
          double loc[3];
          loc[0]=(*cit)[0];
          loc[1]=(*cit)[1];
          loc[2]=(*cit)[2];
	  gluTessVertex(tess_, loc, *cit);
	  //	  CGAL_NEF_TRACEN("    add Vertex");
	}
        gluTessEndContour(tess_);
	//	CGAL_NEF_TRACEN("  End Contour");
      }
      gluTessEndPolygon(tess_);
      //      CGAL_NEF_TRACEN("End Polygon");
      gluDeleteTess(tess_);
      combineCallback(NULL, NULL, NULL, NULL);
    }

    void construct_axes() const
    {
      glLineWidth(2.0);
      // red x-axis
      glColor3f(1.0,0.0,0.0);
      glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(5000.0,0.0,0.0);
      glEnd();
       // green y-axis
      glColor3f(0.0,1.0,0.0);
      glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,5000.0,0.0);
      glEnd();
      // blue z-axis and equator
      glColor3f(0.0,0.0,1.0);
      glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,0.0,5000.0);
      glEnd();
      // six coordinate points in pink:
      glPointSize(10);
      glBegin(GL_POINTS);
      glColor3f(1.0,0.0,0.0);
      glVertex3f(5,0,0);
      glColor3f(0.0,1.0,0.0);
      glVertex3f(0,5,0);
      glColor3f(0.0,0.0,1.0);
      glVertex3f(0,0,5);
      glEnd();
    }


    void fill_display_lists() {
      glNewList(object_list_, GL_COMPILE);
      Vertex_iterator v;
      for(v=vertices_.begin();v!=vertices_.end();++v)
	draw(v);
      glEndList();

      glNewList(object_list_+1, GL_COMPILE);
      Edge_iterator e;
      for(e=edges_.begin();e!=edges_.end();++e)
	draw(e);
      glEndList();

      glNewList(object_list_+2, GL_COMPILE);
      Halffacet_iterator f;
      for(f=halffacets_.begin();f!=halffacets_.end();++f)
	draw(f);
      glEndList();

      glNewList(object_list_+3, GL_COMPILE); // axes:
      construct_axes();
      glEndList();

    }

    void init() {
      if (init_) return;
      init_ = true;
      switches[SNC_AXES] = false;
      style = SNC_BOUNDARY;
      object_list_ = glGenLists(4);
      CGAL_assertion(object_list_);
      fill_display_lists();
    }


    void draw() const
    {
      if (!is_initialized()) const_cast<Polyhedron&>(*this).init();
      float l = (std::max)( (std::max)( bbox().xmax() - bbox().xmin(),
					 bbox().ymax() - bbox().ymin()),
			     bbox().zmax() - bbox().zmin());
      if ( l < 1) // make sure that a single point doesn't screw up here
          l = 1;
      glScalef( 4.0/l, 4.0/l, 4.0/l);
      glTranslatef( -(bbox().xmax() + bbox().xmin()) / 2.0,
                    -(bbox().ymax() + bbox().ymin()) / 2.0,
                    -(bbox().zmax() + bbox().zmin()) / 2.0);
      if (style == SNC_BOUNDARY) {
	//glEnable(GL_LIGHTING);
	glCallList(object_list_+2); // facets
	//glDisable(GL_LIGHTING);
      }
      // move edges and vertices a bit towards the view-point,
      // i.e., 1/100th of the unit vector in camera space
      //      float f = l / 4.0 / 100.0;
      //      glTranslatef( z_vec[0] * f, z_vec[1] * f, z_vec[2] * f);
      glCallList(object_list_+1); // edges
      glCallList(object_list_);   // vertices
      if (switches[SNC_AXES]) glCallList(object_list_+3); // axis
   }

    void debug(std::ostream& os = std::cerr) const
    {
      os << "OGL::Polyhedron" << std::endl;
      os << "Vertices:" << std::endl;
      Vertex_iterator v;
      for(v=vertices_.begin();v!=vertices_.end();++v)
	os << "  "<<*v<<", mark="<<v->mark()<<std::endl;
      os << "Edges:" << std::endl;
      Edge_iterator e;
      for(e=edges_.begin();e!=edges_.end();++e)
	os << "  "<<*e<<", mark="<<e->mark()<<std::endl;
      os << "Facets:" << std::endl;
      Halffacet_iterator f;
      for(f=halffacets_.begin();f!=halffacets_.end();++f)
	{ f->debug(); os << std::endl; }
      os << std::endl;
    }

  }; // Polyhedron

  template<typename Nef_polyhedron>
  class Nef3_Converter {
    typedef typename Nef_polyhedron::SNC_structure           SNC_structure;
    typedef CGAL::SNC_decorator<SNC_structure>               Base;
    typedef CGAL::SNC_FM_decorator<SNC_structure>            FM_decorator;

    public:
    typedef typename SNC_structure::Vertex_const_iterator Vertex_const_iterator;
    typedef typename SNC_structure::Halfedge_const_iterator Halfedge_const_iterator;
    typedef typename SNC_structure::Halffacet_const_iterator Halffacet_const_iterator;
    typedef typename SNC_structure::Halffacet_cycle_const_iterator Halffacet_cycle_const_iterator;

    typedef typename SNC_structure::Object_const_handle Object_const_handle;
    typedef typename SNC_structure::SHalfedge_const_handle SHalfedge_const_handle;
    typedef typename SNC_structure::SHalfloop_const_handle SHalfloop_const_handle;

    typedef typename SNC_structure::Vertex_const_handle Vertex_const_handle;
    typedef typename SNC_structure::Halfedge_const_handle Halfedge_const_handle;
    typedef typename SNC_structure::Halffacet_const_handle Halffacet_const_handle;

    typedef typename SNC_structure::Point_3 Point_3;
    typedef typename SNC_structure::Vector_3 Vector_3;
    typedef typename SNC_structure::Segment_3 Segment_3;
    typedef typename SNC_structure::Plane_3 Plane_3;
    typedef typename SNC_structure::Mark Mark;
    typedef typename SNC_structure::SHalfedge_around_facet_const_circulator
      SHalfedge_around_facet_const_circulator;

  private:
    static OGL::float_point float_point(const Point_3& p)
      { return OGL::float_point(CGAL::to_double(p.x()),
				 CGAL::to_double(p.y()),
				 CGAL::to_double(p.z())); }

    static OGL::float_segment float_segment(const Segment_3& s)
      { return OGL::float_segment(float_point(s.source()),
				   float_point(s.target())); }

    static void draw(Vertex_const_handle v, const Nef_polyhedron& ,
		     CGAL::OGL::Polyhedron& P) {
      Point_3 bp = v->point();
      //    CGAL_NEF_TRACEN("vertex " << bp);
      P.push_back(float_point(bp), v->mark());
    }

    static void draw(Halfedge_const_handle e, const Nef_polyhedron& ,
		     CGAL::OGL::Polyhedron& P) {
      Vertex_const_handle s = e->source();
      Vertex_const_handle t = e->twin()->source();
      Segment_3 seg(s->point(),t->point());
      //    CGAL_NEF_TRACEN("edge " << seg);
      P.push_back(float_segment(seg), e->mark());
    }

    static void draw(Halffacet_const_handle f, const Nef_polyhedron& ,
		     CGAL::OGL::Polyhedron& P) {
      OGL::DFacet g;
      Halffacet_cycle_const_iterator fc; // all facet cycles:
      CGAL_forall_facet_cycles_of(fc,f)
	if ( fc.is_shalfedge() ) { // non-trivial facet cycle
	  g.new_facet_cycle();
	  SHalfedge_const_handle h = fc;
	  SHalfedge_around_facet_const_circulator hc(h), he(hc);
	  CGAL_For_all(hc,he){ // all vertex coordinates in facet cycle
	    Point_3 sp = hc->source()->source()->point();
	    //	      CGAL_NEF_TRACEN(" ");CGAL_NEF_TRACEN("facet" << sp);
	    g.push_back_vertex(float_point(sp));
	  }
	}
      Vector_3 v = f->plane().orthogonal_vector();
      g.set_normal(CGAL::to_double(v.x()),
		   CGAL::to_double(v.y()),
		   CGAL::to_double(v.z()),
		   f->mark());
      P.push_back(g);
    }

    // Returns the bounding box of the finite vertices of the polyhedron.
    // Returns $[-1,+1]^3$ as bounding box if no finite vertex exists.

    static Bbox_3  bounded_bbox(const Nef_polyhedron& N) {
      bool first_vertex = true;
      Bbox_3 bbox( -1.0, -1.0, -1.0, 1.0, 1.0, 1.0);
      Vertex_const_iterator vi;
      CGAL_forall_vertices(vi, N) {
	Point_3 p = vi->point();
	float x = CGAL::to_double(p.hx());
	float y = CGAL::to_double(p.hy());
	float z = CGAL::to_double(p.hz());
	float w = CGAL::to_double(p.hw());
	if (N.is_standard(vi)) {
	  if(first_vertex) {
	    bbox = Bbox_3(x/w, y/w, z/w, x/w, y/w, z/w);
	    first_vertex = false;
	  } else {
	    bbox = bbox + Bbox_3(x/w, y/w, z/w, x/w, y/w, z/w);
	    first_vertex = false;
	  }
	}
      }
      return bbox;
    }

    static void set_R(Bbox_3& bbox, const Nef_polyhedron& N) {
      if(N.is_standard_kernel()) return;
      float size = abs(bbox.xmin());
      if(size < bbox.xmax()) size = bbox.xmax();
      if(size < bbox.ymin()) size = bbox.ymin();
      if(size < bbox.ymax()) size = bbox.ymax();
      if(size < bbox.zmin()) size = bbox.zmin();
      if(size < bbox.zmax()) size = bbox.zmax();
      N.set_size_of_infimaximal_box(size*50);
      //    CGAL_NEF_TRACEN("set infi box size to " << size);
      Vertex_const_iterator vi;
      CGAL_forall_vertices(vi, N)
	if(N.is_standard(vi))
	  return;
      bbox = Bbox_3(bbox.xmin()*10,bbox.ymin()*10,bbox.zmin()*10,
		    bbox.xmax()*10,bbox.ymax()*10,bbox.zmax()*10);
    }
  public:
    static void convert_to_OGLPolyhedron(const Nef_polyhedron& N, CGAL::OGL::Polyhedron* P) {
      Bbox_3 bbox(bounded_bbox(N));
      set_R(bbox,N);
      P->bbox() = bbox;
      Vertex_const_iterator v;
      CGAL_forall_vertices(v,*N.sncp()) draw(v,N,*P);
      Halfedge_const_iterator e;
      CGAL_forall_edges(e,*N.sncp()) draw(e,N,*P);
      Halffacet_const_iterator f;
      CGAL_forall_facets(f,*N.sncp()) draw(f,N,*P);
    }

  }; // Nef3_Converter

} // namespace OGL

} //namespace CGAL
#endif // CGAL_NEF_OPENGL_HELPER_H
