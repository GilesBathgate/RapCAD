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
#ifdef USE_CGAL
#ifndef CGALIMPORT_H
#define CGALIMPORT_H

#include "primitive.h"
#include "reporter.h"
#include <QFileInfo>

class CGALImport
{
	Q_DECLARE_TR_FUNCTIONS(CGALImport)
public:
	explicit CGALImport(const QFileInfo&,Reporter&);
	Primitive* import() const;
private:
	Primitive* importOFF() const;
	Primitive* importOBJ() const;
	Primitive* importSTL() const;
	Primitive* importRCAD() const;
	Primitive* import3MF() const;
	Primitive* importAMF() const;
	Primitive* importNEF() const;

	const QFileInfo& fileInfo;
	Reporter& reporter;

};

#endif // CGALIMPORT_H
#endif
