#ifndef BQGRAPHICSITEM_H
#define BQGRAPHICSITEM_H

#include "bpointitem.h"

// 自定义图元 - 基础类
class BGraphicsItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT

public:
    enum ItemType {
        Circle = 0,
        Ellipse,
        Concentric_circle,
        Rectangle,
        Square,
        Polygon
    };

    ItemType getType() { return m_type; }
    QPointF getCenter() { return m_center; }
    void setCenter(QPointF p) { m_center = p; }

protected:
    BGraphicsItem(QPointF center, ItemType type);

    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

protected:
    QPointF m_center;
    ItemType m_type;

    QList<BGraphicsItem *> m_itemList;
    BPointItemList m_pointList;

    QPen m_pen_isSelected;
    QPen m_pen_noSelected;
};

//------------------------------------------------------------------------------

// 圆
class BCircle : public BGraphicsItem
{
public:
    BCircle(QPointF center, QPointF edge, ItemType type);

    void updateRadius() { m_radius = sqrt(pow(m_center.x() - m_edge.x(), 2) + pow(m_center.y() - m_edge.y(), 2)); }
    QPointF getEdge() { return m_edge; }
    void setEdge(QPointF p) { m_edge = p; }

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    QPointF m_edge;
    qreal m_radius;
};

#endif // BQGRAPHICSITEM_H
