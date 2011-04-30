/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#ifndef BACKGROUNDWORKER_H
#define BACKGROUNDWORKER_H

#include <QObject>
#include <QTextStream>
#include "cgalprimitive.h"

class BackgroundWorker : public QObject
{
	Q_OBJECT
public:
	BackgroundWorker(bool,QTextStream&,QObject* parent = 0);
	void evaluate(QString path, bool print, QString format);
signals:
	void done(CGALPrimitive*);
public slots:
	void doWork();
private:
	QTextStream& output;
	QThread* thread;
	QString path;
	bool print;
	QString format;
	bool background;
};

#endif // BACKGROUNDWORKER_H
