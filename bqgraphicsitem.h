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
        Concentric_Circle,
        Rectangle,
        Square,
        Polygon,
        Rounded_Rectangle,
        Round_End_Rectangle
    };

    QPointF getCenter() { return m_center; }
    void setCenter(QPointF p) { m_center = p; }
    ItemType getType() { return m_type; }

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

// 椭圆
class BEllipse : public BGraphicsItem
{
public:
    BEllipse(qreal x, qreal y, qreal width, qreal height, ItemType type);

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
};

//------------------------------------------------------------------------------

// 圆
class BCircle : public BEllipse
{
public:
    BCircle(qreal x, qreal y, qreal radius, ItemType type);

    void updateRadius() { m_radius = sqrt(pow(m_center.x() - m_edge.x(), 2) + pow(m_center.y() - m_edge.y(), 2)); }

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

protected:
    qreal m_radius;
};

//------------------------------------------------------------------------------

// 同心圆

//------------------------------------------------------------------------------

// 矩形
class BRectangle : public BGraphicsItem
{
public:
    BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type);

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
};

//------------------------------------------------------------------------------

// 正方形
class BSquare : public BRectangle
{
public:
    BSquare(qreal x, qreal y, qreal width, ItemType type);
};

//------------------------------------------------------------------------------

// 多边形

//------------------------------------------------------------------------------

// 圆角矩形

//------------------------------------------------------------------------------

// 圆端矩形

#endif // BQGRAPHICSITEM_H
