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

#include "rotatemodule.h"
#include "context.h"
#include "node/transformationnode.h"
#include "numbervalue.h"
#include "vectorvalue.h"
#include "complexvalue.h"
#include "rmath.h"

RotateModule::RotateModule(Reporter& r) : Module(r,"rotate")
{
	addDescription(tr("Rotates its children about the origin or an arbitrary axis."));
	addParameter("angle",tr("The angle of rotation in degress. It can be a single value or rotation about x,y,z. With the latter, three rotations are performed in the order x,y,z"));
	addParameter("vector",tr("The axis of rotation when used with a single angle value"));
}

Node* RotateModule::evaluate(const Context& ctx) const
{
	enum rotationType {
		axis,
		origin,
		quaternion
	};

	auto* n=new TransformationNode();
	n->setChildren(ctx.getInputNodes());

	rotationType rotation=axis;
	decimal a=0.0,x=0.0,y=0.0,z=1.0;
	auto* angValue=dynamic_cast<NumberValue*>(getParameterArgument(ctx,0));
	if(angValue) {
		a=angValue->getNumber();
		auto* vecValue=dynamic_cast<VectorValue*>(getParameterArgument(ctx,1));
		if(vecValue) {
			Point v=vecValue->getPoint();
			x=v.x(); y=v.y(); z=v.z();
			rotation=axis;
		}
	} else {
		auto* vecValue=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
		if(vecValue) {
			Point v=vecValue->getPoint();
			x=v.x(); y=v.y(); z=v.z();
			rotation=origin;
		} else {
			auto* cpxValue=dynamic_cast<ComplexValue*>(getParameterArgument(ctx,0));
			if(cpxValue) {
				cpxValue->toQuaternion(a,x,y,z);
				rotation=quaternion;
			}
		}
	}

	if(rotation==origin) {

		decimal cx = r_right_cos(x);
		decimal cy = r_right_cos(y);
		decimal cz = r_right_cos(z);
		decimal sx = r_right_sin(x);
		decimal sy = r_right_sin(y);
		decimal sz = r_right_sin(z);

		/*
		Given the three affine transformation matricies for counter-clockwise
		rotations
		Rz = {{cos(z),-sin(z),0,0},{sin(z),cos(z),0,0},{0,0,1,0},{0,0,0,1}}
		Ry = {{cos(y),0,sin(y),0},{0,1,0,0},{-sin(y),0,cos(y),0},{0,0,0,1}}
		Rx = {{1,0,0,0},{0,cos(x),-sin(x),0},{0,sin(x),cos(x),0},{0,0,0,1}}

		We can multiply the matrices to give RzRyRx

		I cheated and used wolfram alpha to do this:
		http://tinyurl.com/q4utr88
		*/

		auto* RzRyRx = new TransformMatrix(
			cy*cz,cz*sx*sy-cx*sz,cx*cz*sy+sx*sz,0,
			cy*sz,cx*cz+sx*sy*sz,-cz*sx+cx*sy*sz,0,
			-sy,cy*sx,cx*cy,0,
			0,0,0,1
		);

		n->setMatrix(RzRyRx);

	} else if(rotation==axis) {

		decimal c=r_right_cos(a);
		decimal s=r_right_sin(a);

		decimal mag = r_sqrt(x*x + y*y + z*z,false);
		if(mag==0)
			return n;

		decimal u = x/mag;
		decimal v = y/mag;
		decimal w = z/mag;
		decimal c1=1-c;

		auto* TxyTzRaTzTxy = new TransformMatrix(
			u*u*c1+c,u*v*c1-w*s,u*w*c1+v*s,0,
			u*v*c1+w*s,v*v*c1+c,v*w*c1-u*s,0,
			u*w*c1-v*s,v*w*c1+u*s,w*w*c1+c,0,
			0,0,0,1
		);

		n->setMatrix(TxyTzRaTzTxy);

	} else {

		decimal xx=x*x;
		decimal xy=x*y;
		decimal xz=x*z;
		decimal xa=x*a;

		decimal yy=y*y;
		decimal yz=y*z;
		decimal ya=y*a;

		decimal zz=z*z;
		decimal za=z*a;

		/* The following rotation matrix is the same
		 * as above for axis rotations, with the exception
		 * that no normalisation is done. */
		auto* mx = new TransformMatrix(
			1-2*(yy+zz),2*(xy-za),2*(xz+ya),0,
			2*(xy+za),1-2*(xx+zz),2*(yz-xa),0,
			2*(xz-ya),2*(yz+xa),1-2*(xx+yy),0,
			0,0,0,1
		);

		n->setMatrix(mx);

	}

	return n;
}
