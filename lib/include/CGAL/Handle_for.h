
#ifndef CGAL_HANDLE_FOR_H
#define CGAL_HANDLE_FOR_H

#include <CGAL/memory.h>

namespace CGAL {

template <class T, class Allocator = CGAL_ALLOCATOR(T)>
class Handle_for {
 public:
  Handle_for() = default;
  Handle_for(const T&) {}
  const T* Ptr() const noexcept {}
  bool is_shared() const noexcept {}
  bool unique() const noexcept {}
  void swap(Handle_for& h) noexcept {}

 protected:
  void copy_on_write() {}
  T* ptr() noexcept {}
  const T* ptr() const noexcept {}
};

template <class T>
inline bool identical(const T& t1, const T& t2) {}

template <class T>
inline const T& get_pointee_or_identity(const T& t) {}

}  // namespace CGAL

#include <CGAL/enable_warnings.h>

#endif  // CGAL_HANDLE_FOR_H
