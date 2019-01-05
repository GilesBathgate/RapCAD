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

#ifndef CENTERNODE_H
#define CENTERNODE_H

#include "node.h"
#include "point.h"

class AlignNode : public Node
{
public:
	enum Face_t {
		Top,
		Bottom,
		North,
		South,
		East,
		West
	};

	AlignNode();

	bool getCenter() const;
	void setCenter(bool value);
	void setCenterVertical();

	QList<Face_t> getAlign() const;
	void setAlign(const QList<Face_t>& value);

	void accept(NodeVisitor&) override;
private:
	QList<Face_t> align;
	bool center;
};

#endif // CENTERNODE_H
