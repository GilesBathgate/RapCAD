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

#include <math.h>
#include "rotatemodule.h"
#include "node/transformationnode.h"
#include "numbervalue.h"
#include "vectorvalue.h"
#include "tau.h"

RotateModule::RotateModule() : Module("rotate")
{
}

Node* RotateModule::evaluate(Context* ctx)
{
	TransformationNode* n=new TransformationNode();

	bool origin;
	Point vec;
	double a=0.0;
	NumberValue* angValue=dynamic_cast<NumberValue*>(ctx->getArgument(0,"angle"));
	if(angValue) {
		a=angValue->getNumber();
		VectorValue* vecValue=dynamic_cast<VectorValue*>(ctx->getArgument(1,"vector"));
		if(vecValue)
			vec=vecValue->getPoint();
		origin=false;
	} else {
		VectorValue* vecValue=dynamic_cast<VectorValue*>(ctx->getArgument(0,"vector"));
		if(vecValue)
			vec=vecValue->getPoint();
		origin=true;
	}

	double x=0.0,y=0.0,z=0.0;
	vec.getXYZ(x,y,z);
	if(x==0.0&&y==0.0&&z==0.0)
		origin=true;

	if(origin) {

		//Convert angles to radians
		double rx=x*M_TAU/360.0;
		double ry=y*M_TAU/360.0;
		double rz=z*M_TAU/360.0;

		double cx = cos(rx);
		double cy = cos(ry);
		double cz = cos(rz);
		double sx = sin(rx);
		double sy = sin(ry);
		double sz = sin(rz);

		/*
		Given the three affine transformation matricies for anti-clockwise
		rotations
		Rx = {{1,0,0,0},{0,cos(x),sin(x),0},{0,-sin(x),cos(x),0},{0,0,0,1}}
		Ry = {{cos(y),0,-sin(y),0},{0,1,0,0},{sin(y),0,cos(y),0},{0,0,0,1}}
		Rz = {{cos(z),sin(z),0,0},{-sin(z),cos(z),0,0},{0,0,1,0},{0,0,0,1}}

		We can multiply the matrices to give RxRyRz

		I cheated and used wolfram alpha to do this:
		http://tinyurl.com/3m8jmdj
		*/
		double RxRyRz[16] = {
			cy*cz,cy*sz,-sy,0,
			cz*sx*sy-cx*sz,cx*cz+sx*sy*sz,cy*sx,0,
			cx*cz*sy+sx*sz,cx*sy*sz-cz*sx,cx*cy,0,
			0,0,0,1
		};

		for(int i=0; i<16; i++)
			n->matrix[i]=RxRyRz[i];

	} else {

		double ra=a*M_TAU/360.0;
		double c=cos(ra);
		double s=sin(ra);

		double mag = sqrt(x*x + y*y + z*z);
		double u = x/mag;
		double v = y/mag;
		double w = z/mag;

		double TxyTzRaTzTxy[16] = {
			u*u*(1-c)+c,u*v*(1-c)+w*s,u*w*(1-c)-v*s,0,
			u*v*(1-c)-w*s,v*v*(1-c)+c,v*w*(1-c)+u*s,0,
			u*w*(1-c)+v*s,v*w*(1-c)-u*s,w*w*(1-c)+c,0,
			0,0,0,1
		};

		for(int i=0; i<16; i++)
			n->matrix[i]=TxyTzRaTzTxy[i];
	}
	n->setChildren(ctx->getInputNodes());
	return n;
}
