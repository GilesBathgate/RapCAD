#include "breadboardcontroller.h"
#include <QMouseEvent>
#include <QTimer>
#include <QUuid>
#include <QVector2D>

BreadboardController::BreadboardController(BreadboardModel *model, QObject *parent)
    : QObject{parent}, m_model(model)
{
    flashTimer = new QTimer(this);
    connect(flashTimer, &QTimer::timeout, this, &BreadboardController::toggleFlashColor);
}

void BreadboardController::handleMousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::RightButton) {
        cancelCreation();
        emit viewNeedsUpdate();
        return;
    }

    Qt::KeyboardModifiers modifiers = event->modifiers();
    if(firstHole && modifiers.testFlag(Qt::ControlModifier)) {
        temporaryWaypoints.push_back(event->pos());
        emit viewNeedsUpdate();
        return;
    }

    Hole* clickedHole = nullptr;
    for(auto& row : m_model->getHoles()) {
        for(auto& h : row) {
            if(QVector2D(event->pos() - QPoint(h.x, h.y)).length() < m_model->holeSize / 2.0) {
                clickedHole = &h;
                break;
            }
        }
        if(clickedHole) break;
    }

    if(!clickedHole) {
        cancelCreation();
        emit viewNeedsUpdate();
        return;
    }

    if(modifiers.testFlag(Qt::ControlModifier) && !firstHole) {
        handleComponentCreationClick(*clickedHole, modifiers);
    } else {
        handleStandardClick(*clickedHole);
    }

    emit viewNeedsUpdate();
}

void BreadboardController::handleMouseMoveEvent(QMouseEvent* event)
{
    mousePos = event->pos();
    Hole* lastHoverHole = hoverHole;
    hoverHole = nullptr;
    for(auto& row : m_model->getHoles()) {
        for(auto& h : row) {
            if(QVector2D(event->pos() - QPoint(h.x, h.y)).length() < m_model->holeSize / 2.0) {
                hoverHole = &h;
                break;
            }
        }
        if(hoverHole) break;
    }

    if(lastHoverHole != hoverHole || firstHole) {
        emit viewNeedsUpdate();
    }

    if(firstHole) {
        bool isShort = hoverHole && ((m_model->isPositiveHole(*firstHole) && m_model->isNegativeHole(*hoverHole)) || (m_model->isNegativeHole(*firstHole) && m_model->isPositiveHole(*hoverHole)));
        if(isShort) {
            if(!flashTimer->isActive()) {
                flashTimer->start(100);
            }
        } else {
            flashTimer->stop();
            if(m_model->isPositiveHole(*firstHole) || (hoverHole && m_model->isPositiveHole(*hoverHole))) {
                temporaryWireColor = Qt::red;
            } else if(m_model->isNegativeHole(*firstHole) || (hoverHole && m_model->isNegativeHole(*hoverHole))) {
                temporaryWireColor = Qt::black;
            } else {
                temporaryWireColor = QColorConstants::White;
            }
        }
    } else {
        flashTimer->stop();
    }
}

void BreadboardController::cancelCreation()
{
    creationMode = NotCreating;
    pinsForNewComponent.clear();
    temporaryWaypoints.clear();
    firstHole = nullptr;
    movingConnectionId.clear();
    movingComponentId.clear();
}

void BreadboardController::handleComponentCreationClick(Hole& clickedHole, Qt::KeyboardModifiers modifiers)
{
    if(m_model->isHoleOccupied(m_model->holeId(clickedHole))) return;

    if(creationMode == NotCreating) {
        if(modifiers.testFlag(Qt::ControlModifier)) {
            creationMode = CreatingMultiPin;
            pinsForNewComponent.push_back(&clickedHole);
        }
    } else if(creationMode == CreatingMultiPin && modifiers.testFlag(Qt::ControlModifier)) {
        auto it = std::find_if(pinsForNewComponent.begin(), pinsForNewComponent.end(), [&](const Hole* pin) {
            return m_model->holeId(*pin) == m_model->holeId(clickedHole);
        });
        if(it == pinsForNewComponent.end()) {
            pinsForNewComponent.push_back(&clickedHole);
        }
    }
}

void BreadboardController::handleStandardClick(Hole& clickedHole)
{
    if(creationMode != NotCreating) {
        if(!m_model->isHoleOccupied(m_model->holeId(clickedHole))) {
            auto it = std::find_if(pinsForNewComponent.begin(), pinsForNewComponent.end(), [&](const Hole* pin) {
                return m_model->holeId(*pin) == m_model->holeId(clickedHole);
            });
            if(it == pinsForNewComponent.end()) {
                pinsForNewComponent.push_back(&clickedHole);
            }

            if((creationMode == CreatingTwoPin && pinsForNewComponent.size() == 2) ||
               (creationMode == CreatingMultiPin && pinsForNewComponent.size() >= 2)) {
                QList<QString> pinIds;
                for(const auto& pin : pinsForNewComponent) {
                    pinIds.push_back(m_model->holeId(*pin));
                }
                m_model->addComponent(pinIds);
            }
        }
        cancelCreation();
    } else if(firstHole) {
        if(!movingComponentId.isNull()) {
            if(m_model->holeId(*firstHole) == m_model->holeId(clickedHole)) {
                m_model->deleteComponent(movingComponentId);
            } else if(!m_model->isHoleOccupied(m_model->holeId(clickedHole))) {
                m_model->moveComponent(movingComponentId, originalHoleId, m_model->holeId(clickedHole));
            }
        } else if(!movingConnectionId.isNull()) {
            Connection* movingConn = nullptr;
            for(auto& c : m_model->getConnections()) {
                if(c.id == movingConnectionId) {
                    movingConn = &c;
                    break;
                }
            }
            if(m_model->holeId(*firstHole) == m_model->holeId(clickedHole)) {
                m_model->getConnections().erase(std::remove_if(m_model->getConnections().begin(), m_model->getConnections().end(), [&](const Connection& c) {
                    return c.id == movingConn->id;
                }), m_model->getConnections().end());
            } else if(!m_model->isHoleOccupied(m_model->holeId(clickedHole)) && !flashTimer->isActive()) {
                m_model->moveConnection(movingConnectionId, originalHoleId, m_model->holeId(clickedHole));
            }
        } else if(!m_model->isHoleOccupied(m_model->holeId(clickedHole)) && !flashTimer->isActive()) {
            QList<QPoint> waypoints;
            for(const auto& p : temporaryWaypoints) {
                waypoints.append(p);
            }
            m_model->addConnection(m_model->holeId(*firstHole), m_model->holeId(clickedHole), temporaryWireColor.name(), waypoints);
        }
        cancelCreation();
    } else {
        bool foundComponent = false;
        for(auto& c : m_model->getComponents()) {
            auto it = std::find(c.pins.begin(), c.pins.end(), m_model->holeId(clickedHole));
            if(it != c.pins.end()) {
                movingComponentId = c.id;
                originalHoleId = *it;
                for(const auto& pin : c.pins) {
                    if(pin != originalHoleId) {
                        firstHole = m_model->findHole(pin);
                        break;
                    }
                }
                foundComponent = true;
                break;
            }
        }
        if(!foundComponent) {
            bool foundConnection = false;
            for(auto& c : m_model->getConnections()) {
                if(c.a == m_model->holeId(clickedHole)) {
                    firstHole = m_model->findHole(c.b);
                    movingConnectionId = c.id;
                    originalHoleId = c.a;
                    foundConnection = true;
                    break;
                }
                if(c.b == m_model->holeId(clickedHole)) {
                    firstHole = m_model->findHole(c.a);
                    movingConnectionId = c.id;
                    originalHoleId = c.b;
                    foundConnection = true;
                    break;
                }
            }
            if(!foundConnection && !m_model->isHoleOccupied(m_model->holeId(clickedHole))) {
                firstHole = &clickedHole;
            }
        }
    }
}

void BreadboardController::toggleFlashColor()
{
    temporaryWireColor = flashState ? Qt::red : Qt::black;
    flashState = !flashState;
    emit viewNeedsUpdate();
}
