#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("自定义图形"));
    setMaximumSize(1024, 768);
    setMinimumSize(1027, 768);

    m_scene.setBackgroundBrush(Qt::gray);
    ui->graphicsView->setScene(&m_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_circleBtn_clicked()
{
    BCircle *m_circle = new BCircle(0, 0, 50, BGraphicsItem::ItemType::Circle);
    m_scene.addItem(m_circle);
}

void MainWindow::on_ellipseBtn_clicked()
{
    BEllipse *m_ellipse = new BEllipse(0, 0, 120, 80, BGraphicsItem::ItemType::Ellipse);
    m_scene.addItem(m_ellipse);
}

void MainWindow::on_conCircleBtn_clicked()
{
    BConcentricCircle *m_conCircle = new BConcentricCircle(0, 0, 50, 80, BGraphicsItem::ItemType::Concentric_Circle);
    m_scene.addItem(m_conCircle);
}

void MainWindow::on_pieBtn_clicked()
{
    BPie *m_pie = new BPie(0, 0, 80, 30, BGraphicsItem::ItemType::Pie);
    m_scene.addItem(m_pie);
}

void MainWindow::on_chordBtn_clicked()
{
    BChord *m_chord = new BChord(0, 0, 80, 30, BGraphicsItem::ItemType::Chord);
    m_scene.addItem(m_chord);
}

void MainWindow::on_squareBtn_clicked()
{
    BSquare *m_square = new BSquare(0, 0, 60, BGraphicsItem::ItemType::Square);
    m_scene.addItem(m_square);
}

void MainWindow::on_rectangleBtn_clicked()
{
    BRectangle *m_rectangle = new BRectangle(0, 0, 80, 60, BGraphicsItem::ItemType::Rectangle);
    m_scene.addItem(m_rectangle);
}

void MainWindow::on_polygonBtn_clicked()
{
    m_scene.startCreate();
    setBtnEnabled(false);
    BPolygon *m_polygon = new BPolygon(BGraphicsItem::ItemType::Polygon);
    m_scene.addItem(m_polygon);

    connect(&m_scene, SIGNAL(updatePoint(QPointF, QList<QPointF>, bool)), m_polygon, SLOT(pushPoint(QPointF, QList<QPointF>, bool)));
    connect(&m_scene, &BQGraphicsScene::createFinished, [=](){ setBtnEnabled(true); });
}

void MainWindow::on_rnRecBtn_clicked()
{
    BRound_End_Rectangle *m_round_end_Rectangle = new BRound_End_Rectangle(0, 0, 80, 60, BGraphicsItem::ItemType::Round_End_Rectangle);
    m_scene.addItem(m_round_end_Rectangle);
}

void MainWindow::on_roundRecBtn_clicked()
{
    BRounded_Rectangle *m_rounded_Rectangle = new BRounded_Rectangle(0, 0, 80, 60, BGraphicsItem::ItemType::Rounded_Rectangle);
    m_scene.addItem(m_rounded_Rectangle);
}

void MainWindow::on_clearBtn_clicked()
{
    m_scene.clear();
}

void MainWindow::on_saveBtn_clicked()
{
    m_scene.saveItemToConfig();
}

void MainWindow::on_loadBtn_clicked()
{
    m_scene.loadItemToScene();
}

void MainWindow::setBtnEnabled(bool enable)
{
    ui->circleBtn->setEnabled(enable);
    ui->ellipseBtn->setEnabled(enable);
    ui->conCircleBtn->setEnabled(enable);
    ui->pieBtn->setEnabled(enable);
    ui->chordBtn->setEnabled(enable);
    ui->squareBtn->setEnabled(enable);
    ui->rectangleBtn->setEnabled(enable);
    ui->roundRecBtn->setEnabled(enable);
    ui->rnRecBtn->setEnabled(enable);
    ui->clearBtn->setEnabled(enable);
    ui->saveBtn->setEnabled(enable);
    ui->loadBtn->setEnabled(enable);
}
