#include "bpointitem.h"

BPointItem::BPointItem(QAbstractGraphicsShapeItem* parent, QPointF p, bool type)
    : QAbstractGraphicsShapeItem(parent)
    , m_point(p)
    , isCenter(type)
{
    this->setPos(m_point);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);

    if (isCenter) {
        this->setCursor(Qt::OpenHandCursor);
    } else {
        this->setCursor(Qt::PointingHandCursor);
    }
}

QRectF BPointItem::boundingRect() const
{
    return QRectF(-4, -4, 8, 8);
}

void BPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    if (isCenter) {
        painter->drawEllipse(-4, -4, 8, 8);
    } else {
        painter->drawRect(QRectF(-4, -4, 8, 8));
    }
    this->setPos(m_point);
}

void BPointItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->buttons() == Qt::LeftButton ) {
        if (isCenter) {
            QGraphicsItem* item = this->parentItem();
            item->moveBy(event->scenePos().x() - event->lastScenePos().x(),
                         event->scenePos().y() - event->lastScenePos().y());
            this->scene()->update();
        } else {
            m_point = this->mapToParent( event->pos() );
            this->setPos(m_point);
            this->scene()->update();
        }
    }
}

void BPointItemList::setRandColor()
{
    this->setColor(QColor(qrand()%256, qrand()%256, qrand()%256));
}

void BPointItemList::setColor(const QColor color)
{
    for (auto &temp : *this)
    {
        temp->setBrush(QBrush(color));
    }
}

void BPointItemList::setVisible(bool visible)
{
    for (auto &temp : *this)
    {
        temp->setVisible(visible);
    }
}
