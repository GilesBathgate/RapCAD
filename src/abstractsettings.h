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
#ifndef ABSTRACTSETTINGS_H
#define ABSTRACTSETTINGS_H

#include <QString>
#include <QVariant>

class AbstractSettings
{
	Q_DISABLE_COPY_MOVE(AbstractSettings)
protected:
	AbstractSettings()=default;
public:
	virtual ~AbstractSettings()=default;
	virtual void sync()=0;
	virtual QVariant value(QString,QVariant) const=0;
	virtual void setValue(QString,QVariant)=0;
	virtual void clear()=0;
};

#endif // ABSTRACTSETTINGS_H
