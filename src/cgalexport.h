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
#ifdef USE_CGAL
#ifndef CGALEXPORT_H
#define CGALEXPORT_H

#include <QString>
#include <QXmlStreamWriter>
#include "cgalprimitive.h"
#include "reporter.h"

class CGALExport
{
	Q_DECLARE_TR_FUNCTIONS(CGALExport)
public:
	CGALExport(Primitive*,Reporter&);
	void exportResult(QString);

private:
	void exportOFF(QString);
	void exportAsciiSTL(QString);
	void exportVRML(QString filename);
	void exportOBJ(QString);
	void exportAMF(QString);
	void export3MF(QString);
	void exportCSG(QString);
	void exportNEF(QString);
	void exportSVG(QString);
	Reporter& reporter;
	Primitive* primitive;
	void exportAMFObject(CGALPrimitive* p, QXmlStreamWriter& xml);
	void descendChildren(Primitive* p, QXmlStreamWriter& xml);
	int id;
};

#endif // CGALEXPORT_H
#endif
