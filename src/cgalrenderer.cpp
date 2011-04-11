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

#include "cgalrenderer.h"
using CGAL::OGL::SNC_BOUNDARY;

class NotImplementedException{};

CGALRenderer::CGALRenderer()
{
}

void CGALRenderer::draw(bool showedges) const
{
	if(this->style == SNC_BOUNDARY) {
		glCallList(this->object_list_+2);
		if(showedges) {
			glDisable(GL_LIGHTING);
			glCallList(this->object_list_+1);
			glCallList(this->object_list_);
		}
	} else {
		glDisable(GL_LIGHTING);
		glCallList(this->object_list_+1);
		glCallList(this->object_list_);
	}
}

CGAL::Color CGALRenderer::getVertexColor(Vertex_iterator) const
{
    throw new NotImplementedException();
}

CGAL::Color CGALRenderer::getEdgeColor(Edge_iterator) const
{
    throw new NotImplementedException();
}

CGAL::Color CGALRenderer::getFacetColor(Halffacet_iterator) const
{
    throw new NotImplementedException();
}
