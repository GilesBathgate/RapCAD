/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2020 Giles Bathgate
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
	CGALExport(const QFileInfo&,Primitive*,Reporter&);
	void exportResult() const;
private:
	void exportOFF() const;
	void exportAsciiSTL() const;
	void exportVRML() const;
	void exportOBJ() const;
	void exportAMF() const;
	void export3MF() const;
	void exportCSG() const;
	void exportNEF() const;
	void exportSVG() const;
	void exportAMFObject(CGALPrimitive* p, QXmlStreamWriter& xml) const;
	void descendChildren(Primitive* p, QXmlStreamWriter& xml) const;

	Reporter& reporter;
	Primitive* primitive;
	const QFileInfo& fileInfo;
	mutable int id;
};

#endif // CGALEXPORT_H
#endif
