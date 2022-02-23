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

#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
#include <QColor>
namespace QColorConstants {
namespace svg {
constexpr Q_DECL_UNUSED QColor dodgerblue  {QColor::Rgb, 0xff * 0x101, 0x1e * 0x101, 0x90 * 0x101, 0xff * 0x101};
constexpr Q_DECL_UNUSED QColor palegreen   {QColor::Rgb, 0xff * 0x101, 0x98 * 0x101, 0xfb * 0x101, 0x98 * 0x101};
constexpr Q_DECL_UNUSED QColor plum        {QColor::Rgb, 0xff * 0x101, 0xdd * 0x101, 0xa0 * 0x101, 0xdd * 0x101};
constexpr Q_DECL_UNUSED QColor orange      {QColor::Rgb, 0xff * 0x101, 0xff * 0x101, 0xa5 * 0x101, 0x00 * 0x101};
constexpr Q_DECL_UNUSED QColor springgreen {QColor::Rgb, 0xff * 0x101, 0x00 * 0x101, 0xff * 0x101, 0x7f * 0x101};
constexpr Q_DECL_UNUSED QColor skyblue     {QColor::Rgb, 0xff * 0x101, 0x87 * 0x101, 0xce * 0x101, 0xeb * 0x101};
constexpr Q_DECL_UNUSED QColor deepskyblue {QColor::Rgb, 0xff * 0x101, 0x00 * 0x101, 0xbf * 0x101, 0xff * 0x101};
}
}
#endif

#endif // QTCOMPAT_H
