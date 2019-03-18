#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "displayinfo.h"
#include "Mysql_air.h"
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSerialPort *serial;
    DisplayInfo *Display;
    MySQL_Air *SQL_Air;
    QStandardItemModel *model_t;
    long GXTime;

private slots:
    void on_pushButton_ScanfCom_clicked();

    void on_pushButton_OpenCom_clicked();

    void Read_Data();

    void on_lineEdit_MQ7_editingFinished();

    void on_lineEdit_PM25_editingFinished();

    void on_lineEdit_MQ135_editingFinished();

    void on_pushButton_ScanfSQL_clicked();

    void on_pushButton_CleanDATA_clicked();

    void SQL_Timer();

    void on_comboBox_Speed_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
