/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include "cgalexplorer.h"
#include "float.h"

typedef CGAL::NefPolyhedron3::Volume_const_iterator VolumeIterator;
typedef CGAL::NefPolyhedron3::Shell_entry_const_iterator ShellIterator;

CGALExplorer::CGALExplorer(const CGAL::NefPolyhedron3& p) : poly(p)
{
}

void CGALExplorer::visit(VertexHandle v)
{
	CGAL::Point3 p = v->point();
	points.append(p);
}

void CGALExplorer::evaluate()
{
	VolumeIterator c;
	CGAL_forall_volumes(c,poly) {
		ShellIterator it;
		CGAL_forall_shells_of(it,c) {
			poly.visit_shell_objects(SFaceHandle(it),*this);
		}
	}
}

CGALExplorer::Polygon CGALExplorer::getPoints()
{
	evaluate();
	return points;
}

CGAL::Bbox_3 CGALExplorer::getBounds()
{
	evaluate();

	double minX=DBL_MAX,maxX=-DBL_MAX;
	double minY=DBL_MAX,maxY=-DBL_MAX;
	double minZ=DBL_MAX,maxZ=-DBL_MAX;
	foreach(CGAL::Point3 pt,points) {
		double x=to_double(pt.x());
		double y=to_double(pt.y());
		double z=to_double(pt.z());
		minX=fmin(x,minX);
		maxX=fmax(x,maxX);
		minY=fmin(y,minY);
		maxY=fmax(y,maxY);
		minZ=fmin(z,minZ);
		maxZ=fmax(z,maxZ);
	}

	return CGAL::Bbox_3(minX,minY,minZ,maxX,maxY,maxZ);
}
