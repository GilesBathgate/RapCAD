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

#include "texteditiodevice.h"

TextEditIODevice::TextEditIODevice(QTextEdit* textEdit,QObject* parent) :
	QIODevice(parent),
	textEdit(textEdit)
{
	open(QIODevice::WriteOnly|QIODevice::Text);
	connect(this,&TextEditIODevice::textRecieved,this,&TextEditIODevice::writeTextEdit);
}

qint64 TextEditIODevice::readData(char*,qint64)
{
	return 0;
}

qint64 TextEditIODevice::writeData(const char* data, qint64 maxSize)
{
	emit textRecieved(data);
	return maxSize;
}

void TextEditIODevice::writeTextEdit(const QString& data)
{
	textEdit->moveCursor(QTextCursor::End);
	textEdit->insertPlainText(data);
}
