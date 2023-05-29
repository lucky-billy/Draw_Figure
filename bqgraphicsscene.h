#ifndef BQGRAPHICSSCENE_H
#define BQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "bpointitem.h"
#include "bqgraphicsitem.h"

class BQGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    BQGraphicsScene(QObject *parent = nullptr);

    void startCreate();
    void saveItemToConfig();
    void loadItemToScene();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void updatePoint(QPointF p, QList<QPointF> list, bool isCenter);
    void createFinished();

protected:
    QList<QPointF> m_list;
    bool is_creating_BPolygon;
};

#endif // BQGRAPHICSSCENE_H
