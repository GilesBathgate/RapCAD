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

#include "reporter.h"

Reporter::Reporter(QTextStream& s) :
	output(s),
	timer(nullptr),
	returnCode(EXIT_FAILURE),
	kludge(0)
{
}

void Reporter::startTiming()
{
	timer=new QElapsedTimer();
	timer->start();
}

void Reporter::reportTiming(const QString& what)
{
	qint64 ticks=timer->elapsed();
	qint64 ms=ticks%1000;
	ticks/=1000;
	qint64 secs=ticks%60;
	ticks/=60;
	qint64 mins=ticks%60;
	ticks/=60;
	qint64 hours=ticks;
	output << tr("Total %1 time: %2h %3m %4s %5ms.").arg(what).arg(hours).arg(mins).arg(secs).arg(ms) << endl;
	delete timer; //Need to delete timer.
}

void Reporter::reportSyntaxError(const AbstractTokenBuilder& t,const QString& msg)
{
	QString text=t.getToken();
	int pos=t.getPosition()+kludge;
	int line=t.getLineNumber();
	output << tr("Line %1: %2 at character %3: '%4'").arg(line).arg(msg).arg(pos).arg(text) << endl;
}

void Reporter::reportLexicalError(const AbstractTokenBuilder& t,const QString& text)
{
	int pos=t.getPosition()+kludge;
	int line=t.getLineNumber();
	output << tr("Line %1: illegal token at character %2: '%3'").arg(line).arg(pos).arg(text) << endl;
}

void Reporter::reportFileMissingError(const QString& fullpath)
{
	output << tr("Can't open input file '%1'").arg(fullpath) << endl;
}

void Reporter::reportWarning(const QString& warning)
{
	output << tr("Warning: %1").arg(warning) << endl;
}

void Reporter::reportMessage(const QString& msg)
{
	output << msg << endl;
}

void Reporter::reportException(const QString& ex)
{
	output << tr("Exception: %1").arg(ex) << endl;
}

void Reporter::setReturnCode(int code)
{
	returnCode=code;
}

bool Reporter::getReturnCode() const
{
	return returnCode;
}

void Reporter::setKludge(int k)
{
	kludge=k;
}
