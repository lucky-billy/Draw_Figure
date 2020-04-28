#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bqgraphicsitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scene.setBackgroundBrush(Qt::gray);
    ui->graphicsView->setScene(&m_scene);

    connect(&m_scene, SIGNAL(createFinished(QList<QPointF>)), this, SLOT(createBPolygon(QList<QPointF>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_circleBtn_clicked()
{
    m_scene.setJudgement(false);
    BCircle *m_circle = new BCircle(0, 0, 50, BGraphicsItem::ItemType::Circle);
    m_scene.addItem(m_circle);
}

void MainWindow::on_ellipseBtn_clicked()
{
    m_scene.setJudgement(false);
    BEllipse *m_ellipse = new BEllipse(0, 0, 120, 80, BGraphicsItem::ItemType::Ellipse);
    m_scene.addItem(m_ellipse);
}

void MainWindow::on_conCircleBtn_clicked()
{
    m_scene.setJudgement(false);
    BConcentricCircle *m_conCircle = new BConcentricCircle(0, 0, 50, 80, BGraphicsItem::ItemType::Concentric_Circle);
    m_scene.addItem(m_conCircle);
}

void MainWindow::on_pieBtn_clicked()
{
    m_scene.setJudgement(false);
    BPie *m_pie = new BPie(0, 0, 80, 30, BGraphicsItem::ItemType::Pie);
    m_scene.addItem(m_pie);
}

void MainWindow::on_chordBtn_clicked()
{
    m_scene.setJudgement(false);
    BChord *m_chord = new BChord(0, 0, 80, 30, BGraphicsItem::ItemType::Chord);
    m_scene.addItem(m_chord);
}

void MainWindow::on_squareBtn_clicked()
{
    m_scene.setJudgement(false);
    BSquare *m_square = new BSquare(0, 0, 60, BGraphicsItem::ItemType::Square);
    m_scene.addItem(m_square);
}

void MainWindow::on_rectangleBtn_clicked()
{
    m_scene.setJudgement(false);
    BRectangle *m_rectangle = new BRectangle(0, 0, 80, 60, BGraphicsItem::ItemType::Rectangle);
    m_scene.addItem(m_rectangle);
}

void MainWindow::on_polygonBtn_clicked()
{
    m_scene.setJudgement(true);
}

void MainWindow::on_roundRecBtn_clicked()
{
    m_scene.setJudgement(false);
}

void MainWindow::on_rnRecBtn_clicked()
{
    m_scene.setJudgement(false);
}

void MainWindow::on_clearBtn_clicked()
{
    m_scene.setJudgement(false);
    m_scene.clear();
}

void MainWindow::on_saveBtn_clicked()
{
    m_scene.setJudgement(false);
}

void MainWindow::on_loadBtn_clicked()
{
    m_scene.setJudgement(false);
}

void MainWindow::createBPolygon(QList<QPointF> m_pointList)
{
    BPolygon *m_polygon = new BPolygon(m_pointList, BGraphicsItem::ItemType::Polygon);
    m_scene.addItem(m_polygon);
}
