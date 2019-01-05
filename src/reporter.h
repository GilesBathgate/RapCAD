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
	explicit Reporter(QTextStream& s);
	void startTiming();
	void reportTiming(const QString&);
	void reportSyntaxError(const AbstractTokenBuilder&, const QString&);
	void reportLexicalError(const AbstractTokenBuilder&,const QString&);
	void reportFileMissingError(const QString&);
	void reportWarning(const QString&);
	void reportMessage(const QString&);
	void reportException(const QString&);
	void setReturnCode(int);
	bool getReturnCode() const;
	void setKludge(int);
	QTextStream& output;
private:
	QElapsedTimer* timer;
	int returnCode;
	int kludge;
};

#endif // REPORTER_H
