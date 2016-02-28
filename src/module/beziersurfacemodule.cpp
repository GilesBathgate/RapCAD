/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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
#include "vectorvalue.h"
#include "node/primitivenode.h"
#include "rmath.h"

BezierSurfaceModule::BezierSurfaceModule(Reporter* r) : Module(r,"bezier_surface")
{
	addDescription(tr("Constructs a bezier surface."));
	addParameter("mesh",tr("A 4 by 4 matrix of points."));
}

decimal BezierSurfaceModule::bez03(decimal u)
{
	return r_pow((1-u), 3);
}

decimal BezierSurfaceModule::bez13(decimal u)
{
	return 3*u*(r_pow((1-u),2));
}

decimal BezierSurfaceModule::bez23(decimal u)
{
	return 3*(r_pow(u,2))*(1-u);
}

decimal BezierSurfaceModule::bez33(decimal u)
{
	return r_pow(u,3);
}

Point BezierSurfaceModule::pointOnBez(Points cps, decimal u)
{
	decimal a=bez03(u),b=bez13(u),c=bez23(u),d=bez33(u);
	decimal x=a*cps[0].getX()+b*cps[1].getX()+c*cps[2].getX()+d*cps[3].getX();
	decimal y=a*cps[0].getY()+b*cps[1].getY()+c*cps[2].getY()+d*cps[3].getY();
	decimal z=a*cps[0].getZ()+b*cps[1].getZ()+c*cps[2].getZ()+d*cps[3].getZ();

	return Point(x,y,z);
}

Point BezierSurfaceModule::pointOnBezMesh(Mesh mesh,Vector uv)
{
	Points p;
	p.append(pointOnBez(mesh[0], uv[0]));
	p.append(pointOnBez(mesh[1], uv[0]));
	p.append(pointOnBez(mesh[2], uv[0]));
	p.append(pointOnBez(mesh[3], uv[0]));

	return pointOnBez(p,uv[1]);
}

Node* BezierSurfaceModule::evaluate(Context* ctx)
{
	Mesh mesh;
	VectorValue* meshVec=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	if(meshVec) {
		foreach(Value* pointsVal,meshVec->getChildren()) {
			Points points;
			VectorValue* pointsVec=dynamic_cast<VectorValue*>(pointsVal);
			if(pointsVec)
				foreach(Value* pointVal,pointsVec->getChildren()) {
				VectorValue* pointVec=dynamic_cast<VectorValue*>(pointVal);
				points.append(pointVec->getPoint());
			}
			mesh.append(points);
		}
	}

	int f=24; //TODO use getfragments and $fn,$fa,$fs variables;

	PrimitiveNode* p=new PrimitiveNode(reporter);
	p->setChildren(ctx->getInputNodes());

	for(int i=0; i<f; i++) {
		for(int j=0; j<f; j++) {
			Vector a;
			decimal u=i;
			decimal v=j;
			a.append(u/f);
			a.append(v/f);

			p->createVertex(pointOnBezMesh(mesh,a));
		}
	}

	Polygon* pg;
	for(int u=0; u<f-1; u++) {
		for(int v=0; v<f-1; v++) {

			int i=(u*f)+v;
			int j=((u+1)*f)+v;
			int k=i+1;
			int l=j+1;
			pg=p->createPolygon();
			pg->append(i);
			pg->append(j);
			pg->append(k);

			pg=p->createPolygon();
			pg->append(l);
			pg->append(k);
			pg->append(j);
		}
	}

	return p;
}
