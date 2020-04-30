#include "bpointitem.h"
#include "bqgraphicsitem.h"

BPointItem::BPointItem(QAbstractGraphicsShapeItem* parent, QPointF p, PointType type)
    : QAbstractGraphicsShapeItem(parent)
    , m_point(p)
    , m_type(type)
{
    this->setPos(m_point);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);

    switch (type) {
    case Center:
        this->setCursor(Qt::OpenHandCursor);
        break;
    case Edge:
        this->setCursor(Qt::PointingHandCursor);
        break;
    case Special:
        this->setCursor(Qt::PointingHandCursor);
        break;
    default: break;
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
    this->setPos(m_point);

    switch (m_type) {
    case Center:
        painter->drawEllipse(-4, -4, 8, 8);
        break;
    case Edge:
        painter->drawRect(QRectF(-4, -4, 8, 8));
        break;
    case Special:
        painter->drawRect(QRectF(-4, -4, 8, 8));
        break;
    default: break;
    }
}

void BPointItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->buttons() == Qt::LeftButton ) {
        qreal dx = event->scenePos().x() - event->lastScenePos().x();
        qreal dy = event->scenePos().y() - event->lastScenePos().y();

        BGraphicsItem* item = static_cast<BGraphicsItem *>(this->parentItem());
        BGraphicsItem::ItemType itemType = item->getType();

        switch (m_type) {
        case Center: {
            item->moveBy(dx, dy);
            this->scene()->update();
        } break;
        case Edge: {
            if (itemType != BGraphicsItem::ItemType::Rounded_Rectangle) {
                m_point = this->mapToParent( event->pos() );
                this->setPos(m_point);
                this->scene()->update();
            }

            switch (itemType) {
            case BGraphicsItem::ItemType::Ellipse: {
                BEllipse *ellipse = dynamic_cast<BEllipse *>(item);
                ellipse->setEdge(m_point);
            } break;
            case BGraphicsItem::ItemType::Circle: {
                BCircle *circle = dynamic_cast<BCircle *>(item);
                circle->setEdge(m_point);
                circle->updateRadius();
            } break;
            case BGraphicsItem::ItemType::Concentric_Circle: {
                BCircle *circle = dynamic_cast<BCircle *>(item);
                circle->setEdge(m_point);
                circle->updateRadius();
            } break;
            case BGraphicsItem::ItemType::Pie: {
                BPie *pie = dynamic_cast<BPie *>(item);
                pie->setEdge(m_point);
                pie->updateRadius();
                pie->updateAngle();
            } break;
            case BGraphicsItem::ItemType::Chord: {
                BChord *chord = dynamic_cast<BChord *>(item);
                chord->setEdge(m_point);
                chord->updateRadius();
                chord->updateEndAngle();
            } break;
            case BGraphicsItem::ItemType::Rectangle: {
                BRectangle *rectangle = dynamic_cast<BRectangle *>(item);
                rectangle->setEdge(m_point);
            } break;
            case BGraphicsItem::ItemType::Square: {
                BSquare *square = dynamic_cast<BSquare *>(item);
                qreal ret = m_point.x() > m_point.y() ? m_point.x() : m_point.y();
                m_point.setX(ret);
                m_point.setY(ret);
                square->setEdge(m_point);
            } break;
            case BGraphicsItem::ItemType::Polygon: {
                BPolygon *polygon = dynamic_cast<BPolygon *>(item);
                polygon->updatePolygon(QPointF(event->lastScenePos().x(), event->lastScenePos().y()),
                                       QPointF(event->scenePos().x(), event->scenePos().y()));
            } break;
            case BGraphicsItem::ItemType::Round_End_Rectangle: {
                BRound_End_Rectangle *round_End_Rectangle = dynamic_cast<BRound_End_Rectangle *>(item);
                round_End_Rectangle->setEdge(m_point);
            } break;
            case BGraphicsItem::ItemType::Rounded_Rectangle: {
                BRounded_Rectangle *rounded_Rectangle = dynamic_cast<BRounded_Rectangle *>(item);

                m_point = this->mapToParent( event->pos() );

                if (m_point.x() <= rounded_Rectangle->getRadius()) {
                    m_point.setX( rounded_Rectangle->getRadius() );
                }
                if (m_point.y() <= rounded_Rectangle->getRadius()) {
                    m_point.setY( rounded_Rectangle->getRadius() );
                }

                this->setPos(m_point);
                this->scene()->update();

                QPointF p = rounded_Rectangle->getEdge();
                rounded_Rectangle->setEdge(m_point);
                rounded_Rectangle->updateAnotherEdge(p);
            } break;
            default: break;
            }
        } break;
        case Special: {
            switch (itemType) {
            case BGraphicsItem::ItemType::Concentric_Circle: {
                m_point = this->mapToParent( event->pos() );
                this->setPos(m_point);
                this->scene()->update();

                BConcentricCircle *conCircle = dynamic_cast<BConcentricCircle *>(item);
                conCircle->setAnotherEdge(m_point);
                conCircle->updateOtherRadius();
            } break;
            case BGraphicsItem::ItemType::Rounded_Rectangle: {
                BRounded_Rectangle *rounded_Rectangle = dynamic_cast<BRounded_Rectangle *>(item);

                qreal retX = 0;
                qreal retY = 0;

                if ( rounded_Rectangle->getEdge().x() == rounded_Rectangle->getAnotherEdge().x() &&
                     (-1) * rounded_Rectangle->getEdge().y() != rounded_Rectangle->getAnotherEdge().y() )
                {
                    retX = rounded_Rectangle->getAnotherEdge().x();
                    retY = rounded_Rectangle->getAnotherEdge().y() + dy;
                }
                else if ( rounded_Rectangle->getEdge().x() != rounded_Rectangle->getAnotherEdge().x() &&
                          (-1) * rounded_Rectangle->getEdge().y() == rounded_Rectangle->getAnotherEdge().y() )
                {
                    retX = rounded_Rectangle->getAnotherEdge().x() + dx;
                    retY = rounded_Rectangle->getAnotherEdge().y();
                }
                else if ( rounded_Rectangle->getEdge().x() == rounded_Rectangle->getAnotherEdge().x() &&
                          (-1) * rounded_Rectangle->getEdge().y() == rounded_Rectangle->getAnotherEdge().y() )
                {
                    if ( abs(dx) >= abs(dy) ) {
                        retX = rounded_Rectangle->getAnotherEdge().x() + dx;
                        retY = rounded_Rectangle->getAnotherEdge().y();
                    } else {
                        retX = rounded_Rectangle->getAnotherEdge().x();
                        retY = rounded_Rectangle->getAnotherEdge().y() + dy;
                    }
                }

                if ( retX > rounded_Rectangle->getEdge().x() ) {
                    retX = rounded_Rectangle->getEdge().x();
                } else if ( retX < 0 ) {
                    retX = 0;
                }

                if ( retY < (-1) * rounded_Rectangle->getEdge().y() ) {
                    retY = (-1) * rounded_Rectangle->getEdge().y();
                } else if ( retY > 0 ) {
                    retY = 0;
                }

                m_point.setX(retX);
                m_point.setY(retY);
                this->setPos(m_point);
                this->scene()->update();

                rounded_Rectangle->setAnotherEdge(m_point);
                rounded_Rectangle->updateRadius();
            }
            default: break;
            }
        } break;
        default: break;
        }
    }
}

//------------------------------------------------------------------------------

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
