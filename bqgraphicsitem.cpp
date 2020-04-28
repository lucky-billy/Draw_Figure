#include "bqgraphicsitem.h"
#include <QVector>
#include <QDebug>

BGraphicsItem::BGraphicsItem(QPointF center, QPointF edge, ItemType type)
    : m_center(center), m_edge(edge), m_type(type)
{
    m_pen_noSelected.setColor(QColor(0, 160, 230));
    m_pen_noSelected.setWidth(2);
    m_pen_isSelected.setColor(QColor(255, 0, 255));
    m_pen_isSelected.setWidth(2);

    this->setPen(m_pen_noSelected);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);
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
    : BGraphicsItem(QPointF(x,y), QPointF(x+width/2,y+height/2), type)
{
    BPointItem *point = new BPointItem(this, m_edge, BPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
    m_pointList.setRandColor();
}

QRectF BEllipse::boundingRect() const
{
    return QRectF(m_center.x() - m_edge.x(), m_center.y() - m_edge.y(), m_edge.x() * 2, m_edge.y() * 2);
}

void BEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_edge.x(), m_center.y() - m_edge.y(), m_edge.x() * 2, m_edge.y() * 2);
    painter->drawEllipse(ret);
}

//------------------------------------------------------------------------------

BCircle::BCircle(qreal x, qreal y, qreal radius, ItemType type)
    : BEllipse(x, y, radius*2, radius*2, type)
{
    updateRadius();
}

void BCircle::updateRadius()
{
    m_radius = sqrt(pow(m_center.x() - m_edge.x(), 2) + pow(m_center.y() - m_edge.y(), 2));
}

QRectF BCircle::boundingRect() const
{
    return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
    painter->drawEllipse(ret);
}

//------------------------------------------------------------------------------

BConcentricCircle::BConcentricCircle(qreal x, qreal y, qreal radius1, qreal radius2, ItemType type)
    : BCircle(x, y, radius1, type), m_another_edge(x+radius2, y+radius2)
{
    BPointItem *point = new BPointItem(this, m_another_edge, BPointItem::Con_Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.setRandColor();

    updateOtherRadius();
}

void BConcentricCircle::updateOtherRadius()
{
    m_another_radius = sqrt(pow(m_center.x() - m_another_edge.x(), 2) +
                            pow(m_center.y() - m_another_edge.y(), 2));
}

QPointF BConcentricCircle::getAnotherEdge()
{
    return m_another_edge;
}

void BConcentricCircle::setAnotherEdge(QPointF p)
{
    m_another_edge = p;
}

QRectF BConcentricCircle::boundingRect() const
{
    qreal temp = m_radius > m_another_radius ? m_radius : m_another_radius;
    return QRectF(m_center.x() - temp, m_center.y() - temp, temp * 2, temp * 2);
}

void BConcentricCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_another_radius, m_center.y() - m_another_radius, m_another_radius * 2, m_another_radius * 2);
    painter->drawEllipse(ret);

    BCircle::paint(painter, option, widget);
}

//------------------------------------------------------------------------------

BPie::BPie(qreal x, qreal y, qreal radius, qreal angle, ItemType type)
    : BCircle(x, y, radius, type), m_angle(angle)
{
    if ( (angle >= 0 && angle < 90) || (angle >= 270 && angle < 360) )
    {
        m_edge.setX( m_center.x() + radius * cos(angle/180*PI) );
        m_edge.setY( m_center.y() + radius * sin(angle/180*PI) * (-1) );
    }
    else if ( (angle >= 90 && angle < 270) )
    {
        m_edge.setY( m_center.y() + radius * sin(angle/180*PI) * (-1) );
        m_edge.setX( m_center.x() + radius * cos(angle/180*PI) );
    }

    m_pointList.at(0)->setPoint(m_edge);
    m_radius = radius;
}

void BPie::updateAngle()
{
    qreal dx = m_edge.x() - m_center.x();
    qreal dy = m_edge.y() - m_center.y();

    if ( dx >= 0 && dy < 0 )
    {
        m_angle = atan2( (-1)*(dy), dx ) *180/PI;
    }
    else if ( dx < 0 && dy < 0 )
    {
        m_angle = atan2( (-1)*dy, dx ) *180/PI;
    }
    else if ( dx < 0 && dy >= 0 )
    {
        m_angle = 360 + atan2( (-1)*dy, dx ) *180/PI;
    }
    else if ( dx >= 0 && dy >= 0 )
    {
        m_angle = 360 - atan2( dy, dx ) *180/PI;
    }
}

void BPie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
    painter->drawPie(ret, 16*0, 16*(m_angle));
}

//------------------------------------------------------------------------------

BChord::BChord(qreal x, qreal y, qreal radius, qreal angle, ItemType type)
    : BPie(x, y, radius, angle, type)
{
    updateEndAngle();
}

void BChord::updateEndAngle()
{
    qreal dx = m_edge.x() - m_center.x();
    qreal dy = m_edge.y() - m_center.y();

    if ( dx >= 0 && dy < 0 )
    {
        m_angle = atan2( (-1)*(dy), dx ) *180/PI;
    }
    else if ( dx < 0 && dy < 0 )
    {
        m_angle = 180 - atan2( (-1)*dy, dx ) *180/PI;
    }
    else if ( dx < 0 && dy >= 0 )
    {
        m_angle = 360 + atan2( (-1)*dy, dx ) *180/PI;
    }
    else if ( dx >= 0 && dy >= 0 )
    {
        m_angle = atan2( (-1)*dy, dx ) *180/PI;
    }

    m_end_angle = 180 - m_angle*2;
}

void BChord::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
    painter->drawChord(ret, 16*(m_angle), 16*(m_end_angle));
}

//------------------------------------------------------------------------------

BRectangle::BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BGraphicsItem(QPointF(x,y), QPointF(width/2,height/2), type)
{
    BPointItem *point = new BPointItem(this, m_edge, BPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
    m_pointList.setRandColor();
}

QRectF BRectangle::boundingRect() const
{
    return QRectF(m_center.x() - m_edge.x(), m_center.y() - m_edge.y(), m_edge.x() * 2, m_edge.y() * 2);
}

void BRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_edge.x(), m_center.y() - m_edge.y(), m_edge.x() * 2, m_edge.y() * 2);
    painter->drawRect(ret);
}

//------------------------------------------------------------------------------

BSquare::BSquare(qreal x, qreal y, qreal width, ItemType type)
    : BRectangle(x, y, width, width, type) {}

//------------------------------------------------------------------------------

BPolygon::BPolygon(QList<QPointF> list, ItemType type)
    : BGraphicsItem(QPointF(0,0), QPointF(0,0), type)
{
    for (auto &temp : list)
    {
        BPointItem *point = new BPointItem(this, temp, BPointItem::Edge);
        point->setParentItem(this);
        m_pointList.append(point);
    }

    m_center = getCentroid(list);
    m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
    m_pointList.setRandColor();

    getMaxLength();
}

QPointF BPolygon::getCentroid(QList<QPointF> list)
{
    qreal x = 0;
    qreal y = 0;
    for (auto &temp : list)
    {
        x += temp.x();
        y += temp.y();
    }
    x = x/list.size();
    y = y/list.size();
    return QPointF(x,y);
}

void BPolygon::getMaxLength()
{
    QVector<qreal> vec;
    for (auto &temp : m_pointList)
    {
        qreal dis = sqrt(pow(m_center.x() - temp->x(), 2) + pow(m_center.y() - temp->y(), 2));
        vec.append(dis);
    }

    qreal ret = 0;
    for (auto &temp : vec)
    {
        if (temp > ret) {
            ret = temp;
        }
    }
    m_radius = ret;
}

void BPolygon::updatePolygon(QPointF origin, QPointF end)
{
    QList<QPointF> list;

    for (auto &temp : m_pointList) {
        if (temp->getPoint() == origin) {
            temp->setPoint(end);
        }
        list.append(temp->getPoint());
    }

    m_center = getCentroid(list);
    m_pointList.at(m_pointList.size()-1)->setPoint(m_center);

    getMaxLength();
}

QRectF BPolygon::boundingRect() const
{
    return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BPolygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    for (int i = 1; i < m_pointList.size() - 1; i++)
    {
        painter->drawLine(m_pointList.at(i-1)->getPoint(), m_pointList.at(i)->getPoint());
    }

    painter->drawLine(m_pointList.at(m_pointList.size()-2)->getPoint(), m_pointList.at(0)->getPoint());
}

//------------------------------------------------------------------------------



