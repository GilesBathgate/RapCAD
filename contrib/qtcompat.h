/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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
#ifndef QTCOMPAT_H
#define QTCOMPAT_H

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
#include <QTextStream>
namespace Qt
{
inline QTextStream& endl(QTextStream& s)
{
	return ::endl(s);
}
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5,12,0)
#include <QColor>
namespace QColorConstants {
namespace Svg {
Q_DECL_UNUSED QColor dodgerblue  {0x1e,0x90,0xff};
Q_DECL_UNUSED QColor palegreen   {0x98,0xfb,0x98};
Q_DECL_UNUSED QColor plum        {0xdd,0xa0,0xdd};
Q_DECL_UNUSED QColor orange      {0xff,0xa5,0x00};
Q_DECL_UNUSED QColor springgreen {0x00,0xff,0x7f};
Q_DECL_UNUSED QColor skyblue     {0x87,0xce,0xeb};
Q_DECL_UNUSED QColor deepskyblue {0x00,0xbf,0xff};
}
}
#endif

#endif // QTCOMPAT_H
