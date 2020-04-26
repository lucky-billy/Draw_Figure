#ifndef BQGRAPHICSITEM_H
#define BQGRAPHICSITEM_H

#include "bpointitem.h"

// 自定义图元 - 基础类
class BGraphicsItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT

public:
    enum ItemType {
        BGraphics_Circle,
        BGraphics_Ellipse,
        BGraphics_ConcentricCircle,
        BGraphics_Rect,
        BGraphics_Square,
        BGraphics_Polygon
    };

protected:
    BGraphicsItem(QGraphicsPixmapItem* parent);

protected:
    QList<BGraphicsItem *> m_itemList;
    BPointItemList m_pointList;
    QPointF center;
};

// 圆
class BCircle : public BGraphicsItem
{
public:
    BCircle(qreal x, qreal y, qreal radius, QGraphicsPixmapItem* parent = nullptr);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

protected:
    qreal radius;
};

#endif // BQGRAPHICSITEM_H
