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
#ifndef PRINTCONSOLE_H
#define PRINTCONSOLE_H

#include <QDialog>

namespace Ui
{
class PrintConsole;
} // namespace Ui

class PrintConsole : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(PrintConsole)
public:
	explicit PrintConsole(QWidget* parent=nullptr);
	~PrintConsole() override;
private:
	static QString getStyleString(const QString&,const QString&);
	Ui::PrintConsole* ui;
};

#endif // PRINTCONSOLE_H
