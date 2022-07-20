/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2022 Giles Bathgate
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
#ifndef CGALRENDERER_H
#define CGALRENDERER_H

#include "renderer.h"
#include "simplerenderer.h"
#include <QColor>
#include <contrib/OGL_helper.h>

class CGALRenderer : public Renderer, private CGAL::OGL::Polyhedron
{
public:
	explicit CGALRenderer(Primitive*);
	~CGALRenderer() override;
	void paint(bool,bool) override;
	void preferencesUpdated() override;
	void setCompiling(bool) override;
private:
	static void setColor(CGAL::IO::Color&,const QColor&);
	CGAL::IO::Color getVertexColor(bool mark) const override;
	CGAL::IO::Color getEdgeColor(bool mark) const override;
	CGAL::IO::Color getFacetColor(bool mark) const override;
	float getVertexSize() const override;
	float getEdgeSize() const override;
	void loadPreferences();
	static void desaturate(CGAL::IO::Color& c);
	void descendChildren(Primitive* pr);

	CGAL::IO::Color markedVertexColor;
	CGAL::IO::Color vertexColor;
	CGAL::IO::Color markedEdgeColor;
	CGAL::IO::Color edgeColor;
	CGAL::IO::Color markedFacetColor;
	CGAL::IO::Color facetColor;
	SimpleRenderer* simple;
	float vertexSize;
	float edgeSize;
};

#endif // CGALRENDERER_H
#endif
