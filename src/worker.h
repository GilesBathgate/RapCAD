/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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
	Q_DECLARE_TR_FUNCTIONS(Worker)
public:
	explicit Worker(Reporter&);
	~Worker() override;
	void setup(const QString&,const QString&,bool);
	int evaluate() override;
	void exportResult(const QString&);
	bool resultAvailable();
	void resultAccepted();
	Renderer* getRenderer();
protected:
	virtual void update() {}
	virtual void finish() {}
private:
	Instance* addProductInstance(const QString&, Script&);
	static QList<Argument*> getArgs(const decimal&);
	decimal getBoundsHeight() const;
	void generation();
	void primary();
	void resultFailed(const QString&);
	void updatePrimitive(Primitive*);

	Primitive* primitive;
	Primitive* previous;
	QFileInfo inputFile;
	QString outputFile;
	bool generate;
};

#endif // WORKER_H
