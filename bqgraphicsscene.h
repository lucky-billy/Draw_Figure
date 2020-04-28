#ifndef BQGRAPHICSSCENE_H
#define BQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "bpointitem.h"

class BQGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    BQGraphicsScene(QObject *parent = nullptr);

    bool getJudgement();
    void setJudgement(bool jud);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void createFinished(QList<QPointF> m_pointList);

protected:
    bool start_Create_BPolygon;
    QList<QPointF> m_pointList;
};

#endif // BQGRAPHICSSCENE_H
