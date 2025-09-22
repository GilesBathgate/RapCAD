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
#ifndef APPLICATION_H
#define APPLICATION_H

#include "reporter.h"
#include "strategy.h"
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QCoreApplication>

class Application
{
	Q_DISABLE_COPY_MOVE(Application)
public:
	Application();
	int run(int argc,char* argv[]);
	~Application();
	static QCoreApplication* headlessOverride();
private:
	Strategy* parseArguments(int,char*[]);

	QStringList inputFiles;
	QTextStream output;
	QTextStream messages;
	Reporter reporter;
	Strategy* strategy;
	QFile* redirectFile;
};

#endif // APPLICATION_H
