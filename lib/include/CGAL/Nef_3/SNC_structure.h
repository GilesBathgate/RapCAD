#ifndef CGAL_SNC_STRUCTURE_H
#define CGAL_SNC_STRUCTURE_H

#include <CGAL/Nef_3/SNC_list.h>
#include <CGAL/memory.h>

namespace CGAL {

template <typename HE>
struct move_shalfedge_around_facet {
  void forward(HE& e) const {}
  void backward(HE& e) const {}
};

template <class Object, class Hash_map, class Union_find>
void merge_sets(Object, Object, Hash_map, Union_find&) {}

template <typename Kernel_, typename Items_, typename Mark_>
class SNC_sphere_map;

template <typename S>
class SM_decorator;

template <typename S>
class SNC_decorator;

template <typename S>
class SNC_io_parser;

template <typename Kernel_, typename Items_, typename Mark_>
class SNC_structure {
 public:
  using Items = Items_;
  using Kernel = Kernel_;
  using Mark = Mark_;

  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Sphere_kernel = Sphere_geometry<Kernel>;

  using Sphere_map = SNC_sphere_map<Kernel, Items, Mark>;
  using SM_decorator = SM_decorator<Sphere_map>;
  using SNC_decorator = SNC_decorator<SNC_structure>;

  using Point_3 = typename Kernel::Point_3;
  using Plane_3 = typename Kernel::Plane_3;
  using Vector_3 = typename Kernel::Vector_3;
  using Direction_3 = typename Kernel::Direction_3;
  using Segment_3 = typename Kernel::Segment_3;
  using Line_3 = typename Kernel::Line_3;
  using Ray_3 = typename Kernel::Ray_3;
  using Triangle_3 = typename Kernel::Triangle_3;

  using Aff_transformation_3 = typename Kernel::Aff_transformation_3;

  using Sphere_point = typename Sphere_kernel::Sphere_point;
  using Sphere_segment = typename Sphere_kernel::Sphere_segment;
  using Sphere_circle = typename Sphere_kernel::Sphere_circle;
  using Sphere_direction = typename Sphere_kernel::Sphere_direction;
  using Size_type = size_t;

  using Vertex_base = Sphere_map;
  using Vertex = SNC_in_place_list_sm<Vertex_base>;
  using Vertex_list = In_place_list<Vertex, false>;
  using Vertex_alloc = CGAL_ALLOCATOR(Vertex);
  using Vertex_handle = typename Vertex_list::iterator;
  using Vertex_const_handle = typename Vertex_list::const_iterator;
  using Vertex_iterator = typename Vertex_list::iterator;
  using Vertex_const_iterator = typename Vertex_list::const_iterator;

  using Halffacet_base = typename Items::template Halffacet<SNC_structure>;
  using Halffacet = SNC_in_place_list_halffacet<Halffacet_base>;
  using Halffacet_list = In_place_list<Halffacet, false>;
  using Halffacet_alloc = CGAL_ALLOCATOR(Halffacet);
  using Halffacet_handle = typename Halffacet_list::iterator;
  using Halffacet_const_handle = typename Halffacet_list::const_iterator;
  using Halffacet_iterator = typename Halffacet_list::iterator;
  using Halffacet_const_iterator = typename Halffacet_list::const_iterator;

  using Volume_base = typename Items::template Volume<SNC_structure>;
  using Volume = SNC_in_place_list_volume<Volume_base>;
  using Volume_list = In_place_list<Volume, false>;
  using Volume_alloc = CGAL_ALLOCATOR(Volume);
  using Volume_handle = typename Volume_list::iterator;
  using Volume_const_handle = typename Volume_list::const_iterator;
  using Volume_iterator = typename Volume_list::iterator;
  using Volume_const_iterator = typename Volume_list::const_iterator;

  using SVertex_base = typename Items::template SVertex<SNC_structure>;
  using SVertex = SNC_in_place_list_svertex<SVertex_base>;
  using SVertex_list = In_place_list<SVertex, false>;
  using SVertex_alloc = CGAL_ALLOCATOR(SVertex);
  using SVertex_handle = typename SVertex_list::iterator;
  using SVertex_const_handle = typename SVertex_list::const_iterator;
  using SVertex_iterator = typename SVertex_list::iterator;
  using SVertex_const_iterator = typename SVertex_list::const_iterator;

  using Halfedge_base = typename Items::template SVertex<SNC_structure>;
  using Halfedge = SNC_in_place_list_svertex<SVertex_base>;
  using Halfedge_list = In_place_list<SVertex, false>;
  using Halfedge_alloc = CGAL_ALLOCATOR(SVertex);
  using Halfedge_handle = typename SVertex_list::iterator;
  using Halfedge_const_handle = typename SVertex_list::const_iterator;
  using Halfedge_iterator = typename SVertex_list::iterator;
  using Halfedge_const_iterator = typename SVertex_list::const_iterator;

  using SHalfedge_base = typename Items::template SHalfedge<SNC_structure>;
  using SHalfedge = SNC_in_place_list_shalfedge<SHalfedge_base>;
  using SHalfedge_list = In_place_list<SHalfedge, false>;
  using SHalfedge_alloc = CGAL_ALLOCATOR(SHalfedge);
  using SHalfedge_handle = typename SHalfedge_list::iterator;
  using SHalfedge_const_handle = typename SHalfedge_list::const_iterator;
  using SHalfedge_iterator = typename SHalfedge_list::iterator;
  using SHalfedge_const_iterator = typename SHalfedge_list::const_iterator;

  using SHalfloop_base = typename Items::template SHalfloop<SNC_structure>;
  using SHalfloop = SNC_in_place_list_shalfloop<SHalfloop_base>;
  using SHalfloop_list = In_place_list<SHalfloop, false>;
  using SHalfloop_alloc = CGAL_ALLOCATOR(SHalfloop);
  using SHalfloop_handle = typename SHalfloop_list::iterator;
  using SHalfloop_const_handle = typename SHalfloop_list::const_iterator;
  using SHalfloop_iterator = typename SHalfloop_list::iterator;
  using SHalfloop_const_iterator = typename SHalfloop_list::const_iterator;

  using SFace_base = typename Items::template SFace<SNC_structure>;
  using SFace = SNC_in_place_list_sface<SFace_base>;
  using SFace_list = In_place_list<SFace, false>;
  using SFace_alloc = CGAL_ALLOCATOR(SFace);
  using SFace_handle = typename SFace_list::iterator;
  using SFace_const_handle = typename SFace_list::const_iterator;
  using SFace_iterator = typename SFace_list::iterator;
  using SFace_const_iterator = typename SFace_list::const_iterator;

  using Object_handle = Object_handle;
  using Object_list = std::list<Object_handle>;
  using Object_iterator = Object_list::iterator;
  using Object_const_iterator = Object_list::const_iterator;
  using Object_const_handle = Object_list::const_iterator;

  using SHalfedge_around_svertex_circulator =
      typename Sphere_map::SHalfedge_around_svertex_circulator;
  using SHalfedge_around_sface_circulator =
      typename Sphere_map::SHalfedge_around_sface_circulator;
  using SHalfedge_around_svertex_const_circulator =
      typename Sphere_map::SHalfedge_around_svertex_const_circulator;
  using SHalfedge_around_sface_const_circulator =
      typename Sphere_map::SHalfedge_around_sface_const_circulator;

  using Infi_box = typename Sphere_map::Infi_box;
  using Standard_kernel = typename Infi_box::Standard_kernel;

  using Constructor_parameter = Vertex_handle;
  using Constructor_const_parameter = Vertex_const_handle;

  using SHalfedge_around_facet_const_circulator =
      CircFromIt<SHalfedge_const_iterator,
                 move_shalfedge_around_facet<SHalfedge_const_iterator>>;
  using SHalfedge_around_facet_circulator =
      CircFromIt<SHalfedge_iterator,
                 move_shalfedge_around_facet<SHalfedge_iterator>>;

  class SFace_cycle_iterator : public Object_iterator {
    using Ibase = Object_iterator;

   public:
    SFace_cycle_iterator() : Ibase() {}
    SFace_cycle_iterator(const Ibase& b) : Ibase(b) {}
    bool is_svertex() const {}
    bool is_shalfedge() const {}
    bool is_shalfloop() const {}
    operator SVertex_handle() const {}
    operator SHalfedge_handle() const {}
    operator SHalfloop_handle() const {}
    operator Object_handle() const {}
    Object_handle& operator*() const {}
    Object_handle operator->() const {}
  };

  class SFace_cycle_const_iterator : public Object_const_iterator {
    using Ibase = Object_const_iterator;

   public:
    SFace_cycle_const_iterator() : Ibase() {}
    SFace_cycle_const_iterator(const Ibase& b) : Ibase(b) {}
    bool is_svertex() const {}
    bool is_shalfedge() const {}
    bool is_shalfloop() const {}
    operator SVertex_const_handle() const {}
    operator SHalfedge_const_handle() const {}
    operator SHalfloop_const_handle() const {}
    operator Object_handle() const {}
    const Object_handle& operator*() const {}
    Object_handle operator->() const {}
  };

  class Halffacet_cycle_iterator : public Object_iterator {
    using Ibase = Object_iterator;

   public:
    Halffacet_cycle_iterator() : Ibase() {}
    Halffacet_cycle_iterator(const Ibase& b) : Ibase(b) {}
    bool is_shalfedge() const {}
    bool is_shalfloop() const {}
    operator SHalfedge_handle() const {}
    operator SHalfloop_handle() const {}
    operator Object_handle() const {}
    Object_handle& operator*() const {}
    Object_handle operator->() const {}
  };

  class Halffacet_cycle_const_iterator : public Object_const_iterator {
    using Ibase = Object_const_iterator;

   public:
    Halffacet_cycle_const_iterator() : Ibase() {}
    Halffacet_cycle_const_iterator(const Ibase& b) : Ibase(b) {}
    bool is_shalfedge() const {}
    bool is_shalfloop() const {}
    operator SHalfedge_const_handle() const {}
    operator SHalfloop_const_handle() const {}
    operator Object_handle() const {}
    const Object_handle& operator*() const {}
    Object_handle operator->() const {}
  };

  class Shell_entry_iterator : public Object_iterator {
    using Ibase = Object_iterator;

   public:
    Shell_entry_iterator() : Ibase() {}
    Shell_entry_iterator(const Ibase& b) : Ibase(b) {}
    operator SFace_handle() const {}
    operator Object_handle() const {}
    Object_handle& operator*() const {}
    Object_handle operator->() const {}
  };

  class Shell_entry_const_iterator : public Object_const_iterator {
    using Ibase = Object_const_iterator;

   public:
    Shell_entry_const_iterator() : Ibase() {}
    Shell_entry_const_iterator(const Ibase& b) : Ibase(b) {}
    operator SFace_const_handle() const {}
    operator Object_handle() const {}
    const Object_handle& operator*() const {}
    Object_handle operator->() const {}
  };

  Vertex_iterator vertices_begin() {}
  Vertex_iterator vertices_end() {}
  Vertex_const_iterator vertices_begin() const {}
  Vertex_const_iterator vertices_end() const {}

  Halfedge_iterator halfedges_begin() {}
  Halfedge_iterator halfedges_end() {}
  Halfedge_const_iterator halfedges_begin() const {}
  Halfedge_const_iterator halfedges_end() const {}

  Halffacet_iterator halffacets_begin() {}
  Halffacet_iterator halffacets_end() {}
  Halffacet_const_iterator halffacets_begin() const {}
  Halffacet_const_iterator halffacets_end() const {}

  Volume_iterator volumes_begin() {}
  Volume_iterator volumes_end() {}
  Volume_const_iterator volumes_begin() const {}
  Volume_const_iterator volumes_end() const {}

  SVertex_iterator svertices_begin() {}
  SVertex_iterator svertices_end() {}

  SHalfedge_iterator shalfedges_begin() {}
  SHalfedge_iterator shalfedges_end() {}
  SHalfedge_const_iterator shalfedges_begin() const {}
  SHalfedge_const_iterator shalfedges_end() const {}

  SHalfloop_iterator shalfloops_begin() {}
  SHalfloop_iterator shalfloops_end() {}
  SHalfloop_const_iterator shalfloops_begin() const {}
  SHalfloop_const_iterator shalfloops_end() const {}

  SFace_iterator sfaces_begin() {}
  SFace_iterator sfaces_end() {}

  Size_type number_of_vertices() const {}
  Size_type number_of_halfedges() const {}

  Size_type number_of_halffacets() const {}
  Size_type number_of_volumes() const {}

  Size_type number_of_shalfedges() const {}
  Size_type number_of_shalfloops() const {}
  Size_type number_of_sfaces() const {}

  Size_type number_of_facets() const {}
  Size_type number_of_edges() const {}

  template <typename H>
  Object_iterator& sm_boundary_item(H) {}

  void reserve_sm_boundary_items(Size_type) {}

  template <typename H>
  bool is_boundary_object(H) const {}

  template <typename H>
  bool is_sm_boundary_object(H) const {}

  template <typename H>
  void store_boundary_item(H, Object_iterator) {}

  template <typename H>
  void store_sm_boundary_item(H, Object_iterator) {}

  template <typename H>
  void undef_boundary_item(H) {}

  template <typename H>
  void undef_sm_boundary_item(H) {}

  void reset_sm_object_list(Object_list&) {}
  void reset_object_list(Object_list&) {}

  Vertex_handle new_vertex(const Point_3& = Point_3(), Mark = Mark()) {}
  Vertex_handle new_vertex_only() {}

  Halfedge_handle new_halfedge_only() {}
  Halfedge_handle new_halfedge_only(Halfedge_handle) {}

  Halffacet_handle new_halffacet_only() {}
  Halffacet_handle new_halffacet_pair(const Plane_3& = Plane_3(),
                                      Mark = Mark()) {}

  Volume_handle new_volume(Mark = Mark()) {}
  Volume_handle new_volume_only() {}

  SHalfloop_handle new_shalfloop_only() {}

  SHalfedge_handle new_shalfedge_only() {}
  SHalfedge_handle new_shalfedge_only(SHalfedge_handle) {}

  SFace_handle new_sface_only() {}
  SFace_handle new_sface_only(SFace_handle) {}

  void delete_vertex(Vertex_handle) {}
  void delete_volume(Volume_handle) {}
  void delete_halffacet_pair(Halffacet_handle) {}
  void delete_halfedge_pair(Halfedge_handle) {}

  void delete_sface_only(SFace_handle) {}
  void delete_shalfloop_only(SHalfloop_handle) {}
  void delete_shalfedge_only(SHalfedge_handle) {}
  void delete_halfedge_only(Halfedge_handle) {}

  void clear_boundary() {}
  void clear_snc_boundary() {}
  void clear() {}
  bool is_empty() const {}
};

}  // namespace CGAL
#endif  // CGAL_SNC_STRUCTURE_H
