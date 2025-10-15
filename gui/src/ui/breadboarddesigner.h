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

namespace Ui
{
class BreadboardDesigner;
} // namespace Ui

struct Hole {
	QString band;
	int r, c;
	int x, y;
};

struct Connection {
	QString id;
	QString a, b;
	QColor color;
	std::vector<QPoint> waypoints;
};

struct Component {
	QString id;
	std::vector<QString> pins;
};

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
	enum CreationMode { NotCreating, CreatingTwoPin, CreatingMultiPin };

	Ui::BreadboardDesigner* ui;
	std::vector<std::vector<Hole>> buildHoles();
	QString holeId(const Hole& h) const;
	Hole* findHole(const QString& id);
	bool isHoleOccupied(const QString& id) const;
	bool sameGroup(const Hole& h1, const Hole& h2) const;
	QPainterPath makePath(const QPointF& a, const QPointF& b) const;
	QPainterPath makeWaypointPath(const QPointF& a, const QPointF& b, const std::vector<QPoint>& waypoints) const;
	bool isPositiveHole(const Hole& h) const;
	bool isNegativeHole(const Hole& h);
	void handleComponentCreationClick(Hole& clickedHole, Qt::KeyboardModifiers modifiers);
	void handleStandardClick(Hole& clickedHole);
	void cancelCreation();


private slots:
	void toggleFlashColor();

private:
	std::vector<QPoint> temporaryWaypoints;
	std::vector<std::vector<Hole>> holes;
	std::vector<Connection> connections;
	std::vector<Component> components;
	CreationMode creationMode = NotCreating;
	std::vector<Hole*> pinsForNewComponent;
	Hole* firstHole = nullptr;
	Hole* hoverHole = nullptr;
	QPoint mousePos;
	QString movingConnectionId;
	QString movingComponentId;
	QString originalHoleId;
	QColor temporaryWireColor = QColorConstants::White;
	QTimer* flashTimer;
	bool flashState = false;
	int columns = 30;
	int holeSize = 12;
	int holeGap = 18;
};

#endif // BREADBOARDDESIGNER_H
