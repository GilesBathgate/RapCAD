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

#include "transformmatrix.h"

TransformMatrix::TransformMatrix()
{
	for(int i=0; i<16; i++)
		matrix[i]=0.0;

	matrix[15]=1.0;
}

TransformMatrix::TransformMatrix(decimal m11, decimal m12, decimal m13, decimal m14,
                                 decimal m21, decimal m22, decimal m23, decimal m24,
                                 decimal m31, decimal m32, decimal m33, decimal m34,
                                 decimal m41, decimal m42, decimal m43, decimal m44)
{
	matrix[ 0]=m11;
	matrix[ 1]=m12;
	matrix[ 2]=m13;
	matrix[ 3]=m14;

	matrix[ 4]=m21;
	matrix[ 5]=m22;
	matrix[ 6]=m23;
	matrix[ 7]=m24;

	matrix[ 8]=m31;
	matrix[ 9]=m32;
	matrix[10]=m33;
	matrix[11]=m34;

	matrix[12]=m41;
	matrix[13]=m42;
	matrix[14]=m43;
	matrix[15]=m44;
}

decimal* TransformMatrix::getValues()
{
	return matrix;
}

void TransformMatrix::setValues(decimal* m)
{
	for(int i=0; i<16; i++)
		matrix[i]=m[i];
}

QString TransformMatrix::toString()
{
	QString result;
	result.append("[[");
	for(int i=0; i<16; i++) {
		if(i>0) {
			if(i%4)
				result.append(",");
			else
				result.append("],[");
		}
		result.append(to_string(matrix[i]));

	}
	result.append("]]");
	return result;
}

