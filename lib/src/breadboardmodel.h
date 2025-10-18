#ifndef BREADBOARDMODEL_H
#define BREADBOARDMODEL_H

#include <QObject>
#include <QList>
#include <QString>
#include <QPoint>

class Hole {
public:
    QString band;
    int r, c;
    int x, y;
};

class Connection {
public:
    QString id;
    QString a, b;
    QString color;
    QList<QPoint> waypoints;
};

class Component {
public:
    QString id;
    QList<QString> pins;
};

class BreadboardModel : public QObject
{
    Q_OBJECT
public:
    explicit BreadboardModel(QObject *parent = nullptr);

    QList<QList<Hole>> getHoles() const;
    QList<Connection> getConnections() const;
    QList<Component> getComponents() const;

    Hole* findHole(const QString& id);
    bool isHoleOccupied(const QString& id) const;
    bool sameGroup(const Hole& h1, const Hole& h2) const;
    bool isPositiveHole(const Hole& h) const;
    bool isNegativeHole(const Hole& h);
    QString holeId(const Hole& h) const;

private:
    QList<QList<Hole>> buildHoles();

    QList<QList<Hole>> holes;
    QList<Connection> connections;
    QList<Component> components;

public:
    void addConnection(const QString& a, const QString& b, const QString& color, const QList<QPoint>& waypoints);
    void addComponent(const QList<QString>& pins);
    void moveComponent(const QString& componentId, const QString& originalPinId, const QString& newPinId);
    void deleteComponent(const QString& componentId);
    void clear();

    int columns = 30;
    int holeSize = 12;
    int holeGap = 18;

private:
    QString movingConnectionId;
    QString movingComponentId;

public:
    void setMovingConnectionId(const QString& id);
    void setMovingComponentId(const QString& id);
    void moveConnection(const QString& connectionId, const QString& originalPinId, const QString& newPinId);
};

#endif // BREADBOARDMODEL_H