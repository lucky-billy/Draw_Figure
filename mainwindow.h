#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_circleBtn_clicked();

    void on_ellipseBtn_clicked();

    void on_conCircleBtn_clicked();

    void on_squareBtn_clicked();

    void on_rectangleBtn_clicked();

    void on_polygonBtn_clicked();

    void on_clearBtn_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene m_scene;
};
#endif // MAINWINDOW_H
