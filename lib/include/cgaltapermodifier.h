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
#ifndef CGALTAPERMODIFIER_H
#define CGALTAPERMODIFIER_H

#include "cgal.h"
#include <CGAL/Modifier_base.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <QMap>
#include <QtGlobal>

namespace CGAL
{
using NefPolyhedron3 = Nef_polyhedron_3<Kernel3>;
} // namespace CGAL

class CGALTaperModifier final : public CGAL::Modifier_base<CGAL::NefPolyhedron3::SNC_structure>
{
public:
	Q_DISABLE_COPY_MOVE(CGALTaperModifier)
	explicit CGALTaperModifier(const QMap<int, CGAL::Vector3>&);
	~CGALTaperModifier() override=default;
	void operator()(CGAL::NefPolyhedron3::SNC_structure&) override;
private:
	const QMap<int, CGAL::Vector3>& movements;
};

#endif // CGALTAPERMODIFIER_H
#endif
