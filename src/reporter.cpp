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
#include "reporter.h"
#include <contrib/qtcompat.h>

Reporter::Reporter(QTextStream& s) :
	Reporter(s,s)
{
}

Reporter::Reporter(QTextStream& o, QTextStream& s):
	output(o),
	messages(s),
	timer(nullptr),
	returnCode(EXIT_FAILURE),
	positionOffset(0)
{
}

void Reporter::startTiming()
{
	timer=new QElapsedTimer();
	timer->start();
}

void Reporter::stopTiming(const QString& what)
{
	qint64 ticks=timer->elapsed();
	const qint64 ms=ticks%1000;
	ticks/=1000;
	const qint64 secs=ticks%60;
	ticks/=60;
	const qint64 mins=ticks%60;
	ticks/=60;
	const qint64 hours=ticks;
	timings << tr("Total %1 time: %2h %3m %4s %5ms.").arg(what).arg(hours).arg(mins).arg(secs).arg(ms);
	delete timer; //Need to delete timer.
}

void Reporter::reportTiming(const QString& what)
{
	stopTiming(what);
	reportTimings();
}

void Reporter::reportTimings()
{
	for(auto& m: qAsConst(timings))
		messages << m << Qt::endl;
	timings.clear();
}

void Reporter::reportSyntaxError(const AbstractTokenBuilder& t,const QString& msg)
{
	const QString& text=t.getToken();
	const int pos=t.getPosition()+positionOffset;
	const int line=t.getLineNumber();
	messages << tr("Line %1: %2 at character %3: '%4'").arg(line).arg(msg).arg(pos).arg(text) << Qt::endl;
}

void Reporter::reportLexicalError(const AbstractTokenBuilder& t,const QString& text)
{
	const int pos=t.getPosition()+positionOffset;
	const int line=t.getLineNumber();
	messages << tr("Line %1: illegal token at character %2: '%3'").arg(line).arg(pos).arg(text) << Qt::endl;
}

void Reporter::reportFileMissingError(const QString& fullpath)
{
	messages << tr("Can't open input file '%1'").arg(fullpath) << Qt::endl;
}

void Reporter::reportTesselationError(const QString& text)
{
	messages << tr("Tessellation Error: %1").arg(text) << Qt::endl;
}

void Reporter::reportWarning(const QString& warning)
{
	messages << tr("Warning: %1").arg(warning) << Qt::endl;
}

void Reporter::reportMessage(const QString& msg)
{
	messages << msg << Qt::endl;
}

void Reporter::reportException()
{
	reportException(tr("Unknown error."));
}

void Reporter::reportException(const QString& ex)
{
	messages << tr("Exception: %1").arg(ex) << Qt::endl;
}

void Reporter::setReturnCode(int code)
{
	returnCode=code;
}

bool Reporter::getReturnCode() const
{
	return returnCode;
}

void Reporter::setPositionOffset(int o)
{
	positionOffset=o;
}
