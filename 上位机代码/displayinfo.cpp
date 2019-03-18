#include "displayinfo.h"
#include <QPainter>
#include <QDebug>

DisplayInfo::DisplayInfo(QWidget *parent) : QWidget(parent)
{
    MQ7_ADCV = MQ135_ADCV = PM25_ADCV = 0;
    MQ7_TV = MQ135_TV = PM25_TV = 0;
    PM25_Boom = MQ135_Boom = MQ7_Boom = 6;
    PM25_Threshold = 60;
    MQ7_Threshold = 200;
    MQ135_Threshold = 300;
}
void DisplayInfo::GXData()
{
    if (PM25_TV==0)
        PM25_TV = PM25_ADCV;
    else
        PM25_TV = ((PM25_ADCV - 25) * 3.89221556) * 0.06 + 0.94 * PM25_TV;
    if (MQ7_ADCV<=40)
        MQ7_TV = 0;
    else
        MQ7_TV = (MQ7_ADCV - 40) * 5;
    if (MQ135_ADCV<=10)
        MQ135_TV = 0;
    else
        MQ135_TV = (MQ135_ADCV - 10) * 5;

    if (MQ135_TV>MQ135_Threshold && MQ135_Boom == 0) MQ135_Boom = 6;
    if (MQ135_TV<MQ135_Threshold) MQ135_Boom = 0;
    if (MQ7_TV>MQ7_Threshold && MQ7_Boom == 0) MQ7_Boom = 6;
    if (MQ7_TV<MQ7_Threshold) MQ7_Boom = 0;
    if (PM25_TV>PM25_Threshold && PM25_Boom == 0) PM25_Boom = 6;
    if (PM25_TV<PM25_Threshold) PM25_Boom = 0;
}
void DisplayInfo::paintEvent(QPaintEvent *event)
{

    QPainter paint(this);//新建绘图控件
    QPen MyPen;//新建笔
    QFont MyFont;
    MyFont.setFamily(QString::fromLocal8Bit("黑体"));
    MyFont.setPixelSize(30);
    paint.setFont(MyFont);
    paint.setRenderHint(QPainter::Antialiasing);//设置平滑
    paint.setWindow(0,0,500,450);
    paint.fillRect(0,0,500,450,QColor(0,0,0));
    /*-----------------Boom-----------------*/
    MyPen.setColor(QColor(255,100,0));
    paint.setPen(MyPen);
    if (MQ135_Boom>0)
    {
        if (MQ135_Boom>3) paint.fillRect(240,6,135,23,QColor(255,0,0));
        MQ135_Boom--;
        if (MQ135_Boom == 0) MQ135_Boom = 6;
    }
    if (PM25_Boom>0)
    {
        if (PM25_Boom>3) paint.fillRect(10,415,50,23,QColor(255,0,0));
        PM25_Boom--;
        if (PM25_Boom == 0) PM25_Boom = 6;
    }
    if (MQ7_Boom>0)
    {
        if (MQ7_Boom>3) paint.fillRect(10,6,135,23,QColor(255,0,0));
        MQ7_Boom--;
        if (MQ7_Boom == 0) MQ7_Boom = 6;
    }

    /*----------------数值显示--------------*/
    MyFont.setPixelSize(20);
    paint.setFont(MyFont);
    MyPen.setColor(QColor(255,100,0));
    paint.setPen(MyPen);
    paint.drawText(10,25,
                   QString::fromLocal8Bit("一氧化碳浓度：")
                   +QString::number(MQ7_TV)
                   +QString::fromLocal8Bit(" PPM"));
    //qDebug()<<"--"<<MQ135_ADCV;
    MyPen.setColor(QColor(0,180,200));
    paint.setPen(MyPen);
    paint.drawText(240,25,
                   QString::fromLocal8Bit("有害物质浓度：")
                   +QString::number(MQ135_TV)
                   +QString::fromLocal8Bit(" PPM"));
    MyPen.setColor(QColor(20,200,50));
    paint.setPen(MyPen);
    paint.drawText(10,435,
                   QString::fromLocal8Bit("PM2.5：  ")
                   +QString::number(PM25_TV,2,2)
                   +QString::fromLocal8Bit(" 微克每立方米"));

    MyPen.setColor(QColor(255,255,255));
    paint.setPen(MyPen);
    MQ7_Table.push_front(MQ7_TV);
    while (MQ7_Table.size()>400) MQ7_Table.pop_back();
    //paint.drawLine(5,210,495,210);
    MyPen.setColor(QColor(255,100,0));
    paint.setPen(MyPen);
    for (int i = 1;i<MQ7_Table.size();i++)
    {
        paint.drawLine(50+i-1,205-MQ7_Table[i-1]*0.145,50+i,205-MQ7_Table[i]*0.145);
    }
    MyPen.setColor(QColor(0,180,200));
    paint.setPen(MyPen);
    MQ135_Table.push_front(MQ135_TV);
    while (MQ135_Table.size()>400) MQ135_Table.pop_back();
    for (int i = 1;i<MQ135_Table.size();i++)
    {
        paint.drawLine(50+i-1,205-MQ135_Table[i-1]*0.145,50+i,205-(MQ135_Table[i]*0.145));
    }
    MyPen.setColor(QColor(20,200,50));
    paint.setPen(MyPen);
    PM25_Table.push_front(PM25_TV);
    while (PM25_Table.size()>400) PM25_Table.pop_back();
    for (int i = 1;i<PM25_Table.size();i++)
    {
        paint.drawLine(50+i-1,400-PM25_Table[i-1]*0.29,50+i,400-(PM25_Table[i])*0.29);
    }
    MyPen.setWidth(2);;
    MyFont.setPixelSize(16);
    paint.setFont(MyFont);
    //kedu pm2.5
    MyPen.setColor(QColor(0,100,50));
    paint.setPen(MyPen);
    paint.drawLine(47,400,47,224);
    paint.drawText(30,400,"0");
    paint.drawLine(40,400,47,400);
    paint.drawText(15,356+8,"152");
    paint.drawLine(40,356,47,356);
    paint.drawText(15,312+8,"305");
    paint.drawLine(40,312,47,312);
    paint.drawText(15,268+8,"456");
    paint.drawLine(40,268,47,268);
    paint.drawText(15,224+8,"610");
    paint.drawLine(40,224,47,224);
    //kedu MQ7
    MyPen.setColor(QColor(255,100,0));
    paint.setPen(MyPen);
    paint.drawLine(47,205,47,60);
    paint.drawText(30,205,"0");
    paint.drawLine(40,205,47,205);
    paint.drawText(15,169+8,"250");
    paint.drawLine(40,169,47,169);
    paint.drawText(15,133+8,"500");
    paint.drawLine(40,133,47,133);
    paint.drawText(15,97+8,"750");
    paint.drawLine(40,97,47,97);
    paint.drawText(10,60+8,"1000");
    paint.drawLine(40,60,47,60);
    //kedu MQ135
    MyPen.setColor(QColor(0,180,200));
    paint.setPen(MyPen);
    paint.drawLine(455,205,455,60);
    paint.drawText(465,205,"0");
    paint.drawLine(455,205,460,205);
    paint.drawText(465,169+8,"250");
    paint.drawLine(455,169,460,169);
    paint.drawText(465,133+8,"500");
    paint.drawLine(455,133,460,133);
    paint.drawText(465,97+8,"750");
    paint.drawLine(455,97,460,97);
    paint.drawText(465,60+8,"1k");
    paint.drawLine(455,60,460,60);
    //*************
    MyPen.setColor(QColor(0,255,0));
    QVector<qreal> dashes;
    qreal space = 4;
    dashes << 4 << space << 4 <<space;
    MyPen.setDashPattern(dashes);
    MyPen.setWidth(1);
    paint.setPen(MyPen);
    paint.drawLine(50,400-PM25_Threshold*0.29,450,400-PM25_Threshold*0.29);

    MyPen.setColor(QColor(255,100,0));
    MyPen.setWidth(1);
    paint.setPen(MyPen);
    paint.drawLine(50,205-MQ7_Threshold*0.145,450,205-MQ7_Threshold*0.145);

    MyPen.setColor(QColor(0,180,200));
    MyPen.setWidth(1);
    paint.setPen(MyPen);
    paint.drawLine(50,205-MQ135_Threshold*0.145,450,205-MQ135_Threshold*0.145);
}
