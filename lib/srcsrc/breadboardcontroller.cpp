#include "breadboardcontroller.h"
#include <QMouseEvent>

BreadboardController::BreadboardController(BreadboardModel *model, QObject *parent)
    : QObject{parent}, m_model(model)
{

}

void BreadboardController::handleMousePressEvent(QMouseEvent* event)
{
    // Logic to be moved from BreadboardDesigner::mousePressEvent
}

void BreadboardController::handleMouseMoveEvent(QMouseEvent* event)
{
    // Logic to be moved from BreadboardDesigner::mouseMoveEvent
}
