/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2013 Giles Bathgate
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
}

void Reporter::startTiming()
{
	timer=new QTime();
	timer->start();
}

void Reporter::reportTiming()
{
	int ticks=timer->elapsed();
	int ms=ticks%1000;
	ticks/=1000;
	int secs=ticks%60;
	ticks/=60;
	int mins=ticks%60;
	ticks/=60;
	int hours=ticks;
	output << QString("Total compiling time: %1h %2m %3s %4ms.\n").arg(hours).arg(mins).arg(secs).arg(ms);
	output.flush();
	delete timer; //Need to delete timer.
}

void Reporter::reportSyntaxError(AbstractTokenBuilder* t, QString msg, QString text)
{
	int pos=t->getPosition();
	int line=t->getLineNumber();
	output << "line " << line << ": " << msg << " at character " << pos << ": '" << text << "'.\n";
}

void Reporter::reportLexicalError(AbstractTokenBuilder* t, QString text)
{
	int line=t->getLineNumber();
	output << line << ": illegal token '" << text << "'.\n";
}

void Reporter::reportFileMissingError(QString fullpath)
{
	output << "Can't open input file '" << fullpath << "'\n";
}
