#include "breadboardmodel.h"
#include <QUuid>

BreadboardModel::BreadboardModel(QObject *parent)
    : QObject{parent}
{
    holes = buildHoles();
}

QList<QList<Hole>> BreadboardModel::getHoles() const
{
    return holes;
}

QList<Connection> BreadboardModel::getConnections() const
{
    return connections;
}

QList<Component> BreadboardModel::getComponents() const
{
    return components;
}

QList<QList<Hole>> BreadboardModel::buildHoles()
{
    QList<QList<Hole>> rows;
    int startX = 60;
    int startY = 60;
    int y = startY;
    int gapBetweenBlocks = 5 * holeGap;

    auto makeRow = [&](const QString& band, int numRows) {
        for(int r = 0; r < numRows; ++r) {
            QList<Hole> row;
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

QString BreadboardModel::holeId(const Hole& h) const
{
    return QString("%1:%2:%3").arg(h.band).arg(h.r).arg(h.c);
}

Hole* BreadboardModel::findHole(const QString& id)
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

bool BreadboardModel::isHoleOccupied(const QString& id) const
{
    for(const auto& c : connections) {
        if (c.id == movingConnectionId) continue;
        if(c.a == id || c.b == id) {
            return true;
        }
    }
    for(const auto& c : components) {
        if (c.id == movingComponentId) continue;
        for(const auto& p : c.pins) {
            if(p == id) {
                return true;
            }
        }
    }
    return false;
}

void BreadboardModel::addConnection(const QString& a, const QString& b, const QString& color, const QList<QPoint>& waypoints)
{
    connections.append({QUuid::createUuid().toString(), a, b, color, waypoints});
}

void BreadboardModel::addComponent(const QList<QString>& pins)
{
    components.append({QUuid::createUuid().toString(), pins});
}

void BreadboardModel::moveComponent(const QString& componentId, const QString& originalPinId, const QString& newPinId)
{
    for(auto& c : components) {
        if(c.id == componentId) {
            for(auto& p : c.pins) {
                if(p == originalPinId) {
                    p = newPinId;
                    return;
                }
            }
        }
    }
}

void BreadboardModel::deleteComponent(const QString& componentId)
{
    components.erase(std::remove_if(components.begin(), components.end(), [&](const Component& c) {
        return c.id == componentId;
    }), components.end());
}

void BreadboardModel::clear()
{
    connections.clear();
    components.clear();
}

void BreadboardModel::setMovingConnectionId(const QString& id)
{
    movingConnectionId = id;
}

void BreadboardModel::setMovingComponentId(const QString& id)
{
    movingComponentId = id;
}

void BreadboardModel::moveConnection(const QString& connectionId, const QString& originalPinId, const QString& newPinId)
{
    for(auto& c : connections) {
        if(c.id == connectionId) {
            if(c.a == originalPinId) {
                c.a = newPinId;
            } else {
                c.b = newPinId;
            }
            setMovingConnectionId(QString());
            return;
        }
    }
}

bool BreadboardModel::sameGroup(const Hole& h1, const Hole& h2) const
{
    if(h1.band.contains("power") && h1.band == h2.band && h1.r == h2.r) return true;
    if(h1.band.contains("term") && h1.band == h2.band && h1.c == h2.c) return true;
    return false;
}

bool BreadboardModel::isPositiveHole(const Hole& h) const
{
    return (h.band == "power-top" && h.r == 0) || (h.band == "power-bottom" && h.r == 1);
}

bool BreadboardModel::isNegativeHole(const Hole& h)
{
    return (h.band == "power-top" && h.r == 1) || (h.band == "power-bottom" && h.r == 0);
}
