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

BEllipse::BEllipse(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BGraphicsItem(QPointF(x,y), type), m_edge(QPointF(x+width/2,y+height/2))
{
    BPointItem *point = new BPointItem(this, m_edge, BPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
    m_pointList.setRandColor();
}

QRectF BEllipse::boundingRect() const
{
    return QRectF(m_center.x() - m_edge.x(),
                  m_center.y() - m_edge.y(),
                  m_edge.x() * 2,
                  m_edge.y() * 2);
}

void BEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_edge.x(),
               m_center.y() - m_edge.y(),
               m_edge.x() * 2,
               m_edge.y() * 2);
    painter->drawEllipse(ret);
}

void BEllipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qreal dx = event->scenePos().x() - event->lastScenePos().x();
    qreal dy = event->scenePos().y() - event->lastScenePos().y();
    this->setCenter( this->getCenter() + QPointF(dx, dy) );
    this->setEdge( this->getEdge() + QPointF(dx, dy) );

    QAbstractGraphicsShapeItem::mouseReleaseEvent(event);
}

//------------------------------------------------------------------------------

BCircle::BCircle(qreal x, qreal y, qreal radius, ItemType type) : BEllipse(x, y, radius*2, radius*2, type)
{
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

//------------------------------------------------------------------------------

BRectangle::BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BGraphicsItem(QPointF(x,y), type), m_edge(QPointF(width/2,height/2))
{
    BPointItem *point = new BPointItem(this, m_edge, BPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
    m_pointList.setRandColor();
}

QRectF BRectangle::boundingRect() const
{
    return QRectF(m_center.x() - m_edge.x(),
                  m_center.y() - m_edge.y(),
                  m_edge.x() * 2,
                  m_edge.y() * 2);
}

void BRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_edge.x(),
               m_center.y() - m_edge.y(),
               m_edge.x() * 2,
               m_edge.y() * 2);
    painter->drawRect(ret);
}

void BRectangle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qreal dx = event->scenePos().x() - event->lastScenePos().x();
    qreal dy = event->scenePos().y() - event->lastScenePos().y();
    this->setCenter( this->getCenter() + QPointF(dx, dy) );
    this->setEdge( this->getEdge() + QPointF(dx, dy) );

    QAbstractGraphicsShapeItem::mouseReleaseEvent(event);
}

//------------------------------------------------------------------------------

BSquare::BSquare(qreal x, qreal y, qreal width, ItemType type) : BRectangle(x, y, width, width, type) {}
