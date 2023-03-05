/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#include "complexvalue.h"
#include "context.h"
#include "node/transformationnode.h"
#include "numbervalue.h"
#include "rmath.h"
#include "vectorvalue.h"

RotateModule::RotateModule(Reporter& r) : Module(r,"rotate")
{
	addDescription(tr("Rotates its children about the origin or an arbitrary axis."));
	addParameter("angle","vec3",tr("The angle of rotation in degress. It can be a single value or rotation about x,y,z. With the latter, three rotations are performed in the order x,y,z"));
	addParameter("vector","vec3",tr("The axis of rotation when used with a single angle value"));
}

Node* RotateModule::evaluate(const Context& ctx) const
{
	enum class RotationTypes {
		Axis,
		Origin,
		Quaternion
	};

	auto* n=new TransformationNode();
	n->setChildren(ctx.getInputNodes());

	RotationTypes rotation=RotationTypes::Axis;
	decimal a=0.0;
	decimal x=0.0;
	decimal y=0.0;
	decimal z=1.0;
	auto* angValue=getParameterArgument<NumberValue>(ctx,0);
	if(angValue) {
		a=angValue->getNumber();
		auto* vecValue=getParameterArgument<VectorValue>(ctx,1);
		if(vecValue) {
			const Point& v=vecValue->getPoint();
			x=v.x();
			y=v.y();
			z=v.z();
			rotation=RotationTypes::Axis;
		}
	} else {
		auto* vecValue=getParameterArgument<VectorValue>(ctx,0);
		if(vecValue) {
			const Point& v=vecValue->getPoint();
			x=v.x();
			y=v.y();
			z=v.z();
			rotation=RotationTypes::Origin;
		} else {
			auto* cpxValue=getParameterArgument<ComplexValue>(ctx,0);
			if(cpxValue) {
				cpxValue->toQuaternion(a,x,y,z);
				rotation=RotationTypes::Quaternion;
			}
		}
	}

	if(rotation==RotationTypes::Origin) {

		const decimal& cx = r_right_cos(x);
		const decimal& cy = r_right_cos(y);
		const decimal& cz = r_right_cos(z);
		const decimal& sx = r_right_sin(x);
		const decimal& sy = r_right_sin(y);
		const decimal& sz = r_right_sin(z);

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
			cy*cz,cz*sx*sy-cx*sz,cx*cz*sy+sx*sz,0.0,
			cy*sz,cx*cz+sx*sy*sz,-cz*sx+cx*sy*sz,0.0,
			-sy,cy*sx,cx*cy,0.0,
			0.0,0.0,0.0,1.0
		);

		n->setMatrix(RzRyRx);

	} else if(rotation==RotationTypes::Axis) {

		const decimal& c=r_right_cos(a);
		const decimal& s=r_right_sin(a);

		const decimal& mag = r_sqrt(x*x + y*y + z*z,false);
		if(mag==0.0)
			return n;

		const decimal& u = x/mag;
		const decimal& v = y/mag;
		const decimal& w = z/mag;
		const decimal& c1=1.0-c;

		auto* TxyTzRaTzTxy = new TransformMatrix(
			u*u*c1+c,u*v*c1-w*s,u*w*c1+v*s,0.0,
			u*v*c1+w*s,v*v*c1+c,v*w*c1-u*s,0.0,
			u*w*c1-v*s,v*w*c1+u*s,w*w*c1+c,0.0,
			0.0,0.0,0.0,1.0
		);

		n->setMatrix(TxyTzRaTzTxy);

	} else {

		const decimal& xx=x*x;
		const decimal& xy=x*y;
		const decimal& xz=x*z;
		const decimal& xa=x*a;

		const decimal& yy=y*y;
		const decimal& yz=y*z;
		const decimal& ya=y*a;

		const decimal& zz=z*z;
		const decimal& za=z*a;

		/* The following rotation matrix is the same
		 * as above for axis rotations, with the exception
		 * that no normalisation is done. */
		auto* mx = new TransformMatrix(
			1.0-2.0*(yy+zz),2.0*(xy-za),2.0*(xz+ya),0.0,
			2.0*(xy+za),1.0-2.0*(xx+zz),2.0*(yz-xa),0.0,
			2.0*(xz-ya),2.0*(yz+xa),1.0-2.0*(xx+yy),0.0,
			0.0,0.0,0.0,1.0
		);

		n->setMatrix(mx);

	}

	return n;
}
