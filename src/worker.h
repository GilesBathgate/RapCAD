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

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTextStream>
#include "strategy.h"
#include "primitive.h"
#include "renderer.h"
#include "reporter.h"
#include "script.h"
#include "callback.h"
#include "instance.h"
#include "renderer.h"

class Worker : public Strategy
{
public:
	Worker(QTextStream&);
	void setup(QString,QString,bool,bool);
	virtual int evaluate();
	void exportResult(QString);
	bool resultAvailable();
	Renderer* getRenderer();
protected:
	void internal();
	virtual void update() {}
	virtual void finish() {}
	Instance* addProductInstance(QString,Script*);
	QString inputFile;
	QString outputFile;
	bool print;
	bool generate;
private:
	QList<Argument*> getArgs(double);
	void primary();
	void generation();
	double getBoundsHeight();
	Primitive* primitive;
	Renderer* render;
};

#endif // WORKER_H
