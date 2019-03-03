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

#ifndef QPATHTEXTBUILDER_H
#define QPATHTEXTBUILDER_H

#include <QString>
#include <QFont>
#include <QPointF>
#include "textbuilder.h"
#include "reporter.h"

class QPathTextBuilder : public TextBuilder
{
public:
	explicit QPathTextBuilder(Reporter&);
	~QPathTextBuilder() override;
	void setText(const QString&) override;
	void setFamily(const QString& value);
	void setSize(int value);
	decimal getHeight() override;
	void setLocation(const Point&) override;
	Primitive* buildPrimitive() const override;
private:
	QFont getFont() const;
	QString text;
	QString family;
	int size;
	bool headless;
	QPointF location;
	Reporter& reporter;
};

#endif // QPATHTEXTBUILDER_H
