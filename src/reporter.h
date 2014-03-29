/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

#ifndef REPORTER_H
#define REPORTER_H

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QTextStream>
#include "abstracttokenbuilder.h"

class Reporter
{
	Q_DECLARE_TR_FUNCTIONS(Reporter)
public:
	Reporter(QTextStream& s);
	void startTiming();
	void reportTiming(QString);
	void reportSyntaxError(AbstractTokenBuilder*,QString,QString);
	void reportLexicalError(AbstractTokenBuilder*,QString);
	void reportFileMissingError(QString);
	void reportWarning(QString);
	void reportMessage(QString);
	void reportException(QString);
	void setReturnCode(int);
	bool getReturnCode();
	void outputTime(int);
	void setKludge(int);
	QTextStream& output;
private:
	QElapsedTimer* timer;
	int returnCode;
	int kludge;
};

#endif // REPORTER_H
