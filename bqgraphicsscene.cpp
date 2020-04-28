#include "bqgraphicsscene.h"

BQGraphicsScene::BQGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    start_Create_BPolygon = false;
}

bool BQGraphicsScene::getJudgement()
{
    return start_Create_BPolygon;
}

void BQGraphicsScene::setJudgement(bool jud)
{
    m_pointList.clear();
    start_Create_BPolygon = jud;
}

void BQGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (start_Create_BPolygon) {
        switch ( event->buttons() )
        {
        case Qt::LeftButton: {
            m_pointList.append(QPointF(event->scenePos().x(), event->scenePos().y()));
        } break;
        case Qt::RightButton: {
            if (m_pointList.size() >= 3) {
                emit createFinished(m_pointList);
                setJudgement(false);
            }
        } break;
        default: break;
        }
    } else {
        QGraphicsScene::mousePressEvent(event);
    }
}
