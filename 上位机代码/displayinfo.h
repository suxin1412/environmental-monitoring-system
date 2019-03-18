#ifndef DISPLAYINFO_H
#define DISPLAYINFO_H

#include <QWidget>

#include <QVector>
class DisplayInfo : public QWidget
{
    Q_OBJECT
public:
    int PM25_ADCV;
    int MQ7_ADCV;
    int MQ135_ADCV;
    double PM25_TV;
    double MQ7_TV;
    double MQ135_TV;
    QVector <double> PM25_Table;
    QVector <double> MQ7_Table;
    QVector <double> MQ135_Table;
    short PM25_Boom;
    short MQ7_Boom;
    short MQ135_Boom;
    double PM25_Threshold;
    double MQ7_Threshold;
    double MQ135_Threshold;
public:
    explicit DisplayInfo(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
signals:

public slots:
    void GXData();
};

#endif // DISPLAYINFO_H
