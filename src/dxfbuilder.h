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
#ifndef DXFBUILDER_H
#define DXFBUILDER_H

#include <QString>
#include <dxflib/dl_creationinterface.h>

class DXFBuilder : private DL_CreationInterface
{
public:
	DXFBuilder(QString);
private:
	void addLayer(const DL_LayerData&);
	void addBlock(const DL_BlockData&);
	void endBlock();
	void addPoint(const DL_PointData&);
	void addLine(const DL_LineData&);
	void addArc(const DL_ArcData&);
	void addCircle(const DL_CircleData&);
	void addEllipse(const DL_EllipseData&);
	void addPolyline(const DL_PolylineData&);
	void addVertex(const DL_VertexData&);
	void addSpline(const DL_SplineData&);
	void addControlPoint(const DL_ControlPointData&);
	void addKnot(const DL_KnotData&);
	void addInsert(const DL_InsertData&);
	void addTrace(const DL_TraceData&);
	void add3dFace(const DL_3dFaceData&);
	void addSolid(const DL_SolidData&);
	void addMText(const DL_MTextData&);
	void addMTextChunk(const char* text);
	void addText(const DL_TextData&);
	void addDimAlign(const DL_DimensionData& data,
					 const DL_DimAlignedData& edata);
	void addDimLinear(const DL_DimensionData& data,
					  const DL_DimLinearData& edata);
	void addDimRadial(const DL_DimensionData& data,
					  const DL_DimRadialData& edata);
	void addDimDiametric(const DL_DimensionData& data,
						 const DL_DimDiametricData& edata);
	void addDimAngular(const DL_DimensionData& data,
					   const DL_DimAngularData& edata);
	void addDimAngular3P(const DL_DimensionData& data,
						 const DL_DimAngular3PData& edata);
	void addDimOrdinate(const DL_DimensionData& data,
						const DL_DimOrdinateData& edata);
	void addLeader(const DL_LeaderData&);
	void addLeaderVertex(const DL_LeaderVertexData&);
	void addHatch(const DL_HatchData&);
	void addImage(const DL_ImageData&);
	void linkImage(const DL_ImageDefData&);
	void addHatchLoop(const DL_HatchLoopData&);
	void addHatchEdge(const DL_HatchEdgeData&);
	void endEntity();
	void addComment(const char* comment);
	void setVariableVector(const char* key,
						   double v1, double v2, double v3, int code);
	void setVariableString(const char* key, const char* value, int code);
	void setVariableInt(const char* key, int value, int code);
	void setVariableDouble(const char* key, double value, int code);
	void endSequence();
};

#endif // DXFBUILDER_H
#endif
