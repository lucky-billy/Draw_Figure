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
    switch ( event->buttons() ) {
    case Qt::LeftButton :
        m_point = this->mapToParent( event->pos() );
        this->setPos(m_point);
        this->scene()->update();
        break;
    default:
        break;
    }
}

void BPointItem::keyPressEvent(QKeyEvent *event)
{
    switch ( event->key() ) {
    case Qt::Key_Left :
        m_point.rx() --;
        break;
    case Qt::Key_Right :
        m_point.rx() ++;
        break;
    case Qt::Key_Up :
        m_point.ry() --;
        break;
    case Qt::Key_Down :
        m_point.ry() ++;
        break;
    default:
        break;
    }
    this->setPos(m_point);
    this->scene()->update();
}

QVariant BPointItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
    case ItemSelectedHasChanged : {
        if( value.toBool() ) {
            this->setPen(QPen(QColor(255,255,255,255)));
        } else {
            this->setPen(QPen(Qt::NoPen));
        }
        break;
    }
    default:
        break;
    }
    return QGraphicsItem::itemChange(change, value);
}

QList<QPointF> BPointItemList::getPoints() const
{
    QList<QPointF> list;

    for (auto &temp : *this)
    {
        list.append(temp->getPoint());
    }

    return list;
}

void BPointItemList::setColor(const QColor color)
{
    for (auto &temp : *this)
    {
        temp->setBrush(QBrush(color));
    }
}

void BPointItemList::setRandColor()
{
    this->setColor(QColor(qrand()%256, qrand()%256, qrand()%256));
}

void BPointItemList::setVisible(bool visible)
{
    for (auto &temp : *this)
    {
        temp->setVisible(visible);
    }
}
