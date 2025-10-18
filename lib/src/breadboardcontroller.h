#ifndef BREADBOARDCONTROLLER_H
#define BREADBOARDCONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QColor>
#include <vector>

#include "breadboardmodel.h"

class QMouseEvent;
class QTimer;

class BreadboardController : public QObject
{
    Q_OBJECT
public:
    enum CreationMode { NotCreating, CreatingTwoPin, CreatingMultiPin };

    explicit BreadboardController(BreadboardModel *model, QObject *parent = nullptr);

    void handleMousePressEvent(QMouseEvent* event);
    void handleMouseMoveEvent(QMouseEvent* event);

    // Getters for view state
    CreationMode getCreationMode() const { return creationMode; }
    Hole* getFirstHole() const { return firstHole; }
    Hole* getHoverHole() const { return hoverHole; }
    QPoint getMousePos() const { return mousePos; }
    QColor getTemporaryWireColor() const { return temporaryWireColor; }
    const std::vector<QPoint>& getTemporaryWaypoints() const { return temporaryWaypoints; }
    const std::vector<Hole*>& getPinsForNewComponent() const { return pinsForNewComponent; }
    QString getMovingConnectionId() const { return movingConnectionId; }
    QString getMovingComponentId() const { return movingComponentId; }

signals:
    void viewNeedsUpdate();

private slots:
    void toggleFlashColor();

private:
    void handleComponentCreationClick(Hole& clickedHole, Qt::KeyboardModifiers modifiers);
    void handleStandardClick(Hole& clickedHole);
    void cancelCreation();

    BreadboardModel *m_model;

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
    std::vector<QPoint> temporaryWaypoints;
};

#endif // BREADBOARDCONTROLLER_H
