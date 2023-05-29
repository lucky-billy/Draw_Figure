#ifndef BPOINTITEM_H
#define BPOINTITEM_H

#include <QObject>
#include <QAbstractGraphicsShapeItem>
#include <QPointF>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QKeyEvent>
#include <QList>

// 自定义功能图元 - 点
class BPointItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT

public:
    enum PointType {
        Center = 0, // 中心点
        Edge,       // 边缘点（可拖动改变图形的形状、大小）
        Special     // 特殊功能点
    };

    BPointItem(QAbstractGraphicsShapeItem* parent, QPointF p, PointType type);

    QPointF getPoint() { return m_point; }
    void setPoint(QPointF p) { m_point = p; }

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
    QPointF m_point;
    PointType m_type;
};

//------------------------------------------------------------------------------

// 存放点的容器
class BPointItemList: public QList<BPointItem *>
{
public:
    void setRandColor();
    void setColor(const QColor color);
    void setVisible(bool visible);
};

#endif // BPOINTITEM_H
