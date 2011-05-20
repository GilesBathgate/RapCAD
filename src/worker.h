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

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTextStream>
#include "cgalprimitive.h"
#include "reporter.h"

class Worker : public QObject
{
	Q_OBJECT
public:
	Worker(QTextStream&,QObject* parent = 0);
	virtual void evaluate(QString path, bool print);
	virtual ~Worker();
signals:
	void done(CGALPrimitive*);
protected slots:
	void doWork();
protected:
	virtual void finish();
	QString path;
	bool print;
private:
	QTextStream& output;
	Reporter* reporter;
};

#endif // WORKER_H
