#ifndef BQGRAPHICSVIEW_H
#define BQGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class BQGraphicsView : public QGraphicsView
{
public:
    BQGraphicsView(QWidget *parent = nullptr);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // BQGRAPHICSVIEW_H
