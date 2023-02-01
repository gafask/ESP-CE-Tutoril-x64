#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>


// Расширяем класс QGraphicsView
class MyGraphicView : public QGraphicsView
{

public:
    explicit MyGraphicView(QWidget *parent = 0);
    ~MyGraphicView();



private slots:
    void slotAlarmTimer();

public:
    QGraphicsScene      *scene = new QGraphicsScene();;     // Объявляем сцену для отрисовки

    QGraphicsItemGroup  *group_1;
    QGraphicsItemGroup  *group_2;
    QGraphicsItemGroup  *group_3;
    QGraphicsItemGroup  *group_4;
    QGraphicsItemGroup  *group_5;
    QGraphicsItemGroup  *group_6;

    QTimer              *timer;

private:
    void resizeEvent(QResizeEvent *event);


public slots:
    void get_pos(int x, int y,int nomer , float ugol_povorota , float ugol_x , float ugol_y);

};


