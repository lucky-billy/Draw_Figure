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

// 自定义功能图元 - 圆点
class BPointItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT

public:
    BPointItem(QAbstractGraphicsShapeItem* parent, QPointF p, bool type);

    QPointF getPoint() const { return m_point; }

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event) override;

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPointF m_point;
    bool isCenter;
};

// 圆点容器
class BPointItemList: public QList<BPointItem *>
{
public:
    BPointItemList() { this->clear(); }

    QList<QPointF> getPoints() const;
    void setColor(const QColor color);
    void setRandColor();
    void setVisible(bool visible);
};

#endif // BPOINTITEM_H
