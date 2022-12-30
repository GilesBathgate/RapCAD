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
#include <QList>

class PointF;
class SegmentF;
class FacetF;

class CGALRenderer : public Renderer
{
public:
	explicit CGALRenderer(Primitive*);
	~CGALRenderer() override;
	void paint(QOpenGLFunctions_1_0&,bool,bool) override;
	void preferencesUpdated() override;
	void setCompiling(bool) override;

private:
	void fillDisplayLists(QOpenGLFunctions_1_0&);
	void drawVertices(QOpenGLFunctions_1_0&,const PointF&) const;
	void drawEdges(QOpenGLFunctions_1_0&,const SegmentF&) const;
	void drawFacets(QOpenGLFunctions_1_0&,const FacetF&) const;
	friend class NefConverter;
	void appendVertex(const PointF&);
	void appendEdge(const SegmentF&);
	void appendFacet(const FacetF&);
	const QColor& getVertexColor(bool mark) const;
	const QColor& getEdgeColor(bool mark) const;
	const QColor& getFacetColor(bool mark) const;
	GLfloat getVertexSize() const;
	GLfloat getEdgeSize() const;
	void loadPreferences();
	static void desaturate(QColor&);
	void descendChildren(Primitive* pr);

	QColor markedVertexColor;
	QColor vertexColor;
	QColor markedEdgeColor;
	QColor edgeColor;
	QColor markedFacetColor;
	QColor facetColor;
	SimpleRenderer* simple;
	class DisplayList* displayList;
	float vertexSize;
	float edgeSize;
	QList<PointF> vertices;
	QList<SegmentF> edges;
	QList<FacetF> facets;
};

#endif // CGALRENDERER_H
#endif
