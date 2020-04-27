#include "bqgraphicsitem.h"
#include <QDebug>

BGraphicsItem::BGraphicsItem(QPointF center, ItemType type) : m_center(center), m_type(type)
{
    m_pen_noSelected.setColor(QColor(0, 160, 230));
    m_pen_noSelected.setWidth(2);
    m_pen_isSelected.setColor(QColor(255, 0, 255));
    m_pen_isSelected.setWidth(2);

    this->setPen(m_pen_noSelected);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);

    m_itemList.append(this);
}

void BGraphicsItem::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    this->setPen(m_pen_isSelected);
}

void BGraphicsItem::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    this->setPen(m_pen_noSelected);
}

//------------------------------------------------------------------------------

BCircle::BCircle(QPointF center, QPointF edge, ItemType type) : BGraphicsItem(center, type), m_edge(edge)
{
    BPointItem *point = new BPointItem(this, edge, BPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new BPointItem(this, center, BPointItem::Center));
    m_pointList.setRandColor();

    updateRadius();
}

QRectF BCircle::boundingRect() const
{
    return QRectF(m_center.x() - m_radius,
                  m_center.y() - m_radius,
                  m_radius * 2,
                  m_radius * 2);
}

void BCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_radius,
               m_center.y() - m_radius,
               m_radius * 2,
               m_radius * 2);
    painter->drawEllipse(ret);
}

void BCircle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qreal dx = event->scenePos().x() - event->lastScenePos().x();
    qreal dy = event->scenePos().y() - event->lastScenePos().y();
    this->setCenter( this->getCenter() + QPointF(dx, dy) );
    this->setEdge( this->getEdge() + QPointF(dx, dy) );

    QAbstractGraphicsShapeItem::mouseReleaseEvent(event);
}
