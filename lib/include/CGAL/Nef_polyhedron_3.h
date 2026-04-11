#ifndef CGAL_NEF_POLYHEDRON_3_H
#define CGAL_NEF_POLYHEDRON_3_H

#include <CGAL/Handle_for.h>
#include <CGAL/Nef_3/Default_items.h>
#include <CGAL/Nef_3/SNC_const_decorator.h>
#include <CGAL/Nef_3/SNC_decorator.h>
#include <CGAL/Nef_3/SNC_point_locator.h>
#include <CGAL/Nef_3/SNC_sphere_map.h>
#include <CGAL/Nef_3/SNC_structure.h>
#include <CGAL/Nef_polyhedron_S2.h>

namespace CGAL {

template <typename Kernel_, typename Items_, typename Mark_>
class Nef_polyhedron_3_rep {};

template <typename Kernel_,
          typename Items_ = typename Default_items<Kernel_>::Items,
          typename Mark_ = bool>
class Nef_polyhedron_3
    : public Handle_for<Nef_polyhedron_3_rep<Kernel_, Items_, Mark_>>,
      public SNC_const_decorator<SNC_structure<Kernel_, Items_, Mark_>> {
 public:
  using Kernel = Kernel_;
  using Items = Items_;
  using Mark = Mark_;

  using SNC_structure = SNC_structure<Kernel, Items, Mark>;
  using Sphere_map = typename SNC_structure::Sphere_map;

  using Nef_polyhedron_S2 = Nef_polyhedron_S2<Kernel, Items, Mark, Sphere_map>;
  using SM_decorator = SM_decorator<Sphere_map>;
  using SM_const_decorator = SM_const_decorator<Sphere_map>;
  using SFace_handle = typename SM_decorator::SFace_handle;
  using SFace_const_iterator = typename SM_decorator::SFace_const_iterator;

  using SNC_decorator = SNC_decorator<SNC_structure>;
  using Vertex_iterator = typename SNC_decorator::Vertex_iterator;
  using Vertex_const_handle = typename SNC_structure::Vertex_const_handle;
  using Shell_entry_iterator = typename SNC_structure::Shell_entry_iterator;
  using Volume_iterator = typename SNC_decorator::Volume_iterator;
  using SNC_point_locator = SNC_point_locator<SNC_decorator>;
  using Aff_transformation_3 = typename Kernel::Aff_transformation_3;

  struct Points_tag {};

  struct Polylines_tag {};

  struct SNC_and_PL {
    SNC_structure* sncp;
    SNC_point_locator* pl;
  };

  Nef_polyhedron_3() = default;
  Nef_polyhedron_3(const Nef_polyhedron_3&) {}
  Nef_polyhedron_3 operator=(const Nef_polyhedron_3&) {}

  template <class PolygonMesh>
  explicit Nef_polyhedron_3(const PolygonMesh&) {}

  template <typename InputIterator>
  Nef_polyhedron_3(InputIterator, InputIterator) {}

  template <typename InputIterator>
  Nef_polyhedron_3(InputIterator, InputIterator, Points_tag) {}

  template <typename InputIterator>
  Nef_polyhedron_3(InputIterator, InputIterator, Polylines_tag) {}

  SNC_point_locator*& pl() {}
  const SNC_point_locator* pl() const {}

  SNC_structure& snc() {}
  const SNC_structure& snc() const {}

  Nef_polyhedron_3 boundary() const {}

  Nef_polyhedron_3 complement() const {}

  template <typename Polyhedron>
  void convert_inner_shell_to_polyhedron(SFace_const_iterator, Polyhedron&) {}

  void delegate(Modifier_base<SNC_structure>&, bool = false, bool = true) {}

  void delegate(Modifier_base<SNC_and_PL>&, bool = false, bool = false) {}

  Nef_polyhedron_S2 get_sphere_map(Vertex_const_handle) const {}

  bool is_empty() const {}
  bool is_valid(bool = false, int = 0) {}

  Nef_polyhedron_3 join(const Nef_polyhedron_3&) const {}
  Nef_polyhedron_3 difference(const Nef_polyhedron_3&) const {}
  Nef_polyhedron_3 intersection(const Nef_polyhedron_3&) const {}
  Nef_polyhedron_3 symmetric_difference(const Nef_polyhedron_3&) const {}

  Nef_polyhedron_3 operator*(const Nef_polyhedron_3&) const {}
  Nef_polyhedron_3 operator+(const Nef_polyhedron_3&) const {}
  Nef_polyhedron_3 operator-(const Nef_polyhedron_3&) const {}
  Nef_polyhedron_3 operator^(const Nef_polyhedron_3&) const {}
  Nef_polyhedron_3 operator!() const {}

  void transform(const Aff_transformation_3&) {}
};

}  // namespace CGAL

#include <CGAL/enable_warnings.h>

#endif  // CGAL_NEF_POLYHEDRON_3_H
