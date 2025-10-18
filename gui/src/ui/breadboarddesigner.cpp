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
    m_model = new BreadboardModel(this);
    m_controller = new BreadboardController(m_model, this);
    connect(m_controller, &BreadboardController::viewNeedsUpdate, this, QOverload<>::of(&BreadboardDesigner::update));
    connect(ui->actionNew, &QAction::triggered, this, &BreadboardDesigner::clearConnections);
}

BreadboardDesigner::~BreadboardDesigner()
{
    delete ui;
}

void BreadboardDesigner::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	auto drawComponent = [&](QPointF start, QPointF end) {
		painter.drawLine(start, end);
		if(start.x() == end.x()) {  // vertical
			int boxHeight = abs(start.y() - end.y()) - m_model->holeSize * 2;
			painter.drawRect(start.x() - m_model->holeSize/2, std::min(start.y(), end.y()) + m_model->holeSize, m_model->holeSize, boxHeight);
		} else { // horizontal
			int boxWidth = abs(start.x() - end.x()) - m_model->holeSize * 2;
			painter.drawRect(std::min(start.x(), end.x()) + m_model->holeSize, start.y() - m_model->holeSize/2, boxWidth, m_model->holeSize);
		}
	};

    // Draw holes
    for (const auto& row : m_model->getHoles()) {
        for (const auto& h : row) {
            bool sameGroupHover = m_controller->getHoverHole() && m_model->sameGroup(*m_controller->getHoverHole(), h);
            QColor fillColor = sameGroupHover ? QColor("#bfdbfe") : (m_controller->getHoverHole() && m_model->holeId(*m_controller->getHoverHole()) == m_model->holeId(h)) ? QColor("#fde68a") : QColorConstants::Gray;
            painter.setBrush(fillColor);
            painter.setPen(QColor(0,0,0,76));
            painter.drawEllipse(QPoint(h.x, h.y), m_model->holeSize / 2, m_model->holeSize / 2);
        }
    }

    // Draw polarity lines
    if (m_model->getHoles().size() >= 14) {
        auto drawLine = [&](int rowIndex, const QColor& color, int y_offset) {
            const auto& row = m_model->getHoles()[rowIndex];
            if (!row.empty()) {
                QPen pen(color);
                pen.setWidth(1);
                painter.setPen(pen);
                int y = row[0].y + y_offset;
                painter.drawLine(row[0].x, y, row.back().x, y);
            }
        };

        int offset = (m_model->holeGap / 2) + 2;
        drawLine(0, Qt::red, -offset);    // Top positive
        drawLine(1, Qt::black, offset);   // Top negative
        drawLine(12, Qt::black, -offset); // Bottom negative
        drawLine(13, Qt::red, offset);    // Bottom positive
    }

    // Draw wires
    for (const auto& c : m_model->getConnections()) {
        if (!m_controller->getMovingConnectionId().isNull() && c.id == m_controller->getMovingConnectionId()) continue;
        Hole* a = m_model->findHole(c.a);
        Hole* b = m_model->findHole(c.b);
        if (a && b) {
            QPen pen(QColor(c.color));
            pen.setWidth(3);
            pen.setCapStyle(Qt::RoundCap);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            if (c.waypoints.empty()) {
                painter.drawPath(makePath(QPointF(a->x, a->y), QPointF(b->x, b->y)));
            } else {
                painter.drawPath(makeWaypointPath(QPointF(a->x, a->y), QPointF(b->x, b->y), c.waypoints));
            }
        }
    }

    // Draw components
    for (const auto& c : m_model->getComponents()) {
        if (!m_controller->getMovingComponentId().isNull() && c.id == m_controller->getMovingComponentId()) continue;
        if (c.pins.empty()) continue;

        painter.setPen(QPen(QColorConstants::White, 1));
        painter.setBrush(QColorConstants::White);

        if (c.pins.size() >= 2) {
            if (c.pins.size() == 2) {  // Draw as a resistor
                Hole* a = m_model->findHole(c.pins[0]);
                Hole* b = m_model->findHole(c.pins[1]);
                if (a && b) {
                    if (a->r == b->r || a->c == b->c) {
                        drawComponent(QPointF(a->x, a->y), QPointF(b->x, b->y));
                    }
                }
            } else { // Draw as a bounding box
                int minX = -1, minY = -1, maxX = -1, maxY = -1;
                for (const auto& pinId : c.pins) {
                    Hole* h = m_model->findHole(pinId);
                    if (h) {
                        if (minX == -1 || h->x < minX) minX = h->x;
                        if (minY == -1 || h->y < minY) minY = h->y;
                        if (maxX == -1 || h->x > maxX) maxX = h->x;
                        if (maxY == -1 || h->y > maxY) maxY = h->y;
                    }
                }
                if (minX != -1) {
                    painter.drawRect(QRect(QPoint(minX - m_model->holeSize/2, minY - m_model->holeSize/2), QPoint(maxX + m_model->holeSize/2, maxY + m_model->holeSize/2)));
                    // Also draw the pins
                    painter.setBrush(QColorConstants::Gray);
                    for (const auto& pinId : c.pins) {
                        Hole* h = m_model->findHole(pinId);
                        if (h) {
                            painter.drawEllipse(QPoint(h->x, h->y), m_model->holeSize / 2, m_model->holeSize / 2);
                        }
                    }
                }
            }
        }
    }

    // Draw temporary component placement
    if (m_controller->getCreationMode() != BreadboardController::NotCreating && !m_controller->getPinsForNewComponent().empty()) {
        painter.setPen(QPen(QColorConstants::White, 1));
        painter.setBrush(QColor(128, 128, 128, 128));
        painter.setOpacity(0.8);

        if (m_controller->getCreationMode() == BreadboardController::CreatingTwoPin || (m_controller->getCreationMode() == BreadboardController::CreatingMultiPin && m_controller->getPinsForNewComponent().size() == 1)) {
            QPointF start(m_controller->getPinsForNewComponent()[0]->x, m_controller->getPinsForNewComponent()[0]->y);
            QPointF end = m_controller->getMousePos();
            if (abs(start.x() - end.x()) > abs(start.y() - end.y())) {
                end.setY(start.y());
            } else {
                end.setX(start.x());
            }
            drawComponent(start, end);
        } else if (m_controller->getCreationMode() == BreadboardController::CreatingMultiPin && m_controller->getPinsForNewComponent().size() > 1) {
            int minX = m_controller->getPinsForNewComponent()[0]->x;
            int minY = m_controller->getPinsForNewComponent()[0]->y;
            int maxX = m_controller->getPinsForNewComponent()[0]->x;
            int maxY = m_controller->getPinsForNewComponent()[0]->y;

            for (size_t i = 1; i < m_controller->getPinsForNewComponent().size(); ++i) {
                minX = std::min(minX, m_controller->getPinsForNewComponent()[i]->x);
                minY = std::min(minY, m_controller->getPinsForNewComponent()[i]->y);
                maxX = std::max(maxX, m_controller->getPinsForNewComponent()[i]->x);
                maxY = std::max(maxY, m_controller->getPinsForNewComponent()[i]->y);
            }

            minX = std::min(minX, (int)m_controller->getMousePos().x());
            minY = std::min(minY, (int)m_controller->getMousePos().y());
            maxX = std::max(maxX, (int)m_controller->getMousePos().x());
            maxY = std::max(maxY, (int)m_controller->getMousePos().y());

            int inflation = 5;
            painter.drawRect(QRect(QPoint(minX - inflation, minY - inflation), QPoint(maxX + inflation, maxY + inflation)));
        }
    }

    if (m_controller->getFirstHole()) {
        if (m_controller->getMovingComponentId().isNull()) {  // Drawing a temporary wire
            QPen pen(m_controller->getTemporaryWireColor());
            pen.setWidth(3);
            pen.setCapStyle(Qt::RoundCap);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.setOpacity(0.8);

            if (m_controller->getTemporaryWaypoints().empty()) {
                painter.drawPath(makePath(QPointF(m_controller->getFirstHole()->x, m_controller->getFirstHole()->y), m_controller->getMousePos()));
            } else {
                QVector<QPoint> waypoints;
                for(const auto& p : m_controller->getTemporaryWaypoints()) {
                    waypoints.append(p);
                }
                painter.drawPath(makeWaypointPath(QPointF(m_controller->getFirstHole()->x, m_controller->getFirstHole()->y), m_controller->getMousePos(), waypoints));
            }
        } else { // Drawing a temporary component (while moving)
            painter.setPen(QPen(QColorConstants::White, 1));
            painter.setBrush(QColorConstants::White);
            painter.setOpacity(0.8);
            QPointF start(m_controller->getFirstHole()->x, m_controller->getFirstHole()->y);
            QPointF end = m_controller->getMousePos();
            if (abs(start.x() - end.x()) > abs(start.y() - end.y())) {
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
    m_controller->handleMousePressEvent(event);
}

void BreadboardDesigner::mouseMoveEvent(QMouseEvent* event)
{
    m_controller->handleMouseMoveEvent(event);
}

void BreadboardDesigner::clearConnections()
{
    m_model->clear();
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

QPainterPath BreadboardDesigner::makeWaypointPath(const QPointF& a, const QPointF& b, const QVector<QPoint>& waypoints) const
{
    QPainterPath path;
    QVector<QPointF> points;
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

        qreal rad = m_model->holeSize;

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
