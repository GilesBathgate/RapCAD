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

#include "beziersurfacemodule.h"
#include "context.h"
#include "fragment.h"
#include "node/primitivenode.h"
#include "rmath.h"
#include "vectorvalue.h"

BezierSurfaceModule::BezierSurfaceModule(Reporter& r) : PrimitiveModule(r,"bezier_surface")
{
	addDescription(tr("Constructs a bezier surface."));
	addParameter("mesh","mat4x4",tr("A 4 by 4 matrix of points."));
}

decimal BezierSurfaceModule::bez03(const decimal& u)
{
	return r_pow((1-u), 3);
}

decimal BezierSurfaceModule::bez13(const decimal& u)
{
	return 3*u*(r_pow((1-u),2));
}

decimal BezierSurfaceModule::bez23(const decimal& u)
{
	return 3*(r_pow(u,2))*(1-u);
}

decimal BezierSurfaceModule::bez33(const decimal& u)
{
	return r_pow(u,3);
}

Point BezierSurfaceModule::pointOnBez(const Points& cps, const decimal& u)
{
	decimal a=bez03(u);
	decimal b=bez13(u);
	decimal c=bez23(u);
	decimal d=bez33(u);
	decimal x=a*cps[0].x()+b*cps[1].x()+c*cps[2].x()+d*cps[3].x();
	decimal y=a*cps[0].y()+b*cps[1].y()+c*cps[2].y()+d*cps[3].y();
	decimal z=a*cps[0].z()+b*cps[1].z()+c*cps[2].z()+d*cps[3].z();

	return Point(x,y,z);
}

Point BezierSurfaceModule::pointOnBezMesh(const Mesh& mesh, const Vector& uv)
{
	Points p;
	decimal uv0=uv[0];
	p.append(pointOnBez(mesh[0], uv0));
	p.append(pointOnBez(mesh[1], uv0));
	p.append(pointOnBez(mesh[2], uv0));
	p.append(pointOnBez(mesh[3], uv0));

	return pointOnBez(p,uv[1]);
}

Node* BezierSurfaceModule::evaluate(const Context& ctx) const
{
	Mesh mesh;
	auto* meshVec=getParameterArgument<VectorValue>(ctx,0);

	auto* pn=new PrimitiveNode();
	Primitive* p=pn->createPrimitive();
	p->setType(PrimitiveTypes::Surface);
	pn->setChildren(ctx.getInputNodes());

	if(!meshVec)
		return pn;

	const int cols = 4;
	const int rows = 4;
	int row=0;
	for(Value* pointsVal: meshVec->getElements()) {
		Points points;
		auto* pointsVec=dynamic_cast<VectorValue*>(pointsVal);
		if(!pointsVec) continue;
		int col=0;
		for(Value* pointVal: pointsVec->getElements()) {
			auto* pointVec=dynamic_cast<VectorValue*>(pointVal);
			if(!pointVec) continue;
			points.append(pointVec->getPoint());
			if(++col >= cols) break;
		}
		if(col < cols)
			return pn;
		mesh.append(points);
		if(++row >= rows) break;
	}
	if(row < rows)
		return pn;

	int f = Fragment::getFragments(ctx,1);

	for(auto i=0; i<f; ++i) {
		for(auto j=0; j<f; ++j) {
			Vector a;
			decimal u=i;
			decimal v=j;
			a.append(u/f);
			a.append(v/f);

			p->createVertex(pointOnBezMesh(mesh,a));
		}
	}

	for(auto u=0; u<f-1; ++u) {
		for(auto v=0; v<f-1; ++v) {

			int i=(u*f)+v;
			int j=((u+1)*f)+v;
			int k=i+1;
			int l=j+1;
			createTriangle(p,i,j,k);
			createTriangle(p,l,k,j);
		}
	}

	return pn;
}
