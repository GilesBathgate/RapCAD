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

#ifndef BACKGROUNDWORKER_H
#define BACKGROUNDWORKER_H

#include <QObject>
#include <QThread>
#include <QTextStream>
#include "worker.h"

class BackgroundWorker : public QObject, public Worker
{
	Q_OBJECT
public:
	BackgroundWorker(Reporter&, QObject* parent = nullptr);
	~BackgroundWorker() override;
	int evaluate() override;
signals:
	void done();
protected slots:
	void start();
private:
	void update() override;
	void finish() override;
	QThread* thread;
};

#endif // BACKGROUNDWORKER_H
