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

#include "rotatemodule.h"
#include "transformationnode.h"
#include "vectorvalue.h"
#include "tau.h"

RotateModule::RotateModule() : Module("rotate")
{
}

Node* RotateModule::evaluate(Context* ctx,QList<Node*> childs)
{
	Point v;
	VectorValue* vec=dynamic_cast<VectorValue*>(ctx->getArgument(0,"vector"));
	if(vec)
		v=vec->getPoint();

	double x=0,y=0,z=0;
	v.getXYZ(x,y,z);

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
	 Rx = [(1,0,0,0),(0,cos(x),sin(x),0),(0,-sin(x),cos(x),0),(0,0,0,1)]
	 Ry = [(cos(y),0,-sin(y),0),(0,1,0,0),(sin(y),0,cos(y),0),(0,0,0,1)]
	 Rz = [(cos(z),sin(z),0,0),(-sin(z),cos(z),0,0),(0,0,1,0),(0,0,0,1)]

	 We can multiply the matrices to give RxRyRz

	 I cheated and used wolfram alpha to do this:
	 http://tinyurl.com/3m8jmdj
	*/
	double RxRyRz[16] = {
				 cy*cz,        cy*sz,   -sy,0,
		cz*sx*sy-cx*sz,cx*cz+sx*sy*sz,cy*sx,0,
		cx*cz*sy+sx*sz,cx*sy*sz-cz*sx,cx*cy,0,
					 0,             0,    0,1
	};

	TransformationNode* n=new TransformationNode();
	for(int i=0; i<16; i++)
		n->matrix[i]=RxRyRz[i];

	n->setChildren(childs);
	return n;
}
