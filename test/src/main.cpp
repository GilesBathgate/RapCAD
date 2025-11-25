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

#include "tester.h"
#include "reporter.h"
#include <QCommandLineParser>

#ifdef Q_OS_WIN
static QStringList getArguments(int argc,char*[])
{
	QStringList list;
	if(wchar_t** argv=CommandLineToArgvW(GetCommandLineW(),&argc)) {
		for(int a=0; a<argc; ++a) {
			list << QString::fromWCharArray(argv[a]);
		}
		LocalFree(argv);
	}
	return list;
}
#else
static QStringList getArguments(int argc,char* argv[])
{
	QStringList list;
	for(int a=0; a<argc; ++a) {
		list << QString::fromLocal8Bit(argv[a]);
	}
	return list;
}
#endif

int main(int argc,char* argv[])
{
	const auto arguments=getArguments(argc,argv);
	QCommandLineParser p;
	const QCommandLineOption testOption(QStringList() << "t" << "test", "Run through tests in given directory.","directory");
	p.addOption(testOption);
	p.parse(arguments);

	if(!p.isSet(testOption)) return EXIT_FAILURE;

	QTextStream output(stdout);
	Reporter reporter(output);
	Tester tester(reporter,p.value(testOption));
	return tester.evaluate();
}
