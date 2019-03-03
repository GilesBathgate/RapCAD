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

#ifndef COMPARER_H
#define COMPARER_H

#include "strategy.h"

class Comparer : public Strategy
{
	Q_DECLARE_TR_FUNCTIONS(Comparer)
public:
	explicit Comparer(Reporter&);
	void setup(const QString&,const QString&);
	int evaluate() override;
private:
	QString aFile;
	QString bFile;
};

#endif // COMPARER_H
