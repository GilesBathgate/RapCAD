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

#ifndef TEXTITERATOR_H
#define TEXTITERATOR_H

#include "valueiterator.h"
#include "value.h"

class TextIterator : public ValueIterator
{
public:
	explicit TextIterator(const QString&);
	ValueIterator& operator++() override;
	bool operator!=(const Iterator&) const override;
	Value* operator*() const override;
private:
	QString text;
	int index;
};

#endif // TEXTITERATOR_H
