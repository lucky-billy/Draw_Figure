#include "bqgraphicsitem.h"
#include <QDebug>

BGraphicsItem::BGraphicsItem(QGraphicsPixmapItem* parent) : QAbstractGraphicsShapeItem(parent)
{
    QPen p = this->pen();
    p.setColor(QColor(0, 160, 230));
    p.setWidth(2);
    this->setPen(p);

    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);

    m_itemList.append(this);
}

BCircle::BCircle(qreal x, qreal y, qreal radius, QGraphicsPixmapItem* parent)
    : BGraphicsItem(parent)
    , radius(radius)
{
    center.setX(x);
    center.setY(y);
    QPointF edge(x+radius, y);
    m_pointList.append(new BPointItem(this, center, true));
    m_pointList.append(new BPointItem(this, edge, false));
    m_pointList.setRandColor();
}

QRectF BCircle::boundingRect() const
{
    return QRectF(center.x() - radius,
                  center.y() - radius,
                  radius * 2,
                  radius * 2);
}

void BCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    QRectF ret(center.x() - radius,
               center.y() - radius,
               radius * 2,
               radius * 2);
    painter->drawEllipse(ret);
}
