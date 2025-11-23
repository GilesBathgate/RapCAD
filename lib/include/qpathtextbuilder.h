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

#ifndef QPATHTEXTBUILDER_H
#define QPATHTEXTBUILDER_H

#include "textbuilder.h"
#include <QFont>
#include <QPointF>
#include <QString>

class QPathTextBuilder : public TextBuilder
{
	Q_DISABLE_COPY_MOVE(QPathTextBuilder)
public:
	QPathTextBuilder();
	~QPathTextBuilder() override = default;
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
	QPointF location;
};

#endif // QPATHTEXTBUILDER_H
