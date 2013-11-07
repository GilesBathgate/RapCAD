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

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTextStream>
#include "primitive.h"
#include "renderer.h"
#include "reporter.h"
#include "renderer.h"

class Worker : public QObject
{
	Q_OBJECT
public:
	Worker(QTextStream&,QObject* parent = 0);
	void setup(QString,QString,bool);
	virtual void evaluate();
	void exportResult(QString);
	bool resultAvailable();
	Renderer* getRenderer();
	virtual ~Worker();
signals:
	void done();
protected slots:
	void doWork();
protected:
	virtual void finish();
	QString inputFile;
	QString outputFile;
	bool print;
private:
	QTextStream& output;
	Reporter* reporter;
	Primitive* primitive;
	Renderer* render;
};

#endif // WORKER_H
