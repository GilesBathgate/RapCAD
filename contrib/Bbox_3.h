// Copyright (c) 1999,2004
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/releases/CGAL-4.0-branch/Kernel_23/include/CGAL/Bbox_3.h $
// $Id: Bbox_3.h 67093 2012-01-13 11:22:39Z lrineau $
//
// Author(s)     : Andreas Fabri

#ifndef CGAL__BBOX_3_H
#define CGAL__BBOX_3_H

#include <CGAL/array.h>

namespace CGAL {

template < class R_ >
class _Bbox_3
{
public:
  typedef typename R_::FT FT;

		_Bbox_3() {}

		_Bbox_3(FT x_min, FT y_min, FT z_min,
			   FT x_max, FT y_max, FT z_max)
	  : rep(CGAL::make_array(x_min, y_min, z_min, x_max, y_max, z_max)) {}

  inline bool operator==(const _Bbox_3 &b) const
  {
	return xmin() == b.xmin() && xmax() == b.xmax()
		&& ymin() == b.ymin() && ymax() == b.ymax()
		&& zmin() == b.zmin() && zmax() == b.zmax();
  }

  inline bool operator!=(const _Bbox_3 &b) const
  {
	return ! (b == *this);
  }

  inline int dimension() const { return 3; }

  inline FT  xmin() const { return rep[0]; }
  inline FT  ymin() const { return rep[1]; }
  inline FT  zmin() const { return rep[2]; }
  inline FT  xmax() const { return rep[3]; }
  inline FT  ymax() const { return rep[4]; }
  inline FT  zmax() const { return rep[5]; }

  inline FT min BOOST_PREVENT_MACRO_SUBSTITUTION (int i) const
  {
	CGAL_kernel_precondition( (i == 0 ) || ( i == 1 ) || ( i == 2) );
	if (i == 0) { return xmin(); }
	if (i == 1) { return ymin(); }
	return zmin();
  }

  inline FT max BOOST_PREVENT_MACRO_SUBSTITUTION (int i) const
  {
	CGAL_kernel_precondition( (i == 0 ) || ( i == 1 ) || ( i == 2 ) );
	if (i == 0) { return xmax(); }
	if (i == 1) { return ymax(); }
	return zmax();
  }

  _Bbox_3  operator+(const _Bbox_3& b) const
  {
	return _Bbox_3((std::min)(xmin(), b.xmin()),
				  (std::min)(ymin(), b.ymin()),
				  (std::min)(zmin(), b.zmin()),
				  (std::max)(xmax(), b.xmax()),
				  (std::max)(ymax(), b.ymax()),
				  (std::max)(zmax(), b.zmax()));
  }
private:
   cpp0x::array<FT, 6>   rep;
};

} //namespace CGAL

#endif // CGAL__Bbox_3_H
