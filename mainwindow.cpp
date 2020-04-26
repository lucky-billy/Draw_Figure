#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include "bqgraphicsitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scene.setBackgroundBrush(Qt::gray);
    ui->graphicsView->setScene(&m_scene);

    m_pen.setColor(QColor(0, 160, 230));
    m_pen.setWidth(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_circleBtn_clicked()
{
    QGraphicsEllipseItem *m_ellipseItem = new QGraphicsEllipseItem();
    m_ellipseItem->setRect(0, 0, 50, 50);
    m_ellipseItem->setPen(m_pen);
    m_ellipseItem->setBrush(QBrush(QColor(0, 0, 0, 0)));
    m_ellipseItem->setFlag(QGraphicsItem::ItemIsMovable);
    m_scene.addItem(m_ellipseItem);
}

void MainWindow::on_ellipseBtn_clicked()
{
    QGraphicsEllipseItem *m_ellipseItem = new QGraphicsEllipseItem();
    m_ellipseItem->setRect(0, 0, 80, 60);
    m_ellipseItem->setPen(m_pen);
    m_ellipseItem->setBrush(QBrush(QColor(0, 0, 0, 0)));
    m_ellipseItem->setFlag(QGraphicsItem::ItemIsMovable);
    m_scene.addItem(m_ellipseItem);
}

void MainWindow::on_conCircleBtn_clicked()
{
//    BCircle *m_circle = new BCircle(0, 0, 80, 60);
//    m_circle->setPen(m_pen);
//    m_circle->setBrush(QBrush(QColor(0, 0, 0, 0)));
//    m_circle->setFlag(QGraphicsItem::ItemIsMovable);
//    m_scene.addItem(m_circle);
}

void MainWindow::on_squareBtn_clicked()
{
    QGraphicsRectItem *m_rectItem = new QGraphicsRectItem();
    m_rectItem->setRect(0, 0, 60, 60);
    m_rectItem->setPen(m_pen);
    m_rectItem->setBrush(QBrush(QColor(0, 0, 0, 0)));
    m_rectItem->setFlag(QGraphicsItem::ItemIsMovable);
    m_scene.addItem(m_rectItem);
}

void MainWindow::on_rectangleBtn_clicked()
{
    QGraphicsRectItem *m_rectItem = new QGraphicsRectItem();
    m_rectItem->setRect(0, 0, 120, 80);
    m_rectItem->setPen(m_pen);
    m_rectItem->setBrush(QBrush(QColor(0, 0, 0, 0)));
    m_rectItem->setFlag(QGraphicsItem::ItemIsMovable);
    m_scene.addItem(m_rectItem);
}

void MainWindow::on_polygonBtn_clicked()
{

}

void MainWindow::on_clearBtn_clicked()
{
    m_scene.clear();
}
