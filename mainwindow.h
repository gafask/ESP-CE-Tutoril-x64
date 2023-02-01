#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsEllipseItem>

#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QLabel>

#include <thread>
#include <mygraphicview.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ESP();
    void potok_ESP_run()
    {
        std::thread t1(&MainWindow::ESP, this);
        t1.detach();
    }
private slots:
    void on_pushButton_start_clicked();
    void on_exit_push_clicked();

public:

    Ui::MainWindow *ui;
    MyGraphicView *myPicture   = new MyGraphicView();


signals:
    void set_pos(int x, int y,int nomer , float ugol_povorota , float ugol_x , float ugol_y);

};
#endif // MAINWINDOW_H



