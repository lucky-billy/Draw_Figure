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
    BPointItem(QAbstractGraphicsShapeItem* parent, QPointF p, bool type);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF m_point;
    bool isCenter;
};

// 存放点的容器
class BPointItemList: public QList<BPointItem *>
{
public:
    BPointItemList() { this->clear(); }

    void setRandColor();
    void setColor(const QColor color);
    void setVisible(bool visible);
};

#endif // BPOINTITEM_H
