#include "mygraphicview.h"
#include <Windows.h>
MyGraphicView::MyGraphicView(QWidget *parent)
    : QGraphicsView(parent)
{


    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали


    this->setScene(scene);          // Устанавливаем сцену в виджет

    group_1 = new QGraphicsItemGroup(); // Инициализируем первую группу элементов
    group_2 = new QGraphicsItemGroup(); // Инициализируем первую группу элементов
    group_3 = new QGraphicsItemGroup();
    group_4 = new QGraphicsItemGroup();
    group_5 = new QGraphicsItemGroup();
    group_6 = new QGraphicsItemGroup();

    scene->addItem(group_1);
    scene->addItem(group_2);
    scene->addItem(group_3);
    scene->addItem(group_4);
    scene->addItem(group_5);
    scene->addItem(group_6);

    timer = new QTimer();
    timer->setSingleShot(true);

    // Подключаем СЛОТ для отрисовки к таймеру
    connect(timer, &QTimer::timeout, this, &MyGraphicView::slotAlarmTimer);
    timer->start(50);                   // Стартуем таймер на 50 миллисекунд
}

MyGraphicView::~MyGraphicView(){}

void MyGraphicView::slotAlarmTimer()
{
    scene->setSceneRect(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

    group_1->addToGroup(scene->addRect(0,0, 20, 20, QPen(Qt::green)));
    group_2->addToGroup(scene->addRect(0,0, 20, 20, QPen(Qt::green)));
    group_3->addToGroup(scene->addRect(0,0, 20, 20, QPen(Qt::green)));
    group_4->addToGroup(scene->addRect(0,0, 20, 20, QPen(Qt::green)));
    group_5->addToGroup(scene->addRect(0,0, 20, 20, QPen(Qt::green)));

    group_6->addToGroup(scene->addLine(0,0, 0, -500, QPen(Qt::green)));

}

void MyGraphicView::resizeEvent(QResizeEvent *event)
{
    timer->start(50);   // Как только событие произошло стартуем таймер для отрисовки
    QGraphicsView::resizeEvent(event);  // Запускаем событие родителького класса
}




void MyGraphicView::get_pos(int x, int y,int nomer , float ugol_povorota , float ugol_x , float ugol_y)
{
    x=x-15;
    if(nomer == 0)
        group_1->setPos(x ,y );
    if(nomer == 1)
        group_2->setPos(x,y);
    if(nomer == 2)
        group_3->setPos(x,y);
    if(nomer == 3)
        group_4->setPos(x,y);
    if(nomer == 4)
        group_5->setPos(x,y);
    group_6->setPos(ugol_x,ugol_y);
    group_6->setRotation(ugol_povorota);
}
