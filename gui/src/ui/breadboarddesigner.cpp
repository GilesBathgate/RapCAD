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

#include "breadboarddesigner.h"
#include "ui_breadboarddesigner.h"
#include <QUuid>

BreadboardDesigner::BreadboardDesigner(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::BreadboardDesigner)
{
	ui->setupUi(this);
	setMouseTracking(true);
	holes = buildHoles();
	flashTimer = new QTimer(this);
	connect(flashTimer, &QTimer::timeout, this, &BreadboardDesigner::toggleFlashColor);
	connect(ui->actionNew, &QAction::triggered, this, &BreadboardDesigner::clearConnections);
}

BreadboardDesigner::~BreadboardDesigner()
{
	delete ui;
}

std::vector<std::vector<Hole>> BreadboardDesigner::buildHoles()
{
	std::vector<std::vector<Hole>> rows;
	int startX = 60;
	int startY = 60;
	int y = startY;
	int gapBetweenBlocks = 5 * holeGap;

	auto makeRow = [&](const QString& band, int numRows) {
		for(int r = 0; r < numRows; ++r) {
			std::vector<Hole> row;
			int x = startX;
			for(int c = 0; c < columns; ++c) {
				row.push_back({band, r, c, x, y});
				x += holeGap;
			}
			rows.push_back(row);
			y += holeGap;
		}
	};

	makeRow("power-top", 2);
	y += holeGap * 2;
	makeRow("term-top", 5);
	y += gapBetweenBlocks;
	makeRow("term-bottom", 5);
	y += holeGap * 2;
	makeRow("power-bottom", 2);

	return rows;
}

QString BreadboardDesigner::holeId(const Hole& h) const
{
	return QString("%1:%2:%3").arg(h.band).arg(h.r).arg(h.c);
}

Hole* BreadboardDesigner::findHole(const QString& id)
{
	for(auto& row : holes) {
		for(auto& h : row) {
			if(holeId(h) == id) {
				return &h;
			}
		}
	}
	return nullptr;
}

bool BreadboardDesigner::isHoleOccupied(const QString& id) const
{
	for(const auto& c : connections) {
		if((c.a == id || c.b == id) && c.id != movingConnectionId) {
			return true;
		}
	}
	for(const auto& c : components) {
		if(c.id != movingComponentId) {
			for(const auto& p : c.pins) {
				if(p == id) {
					return true;
				}
			}
		}
	}
	return false;
}

bool BreadboardDesigner::sameGroup(const Hole& h1, const Hole& h2) const
{
	if(h1.band.contains("power") && h1.band == h2.band && h1.r == h2.r) return true;
	if(h1.band.contains("term") && h1.band == h2.band && h1.c == h2.c) return true;
	return false;
}

bool BreadboardDesigner::isPositiveHole(const Hole& h) const
{
	return (h.band == "power-top" && h.r == 0) || (h.band == "power-bottom" && h.r == 1);
}

bool BreadboardDesigner::isNegativeHole(const Hole& h)
{
	return (h.band == "power-top" && h.r == 1) || (h.band == "power-bottom" && h.r == 0);
}

void BreadboardDesigner::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	auto drawComponent = [&](QPointF start, QPointF end) {
		painter.drawLine(start, end);
		if(start.x() == end.x()) {  // vertical
			int boxHeight = abs(start.y() - end.y()) - holeSize * 2;
			painter.drawRect(start.x() - holeSize/2, std::min(start.y(), end.y()) + holeSize, holeSize, boxHeight);
		} else { // horizontal
			int boxWidth = abs(start.x() - end.x()) - holeSize * 2;
			painter.drawRect(std::min(start.x(), end.x()) + holeSize, start.y() - holeSize/2, boxWidth, holeSize);
		}
	};

	// Draw holes
	for(const auto& row : holes) {
		for(const auto& h : row) {
			bool sameGroupHover = hoverHole && sameGroup(*hoverHole, h);
			QColor fillColor = sameGroupHover ? QColor("#bfdbfe") : (hoverHole && holeId(*hoverHole) == holeId(h)) ? QColor("#fde68a") : QColorConstants::Gray;
			painter.setBrush(fillColor);
			painter.setPen(QColor(0,0,0,76));
			painter.drawEllipse(QPoint(h.x, h.y), holeSize / 2, holeSize / 2);
		}
	}

	// Draw polarity lines
	if(holes.size() >= 14) {
		auto drawLine = [&](int rowIndex, const QColor& color, int y_offset) {
			const auto& row = holes[rowIndex];
			if(!row.empty()) {
				QPen pen(color);
				pen.setWidth(1);
				painter.setPen(pen);
				int y = row[0].y + y_offset;
				painter.drawLine(row[0].x, y, row.back().x, y);
			}
		};

		int offset = (holeGap / 2) + 2;
		drawLine(0, Qt::red, -offset);    // Top positive
		drawLine(1, Qt::black, offset);   // Top negative
		drawLine(12, Qt::black, -offset); // Bottom negative
		drawLine(13, Qt::red, offset);    // Bottom positive
	}

	// Draw wires
	for(const auto& c : connections) {
		if(!movingConnectionId.isNull() && c.id == movingConnectionId) continue;
		Hole* a = findHole(c.a);
		Hole* b = findHole(c.b);
		if(a && b) {
			QPen pen(c.color);
			pen.setWidth(3);
			pen.setCapStyle(Qt::RoundCap);
			painter.setPen(pen);
			painter.setBrush(Qt::NoBrush);
			if(c.waypoints.empty()) {
				painter.drawPath(makePath(QPointF(a->x, a->y), QPointF(b->x, b->y)));
			} else {
				painter.drawPath(makeWaypointPath(QPointF(a->x, a->y), QPointF(b->x, b->y), c.waypoints));
			}
		}
	}

	// Draw components
	for(const auto& c : components) {
		if(!movingComponentId.isNull() && c.id == movingComponentId) continue;
		if(c.pins.empty()) continue;

		painter.setPen(QPen(QColorConstants::White, 1));
		painter.setBrush(QColorConstants::White);

		if(c.pins.size() >= 2) {
			if(c.pins.size() == 2) {  // Draw as a resistor
				Hole* a = findHole(c.pins[0]);
				Hole* b = findHole(c.pins[1]);
				if(a && b) {
					if(a->r == b->r || a->c == b->c) {
						drawComponent(QPointF(a->x, a->y), QPointF(b->x, b->y));
					}
				}
			} else { // Draw as a bounding box
				int minX = -1, minY = -1, maxX = -1, maxY = -1;
				for(const auto& pinId : c.pins) {
					Hole* h = findHole(pinId);
					if(h) {
						if(minX == -1 || h->x < minX) minX = h->x;
						if(minY == -1 || h->y < minY) minY = h->y;
						if(maxX == -1 || h->x > maxX) maxX = h->x;
						if(maxY == -1 || h->y > maxY) maxY = h->y;
					}
				}
				if(minX != -1) {
					painter.drawRect(QRect(QPoint(minX - holeSize/2, minY - holeSize/2), QPoint(maxX + holeSize/2, maxY + holeSize/2)));
					// Also draw the pins
					painter.setBrush(QColorConstants::Gray);
					for(const auto& pinId : c.pins) {
						Hole* h = findHole(pinId);
						if(h) {
							painter.drawEllipse(QPoint(h->x, h->y), holeSize / 2, holeSize / 2);
						}
					}
				}
			}
		}
	}

	// Draw temporary component placement
	if(creationMode != NotCreating && !pinsForNewComponent.empty()) {
		painter.setPen(QPen(QColorConstants::White, 1));
		painter.setBrush(QColor(128, 128, 128, 128));
		painter.setOpacity(0.8);

		if(creationMode == CreatingTwoPin || (creationMode == CreatingMultiPin && pinsForNewComponent.size() == 1)) {
			QPointF start(pinsForNewComponent[0]->x, pinsForNewComponent[0]->y);
			QPointF end = mousePos;
			if(abs(start.x() - end.x()) > abs(start.y() - end.y())) {
				end.setY(start.y());
			} else {
				end.setX(start.x());
			}
			drawComponent(start, end);
		} else if(creationMode == CreatingMultiPin && pinsForNewComponent.size() > 1) {
			int minX = pinsForNewComponent[0]->x;
			int minY = pinsForNewComponent[0]->y;
			int maxX = pinsForNewComponent[0]->x;
			int maxY = pinsForNewComponent[0]->y;

			for(size_t i = 1; i < pinsForNewComponent.size(); ++i) {
				minX = std::min(minX, pinsForNewComponent[i]->x);
				minY = std::min(minY, pinsForNewComponent[i]->y);
				maxX = std::max(maxX, pinsForNewComponent[i]->x);
				maxY = std::max(maxY, pinsForNewComponent[i]->y);
			}

			minX = std::min(minX, (int)mousePos.x());
			minY = std::min(minY, (int)mousePos.y());
			maxX = std::max(maxX, (int)mousePos.x());
			maxY = std::max(maxY, (int)mousePos.y());

			int inflation = 5;
			painter.drawRect(QRect(QPoint(minX - inflation, minY - inflation), QPoint(maxX + inflation, maxY + inflation)));
		}
	}

	if(firstHole) {
		if(movingComponentId.isNull()) {  // Drawing a temporary wire
			QPen pen(temporaryWireColor);
			pen.setWidth(3);
			pen.setCapStyle(Qt::RoundCap);
			painter.setPen(pen);
			painter.setBrush(Qt::NoBrush);
			painter.setOpacity(0.8);

			if(temporaryWaypoints.empty()) {
				painter.drawPath(makePath(QPointF(firstHole->x, firstHole->y), mousePos));
			} else {
				painter.drawPath(makeWaypointPath(QPointF(firstHole->x, firstHole->y), mousePos, temporaryWaypoints));
			}
		} else { // Drawing a temporary component (while moving)
			painter.setPen(QPen(QColorConstants::White, 1));
			painter.setBrush(QColorConstants::White);
			painter.setOpacity(0.8);
			QPointF start(firstHole->x, firstHole->y);
			QPointF end = mousePos;
			if(abs(start.x() - end.x()) > abs(start.y() - end.y())) {
				end.setY(start.y());
			} else {
				end.setX(start.x());
			}
			drawComponent(start, end);
		}
	}
}

void BreadboardDesigner::mousePressEvent(QMouseEvent* event)
{
	if(event->button() == Qt::RightButton) {
		cancelCreation();
		update();
		return;
	}

	Qt::KeyboardModifiers modifiers = event->modifiers();
	if(firstHole && modifiers.testFlag(Qt::ControlModifier)) {
		temporaryWaypoints.push_back(event->pos());
		update();
		return;
	}

	Hole* clickedHole = nullptr;
	for(auto& row : holes) {
		for(auto& h : row) {
			if(QVector2D(event->pos() - QPoint(h.x, h.y)).length() < holeSize / 2.0) {
				clickedHole = &h;
				break;
			}
		}
		if(clickedHole) break;
	}

	if(!clickedHole) {
		cancelCreation();
		update();
		return;
	}

	if(modifiers.testFlag(Qt::ControlModifier) && !firstHole) {
		handleComponentCreationClick(*clickedHole, modifiers);
	} else {
		handleStandardClick(*clickedHole);
	}

	update();
}

void BreadboardDesigner::mouseMoveEvent(QMouseEvent* event)
{
	mousePos = event->pos();
	Hole* lastHoverHole = hoverHole;
	hoverHole = nullptr;
	for(auto& row : holes) {
		for(auto& h : row) {
			if(QVector2D(event->pos() - QPoint(h.x, h.y)).length() < holeSize / 2.0) {
				hoverHole = &h;
				break;
			}
		}
		if(hoverHole) break;
	}

	if(lastHoverHole != hoverHole || firstHole) {
		update();
	}

	if(firstHole) {
		bool isShort = hoverHole && ((isPositiveHole(*firstHole) && isNegativeHole(*hoverHole)) || (isNegativeHole(*firstHole) && isPositiveHole(*hoverHole)));
		if(isShort) {
			if(!flashTimer->isActive()) {
				flashTimer->start(100);
			}
		} else {
			flashTimer->stop();
			if(isPositiveHole(*firstHole) || (hoverHole && isPositiveHole(*hoverHole))) {
				temporaryWireColor = Qt::red;
			} else if(isNegativeHole(*firstHole) || (hoverHole && isNegativeHole(*hoverHole))) {
				temporaryWireColor = Qt::black;
			} else {
				temporaryWireColor = QColorConstants::White;
			}
		}
	} else {
		flashTimer->stop();
	}
}

void BreadboardDesigner::clearConnections()
{
	connections.clear();
	components.clear();
	update();
}

QPainterPath BreadboardDesigner::makePath(const QPointF& a, const QPointF& b) const
{
	QPainterPath path;
	QPointF c1(a.x() + (b.x() - a.x()) / 2, a.y());
	QPointF c2(a.x() + (b.x() - a.x()) / 2, b.y());
	path.moveTo(a);
	path.cubicTo(c1, c2, b);
	return path;
}

QPainterPath BreadboardDesigner::makeWaypointPath(const QPointF& a, const QPointF& b, const std::vector<QPoint>& waypoints) const
{
	QPainterPath path;
	std::vector<QPointF> points;
	points.push_back(a);
	for(const auto& wp : waypoints) {
		points.push_back(wp);
	}
	points.push_back(b);

	path.moveTo(points[0]);
	// For each waypoint, draw a line to a point before the waypoint, then draw a quadratic bezier curve to a point after the waypoint.
	// This creates a rounded corner at the waypoint.
	for(size_t i = 1; i < points.size() - 1; ++i) {
		QPointF p1 = points[i-1];
		QPointF p2 = points[i]; // The waypoint
		QPointF p3 = points[i+1];

		qreal rad = holeSize;

		// Calculate the point on the line from p1 to p2, just before p2
		QLineF line1(p2, p1);
		line1.setLength(rad);

		// Calculate the point on the line from p2 to p3, just after p2
		QLineF line2(p2, p3);
		line2.setLength(rad);

		// Draw a line to the point before the waypoint
		path.lineTo(line1.p2());
		// Use the waypoint as the control point for the quadratic bezier curve, and the point after the waypoint as the end point.
		path.quadTo(p2, line2.p2());
	}
	// Draw a line to the last point
	path.lineTo(points.back());

	return path;
}

void BreadboardDesigner::cancelCreation()
{
	creationMode = NotCreating;
	pinsForNewComponent.clear();
	temporaryWaypoints.clear();
	firstHole = nullptr;
	movingConnectionId.clear();
	movingComponentId.clear();
}

void BreadboardDesigner::handleComponentCreationClick(Hole& clickedHole, Qt::KeyboardModifiers modifiers)
{
	if(isHoleOccupied(holeId(clickedHole))) return;

	if(creationMode == NotCreating) {
		if(modifiers.testFlag(Qt::ControlModifier)) {
			creationMode = CreatingMultiPin;
			pinsForNewComponent.push_back(&clickedHole);
		}
	} else if(creationMode == CreatingMultiPin && modifiers.testFlag(Qt::ControlModifier)) {
		auto it = std::find_if(pinsForNewComponent.begin(), pinsForNewComponent.end(), [&](const Hole* pin) {
			return holeId(*pin) == holeId(clickedHole);
		});
		if(it == pinsForNewComponent.end()) {
			pinsForNewComponent.push_back(&clickedHole);
		}
	}
}

void BreadboardDesigner::handleStandardClick(Hole& clickedHole)
{
	if(creationMode != NotCreating) {
		if(!isHoleOccupied(holeId(clickedHole))) {
			auto it = std::find_if(pinsForNewComponent.begin(), pinsForNewComponent.end(), [&](const Hole* pin) {
				return holeId(*pin) == holeId(clickedHole);
			});
			if(it == pinsForNewComponent.end()) {
				pinsForNewComponent.push_back(&clickedHole);
			}

			if((creationMode == CreatingTwoPin && pinsForNewComponent.size() == 2) ||
			   (creationMode == CreatingMultiPin && pinsForNewComponent.size() >= 2)) {
				std::vector<QString> pinIds;
				for(const auto& pin : pinsForNewComponent) {
					pinIds.push_back(holeId(*pin));
				}
				components.push_back({QUuid::createUuid().toString(), pinIds});
			}
		}
		cancelCreation();
	} else if(firstHole) {
		if(!movingComponentId.isNull()) {
			if(holeId(*firstHole) == holeId(clickedHole)) {
				components.erase(std::remove_if(components.begin(), components.end(), [&](const Component& c) {
					return c.id == movingComponentId;
				}), components.end());
			} else if(!isHoleOccupied(holeId(clickedHole))) {
				for(auto& c : components) {
					if(c.id == movingComponentId) {
						for(auto& pin : c.pins) {
							if(pin == originalHoleId) {
								pin = holeId(clickedHole);
								break;
							}
						}
						break;
					}
				}
			}
		} else if(!movingConnectionId.isNull()) {
			Connection* movingConn = nullptr;
			for(auto& c : connections) {
				if(c.id == movingConnectionId) {
					movingConn = &c;
					break;
				}
			}
			if(holeId(*firstHole) == holeId(clickedHole)) {
				connections.erase(std::remove_if(connections.begin(), connections.end(), [&](const Connection& c) {
					return c.id == movingConn->id;
				}), connections.end());
			} else if(!isHoleOccupied(holeId(clickedHole)) && !flashTimer->isActive()) {
				if(movingConn->a == holeId(*firstHole)) movingConn->b = holeId(clickedHole);
				else movingConn->a = holeId(clickedHole);
				movingConn->color = temporaryWireColor;
			}
		} else if(!isHoleOccupied(holeId(clickedHole)) && !flashTimer->isActive()) {
			connections.push_back({QUuid::createUuid().toString(), holeId(*firstHole), holeId(clickedHole), temporaryWireColor, temporaryWaypoints});
		}
		cancelCreation();
	} else {
		bool foundComponent = false;
		for(auto& c : components) {
			auto it = std::find(c.pins.begin(), c.pins.end(), holeId(clickedHole));
			if(it != c.pins.end()) {
				movingComponentId = c.id;
				originalHoleId = *it;
				for(const auto& pin : c.pins) {
					if(pin != originalHoleId) {
						firstHole = findHole(pin);
						break;
					}
				}
				foundComponent = true;
				break;
			}
		}
		if(!foundComponent) {
			bool foundConnection = false;
			for(auto& c : connections) {
				if(c.a == holeId(clickedHole)) {
					firstHole = findHole(c.b);
					movingConnectionId = c.id;
					originalHoleId = c.a;
					foundConnection = true;
					break;
				}
				if(c.b == holeId(clickedHole)) {
					firstHole = findHole(c.a);
					movingConnectionId = c.id;
					originalHoleId = c.b;
					foundConnection = true;
					break;
				}
			}
			if(!foundConnection && !isHoleOccupied(holeId(clickedHole))) {
				firstHole = &clickedHole;
			}
		}
	}
}


void BreadboardDesigner::toggleFlashColor()
{
	temporaryWireColor = flashState ? Qt::red : Qt::black;
	flashState = !flashState;
	update();
}
