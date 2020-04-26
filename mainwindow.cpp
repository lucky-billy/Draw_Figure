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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_circleBtn_clicked()
{
    BCircle *m_circle = new BCircle(0, 0, 50);
    m_scene.addItem(m_circle);
}

void MainWindow::on_ellipseBtn_clicked()
{

}

void MainWindow::on_conCircleBtn_clicked()
{

}

void MainWindow::on_squareBtn_clicked()
{

}

void MainWindow::on_rectangleBtn_clicked()
{

}

void MainWindow::on_polygonBtn_clicked()
{

}

void MainWindow::on_clearBtn_clicked()
{
    m_scene.clear();
}
