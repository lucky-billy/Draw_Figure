#include "bqgraphicsitem.h"
#include <QVector>
#include <QDebug>
#include <QMenu>
#include <QSpinBox>
#include <QWidgetAction>

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

void BEllipse::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }"
                        "QMenu::item { font-size: 10pt; color: rgb(225,225,225); border: 3px solid rgb(60,60,60); "
                        "background-color:rgb(89,87,87); padding:16px 16px; margin:2px 2px; }"
                        "QMenu::item:selected { background-color:rgb(235,110,36); }"
                        "QMenu::item:pressed { border: 1px solid rgb(60,60,61); background-color: rgb(220,80,6); }");

    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(16);
    font.setBold(true);

    QSpinBox* width_spinBox = new QSpinBox(menu);
    width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px;}");
    width_spinBox->setFont(font);
    width_spinBox->setRange(0, 1000);
    width_spinBox->setPrefix("w: ");
    width_spinBox->setSuffix(" mm");
    width_spinBox->setSingleStep(1);
    width_spinBox->setValue(2 * abs(m_edge.x()));
    connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        m_edge.setX(v/2);
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QSpinBox* height__spinBox = new QSpinBox(menu);
    height__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px;}");
    height__spinBox->setFont(font);
    height__spinBox->setRange(0, 1000);
    height__spinBox->setPrefix("h: ");
    height__spinBox->setSuffix(" mm");
    height__spinBox->setSingleStep(1);
    height__spinBox->setValue(2 * abs(m_edge.y()));
    connect(height__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        m_edge.setY(v/2);
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* width_widgetAction = new QWidgetAction(menu);
    width_widgetAction->setDefaultWidget(width_spinBox);
    menu->addAction(width_widgetAction);

    QWidgetAction* height_widgetAction = new QWidgetAction(menu);
    height_widgetAction->setDefaultWidget(height__spinBox);
    menu->addAction(height_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
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
    BPointItem *point = new BPointItem(this, m_another_edge, BPointItem::Special);
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

BPolygon::BPolygon(ItemType type)
    : BGraphicsItem(QPointF(0,0), QPointF(0,0), type)
{
    is_create_finished = false;
}

void BPolygon::pushPoint(QPointF p, QList<QPointF> list, bool isCenter)
{
    if (!is_create_finished) {
        m_center = getCentroid(list);
        getMaxLength();

        if (isCenter) {
            m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
            m_pointList.setRandColor();
            is_create_finished = true;
        } else {
            BPointItem *point = new BPointItem(this, p, BPointItem::Edge);
            point->setParentItem(this);
            m_pointList.append(point);
            m_pointList.setColor(QColor(0, 255, 0));
        }

        this->update();
    }
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

    if (is_create_finished) {
        for (int i = 1; i < m_pointList.size() - 1; i++)
        {
            painter->drawLine(m_pointList.at(i-1)->getPoint(), m_pointList.at(i)->getPoint());
        }

        painter->drawLine(m_pointList.at(m_pointList.size()-2)->getPoint(), m_pointList.at(0)->getPoint());
    } else {
        for (int i = 1; i < m_pointList.size(); i++)
        {
            painter->drawLine(m_pointList.at(i-1)->getPoint(), m_pointList.at(i)->getPoint());
        }
    }
}

//------------------------------------------------------------------------------

BRound_End_Rectangle::BRound_End_Rectangle(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BRectangle(x, y, width, height, type) {}

QRectF BRound_End_Rectangle::boundingRect() const
{
    return QRectF(m_center.x() - m_edge.x() - m_edge.y(),
                  m_center.y() - m_edge.y(),
                  m_edge.x() * 2 + m_edge.y() * 2,
                  m_edge.y() * 2);
}

void BRound_End_Rectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QPointF left_top, left_bottom, right_top, right_bottom;

    if ( m_edge.x() >= 0 && m_edge.y() < 0 )
    {
        left_top = QPointF(m_center.x() - m_edge.x() + m_edge.y(), m_edge.y());
        left_bottom = QPointF(m_center.x() - m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_top = QPointF(m_center.x() + m_edge.x() - m_edge.y(), m_edge.y());
        right_bottom = QPointF(m_center.x() + m_edge.x() - m_edge.y(), (-1) * m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() < 0 )
    {
        left_top = QPointF(m_center.x() + m_edge.x() + m_edge.y(), m_edge.y());
        left_bottom = QPointF(m_center.x() + m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_top = QPointF(m_center.x() - m_edge.x() - m_edge.y(), m_edge.y());
        right_bottom = QPointF(m_center.x() - m_edge.x() - m_edge.y(), (-1) * m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() >= 0 )
    {
        left_top = QPointF(m_center.x() + m_edge.x() - m_edge.y(), (-1) * m_edge.y());
        left_bottom = QPointF(m_center.x() + m_edge.x() - m_edge.y(), m_edge.y());
        right_top = QPointF(m_center.x() - m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_bottom = QPointF(m_center.x() - m_edge.x() + m_edge.y(), m_edge.y());
    }
    else if ( m_edge.x() >= 0 && m_edge.y() >=0 )
    {
        left_top = QPointF(m_center.x() - m_edge.x() - m_edge.y(), (-1) * m_edge.y());
        left_bottom = QPointF(m_center.x() - m_edge.x() - m_edge.y(), m_edge.y());
        right_top = QPointF(m_center.x() + m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_bottom = QPointF(m_center.x() + m_edge.x() + m_edge.y(), m_edge.y());
    }

    int radius = abs(m_edge.y());
    QPointF deltax(radius, 0);
    QPointF deltay(0, radius);

    painter->drawLine(left_top + deltax, right_top - deltax);
    painter->drawLine(left_bottom + deltax, right_bottom - deltax);
    painter->drawLine(left_top + deltay, left_bottom - deltay);
    painter->drawLine(right_top + deltay, right_bottom - deltay);

    painter->drawArc(QRectF(left_top, QSizeF(radius*2, radius*2)), -180 * 16, -90 * 16);
    painter->drawArc(QRectF(left_bottom-deltay*2, QSizeF(radius*2, radius*2)), -90 * 16, -90 * 16);
    painter->drawArc(QRectF(right_top-deltax*2, QSizeF(radius*2, radius*2)), 0 * 16, 90 * 16);
    painter->drawArc(QRectF(right_bottom-deltax*2-deltay*2, QSizeF(radius*2, radius*2)), 0 * 16, -90 * 16);
}

//------------------------------------------------------------------------------

BRounded_Rectangle::BRounded_Rectangle(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BRectangle(x, y, width, height, type)
{
    m_another_edge.setX( m_edge.x() );
    m_another_edge.setY( (-1) * m_edge.y() );

    BPointItem *point = new BPointItem(this, m_another_edge, BPointItem::Special);
    point->setParentItem(this);
    point->setCursor(Qt::SizeAllCursor);
    m_pointList.append(point);
    m_pointList.setRandColor();

    updateRadius();
}

void BRounded_Rectangle::updateRadius()
{
    qreal dx = 0;
    qreal dy = 0;
    qreal absX = abs(m_edge.x());
    qreal absY = abs(m_edge.y());

    if ( m_another_edge.x() >= 0 && m_another_edge.y() < 0 )
    {
        dx = absX - m_another_edge.x();
        dy = absY + m_another_edge.y();
    }
    else if ( m_another_edge.x() < 0 && m_another_edge.y() < 0 )
    {
        dx = absX + m_another_edge.x();
        dy = absY + m_another_edge.y();
    }
    else if ( m_another_edge.x() < 0 && m_another_edge.y() >= 0 )
    {
        dx = absX + m_another_edge.x();
        dy = absY - m_another_edge.y();
    }
    else if ( m_another_edge.x() >= 0 && m_another_edge.y() >= 0 )
    {
        dx = absX - m_another_edge.x();
        dy = absY - m_another_edge.y();
    }

    m_radius = dx >= dy ? dx : dy;
}

void BRounded_Rectangle::updateAnotherEdge(QPointF p)
{
    qreal retX = 0;
    qreal retY = 0;

    if ( p.x() == m_another_edge.x() ) {
        retX = m_edge.x();
        retY = (-1) * m_edge.y() + m_radius;
    } else {
        retX = m_edge.x() - m_radius;
        retY = (-1) * m_edge.y();
    }

    m_another_edge.setX(retX);
    m_another_edge.setY(retY);
    m_pointList.at(2)->setPoint(m_another_edge);
}

qreal BRounded_Rectangle::getRadius()
{
    return m_radius;
}

QPointF BRounded_Rectangle::getAnotherEdge()
{
    return m_another_edge;
}

void BRounded_Rectangle::setAnotherEdge(QPointF p)
{
    m_another_edge = p;
}

void BRounded_Rectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_edge.x(), m_center.y() - m_edge.y(), m_edge.x() * 2, m_edge.y() * 2);
    painter->drawRoundedRect(ret, m_radius, m_radius);
}

//------------------------------------------------------------------------------
