#ifndef BREADBOARDMODEL_H
#define BREADBOARDMODEL_H

#include <QObject>
#include <QVector>
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
    QVector<QPoint> waypoints;
};

class Component {
public:
    QString id;
    QVector<QString> pins;
};

class BreadboardModel : public QObject
{
    Q_OBJECT
public:
    explicit BreadboardModel(QObject *parent = nullptr);

    const QVector<QVector<Hole>>& getHoles() const;
    const QVector<Connection>& getConnections() const;
    const QVector<Component>& getComponents() const;

    Hole* findHole(const QString& id);
    bool isHoleOccupied(const QString& id) const;
    bool sameGroup(const Hole& h1, const Hole& h2) const;
    bool isPositiveHole(const Hole& h) const;
    bool isNegativeHole(const Hole& h);
    QString holeId(const Hole& h) const;

private:
    QVector<QVector<Hole>> buildHoles();

    QVector<QVector<Hole>> holes;
    QVector<Connection> connections;
    QVector<Component> components;

public:
    void addConnection(const QString& a, const QString& b, const QString& color, const QVector<QPoint>& waypoints);
    void addComponent(const QVector<QString>& pins);
    void moveComponent(const QString& componentId, const QString& originalPinId, const QString& newPinId);
    void updateConnectionColor(const QString& connectionId, const QString& color);
    void deleteComponent(const QString& componentId);
    void deleteConnection(const QString& connectionId);
    void clear();

    int columns = 30;
    int holeSize = 12;
    int holeGap = 18;

    void setMovingConnectionId(const QString& id);
    void setMovingComponentId(const QString& id);
    void moveConnection(const QString& connectionId, const QString& originalPinId, const QString& newPinId);

private:
    QString movingConnectionId;
    QString movingComponentId;
};

#endif // BREADBOARDMODEL_H