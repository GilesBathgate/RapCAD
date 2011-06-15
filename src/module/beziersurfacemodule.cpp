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

#include "beziersurfacemodule.h"
#include "vectorvalue.h"
#include "primitivenode.h"
#include "math.h"

BezierSurfaceModule::BezierSurfaceModule() : Module("bezier_surface")
{
}

double BezierSurfaceModule::bez03(double u)
{
	return pow((1-u), 3);
}

double BezierSurfaceModule::bez13(double u)
{
	return 3*u*(pow((1-u),2));
}

double BezierSurfaceModule::bez23(double u)
{
	return 3*(pow(u,2))*(1-u);
}

double BezierSurfaceModule::bez33(double u)
{
	return pow(u,3);
}

Point BezierSurfaceModule::pointOnBez(Points cps, double u)
{
	double a=bez03(u),b=bez13(u),c=bez23(u),d=bez33(u);
	double x=a*cps[0].getX()+b*cps[1].getX()+c*cps[2].getX()+d*cps[3].getX();
	double y=a*cps[0].getY()+b*cps[1].getY()+c*cps[2].getY()+d*cps[3].getY();
	double z=a*cps[0].getZ()+b*cps[1].getZ()+c*cps[2].getZ()+d*cps[3].getZ();

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

BezierSurfaceModule::Points BezierSurfaceModule::getCurveQuad(Mesh mesh,Vector u1v1,Vector u2v2)
{
	Vector a;
	a.append(u1v1[0]);
	a.append(u2v2[1]);
	Vector b;
	b.append(u2v2[0]);
	b.append(u1v1[1]);

	Points p;
	p.append(pointOnBezMesh(mesh, a));
	p.append(pointOnBezMesh(mesh, u1v1));
	p.append(pointOnBezMesh(mesh, b));
	p.append(pointOnBezMesh(mesh, u2v2));

	return p;
}

Node* BezierSurfaceModule::evaluate(Context* ctx, QList<Node*>)
{
	Mesh mesh;
	VectorValue* meshVec=dynamic_cast<VectorValue*>(ctx->getArgument(0,"mesh"));
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

	int steps=24; //TODO use getfragments and $fn,$fa,$fs variables;

	PrimitiveNode* p=new PrimitiveNode();
	for(double ustep=0; ustep<steps; ustep++) {
		for(double vstep=0; vstep<steps; vstep++) {
			double ufrac1=ustep/steps;
			double ufrac2=(ustep+1)/steps;
			double vfrac1=vstep/steps;
			double vfrac2=(vstep+1)/steps;
			Vector a;
			a.append(ufrac1);
			a.append(vfrac1);
			Vector b;
			b.append(ufrac2);
			b.append(vfrac2);
			Points quad = getCurveQuad(mesh,a,b);

			p->createPolygon();
			p->appendVertex(quad[0]);
			p->appendVertex(quad[1]);
			p->appendVertex(quad[2]);

			p->createPolygon();
			p->appendVertex(quad[0]);
			p->appendVertex(quad[2]);
			p->appendVertex(quad[3]);
		}
	}

	return p;
}
