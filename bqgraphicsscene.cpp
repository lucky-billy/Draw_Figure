#include "bqgraphicsscene.h"
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QDebug>

BQGraphicsScene::BQGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    is_creating_BPolygon = false;
}

void BQGraphicsScene::startCreate()
{
    is_creating_BPolygon = true;
    m_list.clear();
}

void BQGraphicsScene::saveItemToConfig()
{
    QString fileName = QDir::currentPath() + "/item.ini";
    if ( QFile::exists(fileName) ) {
        QFile(fileName).remove();
    }

    QSettings setting(fileName, QSettings::IniFormat);

    QList<QGraphicsItem *> list = this->items();

    setting.beginWriteArray("itemList");
    for ( int i = 0; i < list.size(); ++i )
    {
        setting.setArrayIndex(i);

        BGraphicsItem *item = static_cast<BGraphicsItem *>(list[i]);
        BGraphicsItem::ItemType type = item->getType();
        setting.setValue("type", (int)type);
        setting.setValue("x", item->x());
        setting.setValue("y", item->y());

        switch (type)
        {
        case BGraphicsItem::ItemType::Circle: {
            BCircle *circle = static_cast<BCircle *>(item);
            setting.setValue("radius", circle->m_radius);
        } break;
        case BGraphicsItem::ItemType::Ellipse: {
            BEllipse *ellipse = static_cast<BEllipse *>(item);
            setting.setValue("edgeX", ellipse->getEdge().x());
            setting.setValue("edgeY", ellipse->getEdge().y());
        } break;
        case BGraphicsItem::ItemType::Concentric_Circle: {
            BConcentricCircle *conCircle = static_cast<BConcentricCircle *>(item);
            setting.setValue("radius", conCircle->m_radius);
            setting.setValue("radius2", conCircle->m_another_radius);
        } break;
        case BGraphicsItem::ItemType::Pie: {
            BPie *pie = static_cast<BPie *>(item);
            setting.setValue("radius", pie->m_radius);
            setting.setValue("angle", pie->m_angle);
        } break;
        case BGraphicsItem::ItemType::Chord: {
            BChord *chord = static_cast<BChord *>(item);
            setting.setValue("radius", chord->m_radius);
            setting.setValue("angle", chord->m_angle);
        } break;
        case BGraphicsItem::ItemType::Rectangle: {
            BRectangle *rectangle = static_cast<BRectangle *>(item);
            setting.setValue("edgeX", rectangle->getEdge().x());
            setting.setValue("edgeY", rectangle->getEdge().y());
        } break;
        case BGraphicsItem::ItemType::Square: {
            BSquare *square = static_cast<BSquare *>(item);
            setting.setValue("edgeX", square->getEdge().x());
        } break;
        case BGraphicsItem::ItemType::Polygon: {
            BPolygon *polygon = static_cast<BPolygon *>(item);
            BPointItemList list = polygon->m_pointList;
            setting.beginWriteArray("polygon");
            for ( int j = 0; j < list.size() - 1; j++ )
            {
                setting.setArrayIndex(j);
                setting.setValue("edge", list.at(j)->m_point);
            }
            setting.endArray();
        } break;
        case BGraphicsItem::ItemType::Round_End_Rectangle: {
            BRound_End_Rectangle *round_End_Rectangle = static_cast<BRound_End_Rectangle *>(item);
            setting.setValue("edgeX", round_End_Rectangle->getEdge().x());
            setting.setValue("edgeY", round_End_Rectangle->getEdge().y());
        } break;
        case BGraphicsItem::ItemType::Rounded_Rectangle: {
            BRounded_Rectangle *rounded_Rectangle = static_cast<BRounded_Rectangle *>(item);
            setting.setValue("edgeX", rounded_Rectangle->getEdge().x());
            setting.setValue("edgeY", rounded_Rectangle->getEdge().y());
            setting.setValue("radius", rounded_Rectangle->m_radius);
        } break;
        default: break;
        }
    }
    setting.endArray();
}

void BQGraphicsScene::loadItemToScene()
{
    this->clear();

    QString fileName = QDir::currentPath() + "/item.ini";
    QSettings setting(fileName, QSettings::IniFormat);

    int size = setting.beginReadArray("itemList");
    for ( int i = 0; i < size; ++i )
    {
        setting.setArrayIndex(i);

        BGraphicsItem::ItemType type = (BGraphicsItem::ItemType)setting.value("type").toInt();

        switch (type)
        {
        case BGraphicsItem::ItemType::Circle: {
            qreal radius = setting.value("radius").toDouble();
            BCircle *m_circle = new BCircle(0, 0, radius, BGraphicsItem::ItemType::Circle);
            m_circle->setX(setting.value("x").toDouble());
            m_circle->setY(setting.value("y").toDouble());
            addItem(m_circle);
        } break;
        case BGraphicsItem::ItemType::Ellipse: {
            qreal edgeX = setting.value("edgeX").toDouble();
            qreal edgeY = setting.value("edgeY").toDouble();
            BEllipse *m_ellipse = new BEllipse(0, 0, edgeX*2, edgeY*2, BGraphicsItem::ItemType::Ellipse);
            m_ellipse->setX(setting.value("x").toDouble());
            m_ellipse->setY(setting.value("y").toDouble());
            addItem(m_ellipse);
        } break;
        case BGraphicsItem::ItemType::Concentric_Circle: {
            qreal radius = setting.value("radius").toDouble();
            qreal radius2 = setting.value("radius2").toDouble();
            BConcentricCircle *m_conCircle = new BConcentricCircle(0, 0, radius, radius2, BGraphicsItem::ItemType::Concentric_Circle);
            m_conCircle->setX(setting.value("x").toDouble());
            m_conCircle->setY(setting.value("y").toDouble());
            addItem(m_conCircle);
        } break;
        case BGraphicsItem::ItemType::Pie: {
            qreal radius = setting.value("radius").toDouble();
            qreal angle = setting.value("angle").toDouble();
            BPie *m_pie = new BPie(0, 0, radius, angle, BGraphicsItem::ItemType::Pie);
            m_pie->setX(setting.value("x").toDouble());
            m_pie->setY(setting.value("y").toDouble());
            addItem(m_pie);
        } break;
        case BGraphicsItem::ItemType::Chord: {
            qreal radius = setting.value("radius").toDouble();
            qreal angle = setting.value("angle").toDouble();
            BChord *m_chord = new BChord(0, 0, radius, angle, BGraphicsItem::ItemType::Chord);
            m_chord->setX(setting.value("x").toDouble());
            m_chord->setY(setting.value("y").toDouble());
            addItem(m_chord);
        } break;
        case BGraphicsItem::ItemType::Rectangle: {
            qreal edgeX = setting.value("edgeX").toDouble();
            qreal edgeY = setting.value("edgeY").toDouble();
            BRectangle *m_rectangle = new BRectangle(0, 0, edgeX*2, edgeY*2, BGraphicsItem::ItemType::Rectangle);
            m_rectangle->setX(setting.value("x").toDouble());
            m_rectangle->setY(setting.value("y").toDouble());
            addItem(m_rectangle);
        } break;
        case BGraphicsItem::ItemType::Square: {
            qreal edgeX = setting.value("edgeX").toDouble();
            BSquare *m_square = new BSquare(0, 0, edgeX*2, BGraphicsItem::ItemType::Square);
            m_square->setX(setting.value("x").toDouble());
            m_square->setY(setting.value("y").toDouble());
            addItem(m_square);
        } break;
        case BGraphicsItem::ItemType::Polygon: {
            startCreate();
            BPolygon *m_polygon = new BPolygon(BGraphicsItem::ItemType::Polygon);
            addItem(m_polygon);

            int count = setting.beginReadArray("polygon");
            QList<QPointF> list;
            for ( int j = 0; j < count; ++j )
            {
                setting.setArrayIndex(j);
                QPointF edge = setting.value("edge").toPointF();
                list.append(edge);
                m_polygon->pushPoint(edge, list, false);
            }
            setting.endArray();

            m_polygon->setX(setting.value("x").toDouble());
            m_polygon->setY(setting.value("y").toDouble());
            m_polygon->pushPoint(QPointF(0,0), list, true);
            is_creating_BPolygon = false;
        } break;
        case BGraphicsItem::ItemType::Round_End_Rectangle: {
            qreal edgeX = setting.value("edgeX").toDouble();
            qreal edgeY = setting.value("edgeY").toDouble();
            BRound_End_Rectangle *m_round_end_Rectangle = new BRound_End_Rectangle(0, 0, edgeX*2, edgeY*2,
                                                                                   BGraphicsItem::ItemType::Round_End_Rectangle);
            m_round_end_Rectangle->setX(setting.value("x").toDouble());
            m_round_end_Rectangle->setY(setting.value("y").toDouble());
            addItem(m_round_end_Rectangle);
        } break;
        case BGraphicsItem::ItemType::Rounded_Rectangle: {
            qreal edgeX = setting.value("edgeX").toDouble();
            qreal edgeY = setting.value("edgeY").toDouble();
            qreal radius = setting.value("radius").toDouble();
            BRounded_Rectangle *m_rounded_Rectangle = new BRounded_Rectangle(0, 0, edgeX*2, edgeY*2,
                                                                             BGraphicsItem::ItemType::Rounded_Rectangle);
            m_rounded_Rectangle->m_radius = radius;
            m_rounded_Rectangle->setX(setting.value("x").toDouble());
            m_rounded_Rectangle->setY(setting.value("y").toDouble());
            addItem(m_rounded_Rectangle);
        } break;
        default: break;
        }
    }
    setting.endArray();
}

void BQGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (is_creating_BPolygon) {
        QPointF p(event->scenePos().x(), event->scenePos().y());

        switch ( event->buttons() )
        {
        case Qt::LeftButton: {
            m_list.push_back(p);
            emit updatePoint(p, m_list, false);
        } break;
        case Qt::RightButton: {
            if (m_list.size() >= 3) {
                emit updatePoint(p, m_list, true);
                emit createFinished();
                is_creating_BPolygon = false;
                m_list.clear();
            }
        } break;
        default: break;
        }
    } else {
        QGraphicsScene::mousePressEvent(event);
    }
}
