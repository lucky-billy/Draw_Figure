#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bqgraphicsitem.h"
#include "bqgraphicsscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setBtnEnabled(bool enable);

private slots:
    void on_circleBtn_clicked();

    void on_ellipseBtn_clicked();

    void on_conCircleBtn_clicked();

    void on_pieBtn_clicked();

    void on_chordBtn_clicked();

    void on_squareBtn_clicked();

    void on_rectangleBtn_clicked();

    void on_polygonBtn_clicked();

    void on_rnRecBtn_clicked();

    void on_roundRecBtn_clicked();

    void on_clearBtn_clicked();

    void on_saveBtn_clicked();

    void on_loadBtn_clicked();

private:
    Ui::MainWindow *ui;

    BQGraphicsScene m_scene;
};
#endif // MAINWINDOW_H
