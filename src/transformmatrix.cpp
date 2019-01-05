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

#include "transformmatrix.h"

TransformMatrix::TransformMatrix()
{
	int n=0;
	for(auto i=0; i<4; ++i)
		for(auto j=0; j<4; ++j)
			matrix[i][j]=(n++%5)?0.0:1.0;
}

TransformMatrix::TransformMatrix(
	const decimal& m00,const decimal& m01,const decimal& m02,const decimal& m03,
	const decimal& m10,const decimal& m11,const decimal& m12,const decimal& m13,
	const decimal& m20,const decimal& m21,const decimal& m22,const decimal& m23,
	const decimal& m30,const decimal& m31,const decimal& m32,const decimal& m33)
{
	matrix[0][0]=m00; matrix[0][1]=m01; matrix[0][2]=m02; matrix[0][3]=m03;
	matrix[1][0]=m10; matrix[1][1]=m11; matrix[1][2]=m12; matrix[1][3]=m13;
	matrix[2][0]=m20; matrix[2][1]=m21; matrix[2][2]=m22; matrix[2][3]=m23;
	matrix[3][0]=m30; matrix[3][1]=m31; matrix[3][2]=m32; matrix[3][3]=m33;
}

void TransformMatrix::setValue(int i,int j,const decimal& d)
{
	matrix[i][j]=d;
}

#ifdef USE_CGAL
CGAL::AffTransformation3 TransformMatrix::getTransform() const
{
	return CGAL::AffTransformation3(
				matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
				matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
				matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
			  /*matrix[3][0], matrix[3][1], matrix[3][2]*/matrix[3][3]);
}
#endif

QString TransformMatrix::toString() const
{
	QString result;
	result.append("[[");
	for(auto i=0; i<4; ++i) {
		if(i>0)
			result.append("],[");

		for(auto j=0; j<4; ++j) {
			if(j>0)
				result.append(",");

			result.append(to_string(matrix[i][j]));
		}
	}
	result.append("]]");
	return result;
}

