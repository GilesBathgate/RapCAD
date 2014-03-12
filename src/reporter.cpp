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

#include "reporter.h"

Reporter::Reporter(QTextStream& s) : output(s)
{
	kludge=0;
	returnCode=EXIT_FAILURE;
}

void Reporter::startTiming()
{
	timer=new QElapsedTimer();
	timer->start();
}

void Reporter::reportTiming(QString what)
{
	qint64 ticks=timer->elapsed();
	qint64 ms=ticks%1000;
	ticks/=1000;
	qint64 secs=ticks%60;
	ticks/=60;
	qint64 mins=ticks%60;
	ticks/=60;
	qint64 hours=ticks;
	output << QString("Total %1 time: %2h %3m %4s %5ms.\n").arg(what).arg(hours).arg(mins).arg(secs).arg(ms) << endl;
	delete timer; //Need to delete timer.
}

void Reporter::reportSyntaxError(AbstractTokenBuilder* t, QString msg, QString text)
{
	int pos=t->getPosition()+kludge;
	int line=t->getLineNumber();
	output << "line " << line << ": " << msg << " at character " << pos << ": '" << text << "'.\n";
}

void Reporter::reportLexicalError(AbstractTokenBuilder* t, QString text)
{
	int pos=t->getPosition()+kludge;
	int line=t->getLineNumber();
	output << "Line " << line << ": illegal token at character " << pos << ": '" << text << "'.\n";
}

void Reporter::reportFileMissingError(QString fullpath)
{
	output << "Can't open input file '" << fullpath << "'\n";
}

void Reporter::setReturnCode(int code)
{
	returnCode=code;
}

bool Reporter::getReturnCode()
{
	return returnCode;
}

void Reporter::setKludge(int k)
{
	kludge=k;
}
