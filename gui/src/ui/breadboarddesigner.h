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
#ifndef BREADBOARDDESIGNER_H
#define BREADBOARDDESIGNER_H

#include <QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include <QPainterPath>
#include <QTimer>
#include <QColor>

#include "breadboardmodel.h"
#include "breadboardcontroller.h"

namespace Ui
{
class BreadboardDesigner;
} // namespace Ui

class BreadboardDesigner : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(BreadboardDesigner)

public:
	explicit BreadboardDesigner(QWidget* parent = nullptr);
	~BreadboardDesigner() override;

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private slots:
	void clearConnections();

private:
	QPainterPath makePath(const QPointF& a, const QPointF& b) const;
	QPainterPath makeWaypointPath(const QPointF& a, const QPointF& b, const QVector<QPoint>& waypoints) const;

private:
	Ui::BreadboardDesigner* ui;
	BreadboardModel* m_model;
	BreadboardController* m_controller;
};

#endif // BREADBOARDDESIGNER_H
