/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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

#ifdef USE_DXF
#include "dxfbuilder.h"
#include <dxflib/dl_dxf.h>

DXFBuilder::DXFBuilder(QString file)
{
	DL_Dxf* dxf = new DL_Dxf();
	dxf->in(file.toStdString(),this);
	delete dxf;
}

void DXFBuilder::addLayer(const DL_LayerData&)
{
}

void DXFBuilder::addBlock(const DL_BlockData&)
{
}

void DXFBuilder::endBlock()
{
}

void DXFBuilder::addPoint(const DL_PointData&)
{
}

void DXFBuilder::addLine(const DL_LineData&)
{
}

void DXFBuilder::addArc(const DL_ArcData&)
{
}

void DXFBuilder::addCircle(const DL_CircleData&)
{
}

void DXFBuilder::addEllipse(const DL_EllipseData&)
{
}

void DXFBuilder::addPolyline(const DL_PolylineData&)
{
}

void DXFBuilder::addVertex(const DL_VertexData&)
{
}

void DXFBuilder::addSpline(const DL_SplineData&)
{
}

void DXFBuilder::addControlPoint(const DL_ControlPointData&)
{
}

void DXFBuilder::addKnot(const DL_KnotData&)
{
}

void DXFBuilder::addInsert(const DL_InsertData&)
{
}

void DXFBuilder::addTrace(const DL_TraceData&)
{
}

void DXFBuilder::add3dFace(const DL_3dFaceData&)
{
}

void DXFBuilder::addSolid(const DL_SolidData&)
{
}

void DXFBuilder::addMText(const DL_MTextData&)
{
}

void DXFBuilder::addMTextChunk(const char*)
{
}

void DXFBuilder::addText(const DL_TextData&)
{
}

void DXFBuilder::addDimAlign(const DL_DimensionData&,
							 const DL_DimAlignedData&)
{
}

void DXFBuilder::addDimLinear(const DL_DimensionData&,
							  const DL_DimLinearData&)
{
}

void DXFBuilder::addDimRadial(const DL_DimensionData&,
							  const DL_DimRadialData&)
{
}

void DXFBuilder::addDimDiametric(const DL_DimensionData&,
								 const DL_DimDiametricData&)
{
}

void DXFBuilder::addDimAngular(const DL_DimensionData&,
							   const DL_DimAngularData&)
{
}

void DXFBuilder::addDimAngular3P(const DL_DimensionData&,
								 const DL_DimAngular3PData&)
{
}

void DXFBuilder::addDimOrdinate(const DL_DimensionData&,
								const DL_DimOrdinateData&)
{
}

void DXFBuilder::addLeader(const DL_LeaderData&)
{
}

void DXFBuilder::addLeaderVertex(const DL_LeaderVertexData&)
{
}

void DXFBuilder::addHatch(const DL_HatchData&)
{
}

void DXFBuilder::addImage(const DL_ImageData&)
{
}

void DXFBuilder::linkImage(const DL_ImageDefData&)
{
}

void DXFBuilder::addHatchLoop(const DL_HatchLoopData&)
{
}

void DXFBuilder::addHatchEdge(const DL_HatchEdgeData&)
{
}

void DXFBuilder::endEntity()
{
}

void DXFBuilder::addComment(const char*)
{
}

void DXFBuilder::setVariableVector(const char*,
								   double, double, double, int)
{
}

void DXFBuilder::setVariableString(const char*, const char*, int)
{
}

void DXFBuilder::setVariableInt(const char*, int, int)
{
}

void DXFBuilder::setVariableDouble(const char*, double, int)
{
}

void DXFBuilder::endSequence()
{
}
#endif
