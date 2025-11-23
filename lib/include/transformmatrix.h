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

#ifndef TRANSFORMMATRIX_H
#define TRANSFORMMATRIX_H

#include "decimal.h"
#include <QGenericMatrix>

enum class TransformType
{
	Translation,
	UniformScaling,
	Rotation,
	Custom
};

class TransformMatrix
{
	static constexpr int N=4;
	static constexpr int M=4;
public:
	TransformMatrix();
	TransformMatrix(
					const decimal&,const decimal&,const decimal&,const decimal&,
					const decimal&,const decimal&,const decimal&,const decimal&,
					const decimal&,const decimal&,const decimal&,const decimal&,
					const decimal&,const decimal&,const decimal&,const decimal&);
	void setValue(int,int,const decimal&);
	QString toString() const;
#if USE_CGAL
	CGAL::AffTransformation3 getTransform() const;
#else
	const QGenericMatrix<N,M,decimal>& getValues() const;
#endif
	void setType(const TransformType& value);

private:
	QGenericMatrix<N,M,decimal> matrix;
	TransformType type;
};

#endif // TRANSFORMMATRIX_H
