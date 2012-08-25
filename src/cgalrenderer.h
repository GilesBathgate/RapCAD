/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2012 Giles Bathgate
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

#ifndef CGALRENDERER_H
#define CGALRENDERER_H

#include <QColor>
#include "contrib/OGL_helper.h"
#include "renderer.h"
#include "cgalprimitive.h"

class CGALRenderer : public Renderer, private CGAL::OGL::Polyhedron
{
public:
	CGALRenderer(Primitive*);
	void draw(bool,bool);
private:
	void setColor(CGAL::Color&,QColor);
	CGAL::Color getVertexColor(bool mark) const;
	CGAL::Color getEdgeColor(bool mark) const;
	CGAL::Color getFacetColor(bool mark) const;
	double getVertexSize() const;
	double getEdgeSize() const;
	CGAL::Color markedVertexColor;
	CGAL::Color vertexColor;
	CGAL::Color markedEdgeColor;
	CGAL::Color edgeColor;
	CGAL::Color markedFacetColor;
	CGAL::Color facetColor;
	double vertexSize;
	double edgeSize;
};

#endif // CGALRENDERER_H
