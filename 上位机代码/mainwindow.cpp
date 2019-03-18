#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox_ComList->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial_t;
        serial_t.setPort(info);
        if(serial_t.open(QIODevice::ReadWrite))
        {
            ui->comboBox_ComList->addItem(serial_t.portName());
            serial_t.close();
        }
    }
    serial = new QSerialPort;
    Display = new DisplayInfo;
    ui->scrollArea->setWidget(Display);
    QTimer *MyTimer = new QTimer;
    MyTimer->start(50);
    connect(MyTimer,SIGNAL(timeout()),Display,SLOT(update()));
    connect(MyTimer,SIGNAL(timeout()),Display,SLOT(GXData()));
    connect(MyTimer,SIGNAL(timeout()),this,SLOT(SQL_Timer()));
    model_t = new QStandardItemModel;
    ui->tableView->setModel(model_t);
    model_t->setColumnCount(4);
    model_t->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("时间"));
    model_t->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("PM2.5浓度"));
    model_t->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("有害气体浓度"));
    model_t->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("一氧化碳浓度"));
    ui->tableView->setColumnWidth(0,147);
    ui->tableView->setColumnWidth(1,114);
    ui->tableView->setColumnWidth(2,115);
    ui->tableView->setColumnWidth(3,116);
    SQL_Air =  new MySQL_Air;
    GXTime = 500;
    QSqlDatabase db = QSqlDatabase::database("airp1");
    QSqlQuery query(db);
    query.exec("select * from table_1;");
    int i = 0;
    model_t->clear();
    model_t->setColumnCount(4);
    model_t->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("时间"));
    model_t->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("PM2.5浓度"));
    model_t->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("有害气体浓度"));
    model_t->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("一氧化碳浓度"));
    ui->tableView->setColumnWidth(0,140);
    ui->tableView->setColumnWidth(1,110);
    ui->tableView->setColumnWidth(2,110);
    ui->tableView->setColumnWidth(3,114);
    while (query.next())
    {
        model_t->setItem(i,0,new QStandardItem(query.value(1).toString()));
        model_t->setItem(i,1,new QStandardItem(query.value(2).toString()));
        model_t->setItem(i,2,new QStandardItem(query.value(3).toString()));
        model_t->setItem(i++,3,new QStandardItem(query.value(4).toString()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ScanfCom_clicked()
{
    ui->comboBox_ComList->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial_t;
        serial_t.setPort(info);
        if(serial_t.open(QIODevice::ReadWrite))
        {
            ui->comboBox_ComList->addItem(serial_t.portName());
            serial_t.close();
        }
    }
}

void MainWindow::on_pushButton_OpenCom_clicked()
{
    if (ui->comboBox_ComList->currentText()=="") return;
    if (serial->isOpen() == false)
    {
        serial->setPortName(ui->comboBox_ComList->currentText());
        serial->open(QIODevice::ReadWrite);
        serial->setBaudRate(115200);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
        ui->pushButton_OpenCom->setText(QString::fromLocal8Bit("关闭端口"));
    }else{
        serial->clear();
        serial->close();
        ui->pushButton_OpenCom->setText(QString::fromLocal8Bit("打开端口"));
    }
}

void MainWindow::Read_Data()
{
    QByteArray buf;
    double TempDistance = 0;
    buf = serial->readAll();
    //qDebug()<<buf;
    if (!buf.isEmpty())
    {
        TempDistance = buf.toDouble();
        if (TempDistance>1000&&TempDistance<2000) {
            Display->PM25_ADCV = TempDistance - 1000;
            Display->PM25_ADCV -= 5;
            if (Display->PM25_ADCV<25) Display->PM25_ADCV = 26;
        }
        if (TempDistance>2000&&TempDistance<3000) {
            Display->MQ7_ADCV = TempDistance - 2000;
        }
        if (TempDistance>3000&&TempDistance<4000) {
            Display->MQ135_ADCV = TempDistance - 3000;
            //qDebug()<<TempDistance;
        }
    }
    buf.clear();
}



void MainWindow::on_lineEdit_MQ7_editingFinished()
{
    if (ui->lineEdit_MQ7->text().toDouble()<0 ||ui->lineEdit_MQ7->text().toDouble()>1000) return;
    Display->MQ7_Threshold = ui->lineEdit_MQ7->text().toDouble();
}

void MainWindow::on_lineEdit_PM25_editingFinished()
{
    if (ui->lineEdit_PM25->text().toDouble()<0 || ui->lineEdit_PM25->text().toDouble()>650) return;
    Display->PM25_Threshold = ui->lineEdit_PM25->text().toDouble();
}

void MainWindow::on_lineEdit_MQ135_editingFinished()
{
    if (ui->lineEdit_MQ135->text().toDouble()<0||ui->lineEdit_MQ135->text().toDouble()>1000) return;
    Display->MQ135_Threshold = ui->lineEdit_MQ135->text().toDouble();
}

void MainWindow::on_pushButton_ScanfSQL_clicked()
{
    SQL_Air->SQL_AddData(QString::number(Display->PM25_TV,'f',2),
                         QString::number(Display->MQ135_TV),
                         QString::number(Display->MQ7_TV));
    QSqlDatabase db = QSqlDatabase::database("airp1");
    QSqlQuery query(db);
    query.exec("select * from table_1;");
    int i = 0;
    model_t->clear();
    model_t->setColumnCount(4);
    model_t->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("时间"));
    model_t->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("PM2.5浓度"));
    model_t->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("有害气体浓度"));
    model_t->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("一氧化碳浓度"));
    ui->tableView->setColumnWidth(0,140);
    ui->tableView->setColumnWidth(1,110);
    ui->tableView->setColumnWidth(2,110);
    ui->tableView->setColumnWidth(3,114);
    while (query.next())
    {
        model_t->setItem(i,0,new QStandardItem(query.value(1).toString()));
        model_t->setItem(i,1,new QStandardItem(query.value(2).toString()));
        model_t->setItem(i,2,new QStandardItem(query.value(3).toString()));
        model_t->setItem(i++,3,new QStandardItem(query.value(4).toString()));
        //qDebug()<<query.value(0).toInt()<<query.value(1)<<query.value(2)<<query.value(3);
    }
}

void MainWindow::on_pushButton_CleanDATA_clicked()
{
    SQL_Air->SQL_Delete();
    QSqlDatabase db = QSqlDatabase::database("airp1");
    QSqlQuery query(db);
    query.exec("select * from table_1;");
    int i = 0;
    model_t->clear();
    model_t->setColumnCount(4);
    model_t->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("时间"));
    model_t->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("PM2.5浓度"));
    model_t->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("有害气体浓度"));
    model_t->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("一氧化碳浓度"));
    ui->tableView->setColumnWidth(0,140);
    ui->tableView->setColumnWidth(1,110);
    ui->tableView->setColumnWidth(2,110);
    ui->tableView->setColumnWidth(3,114);
    while (query.next())
    {
        model_t->setItem(i,0,new QStandardItem(query.value(1).toString()));
        model_t->setItem(i,1,new QStandardItem(query.value(2).toString()));
        model_t->setItem(i,2,new QStandardItem(query.value(3).toString()));
        model_t->setItem(i++,3,new QStandardItem(query.value(4).toString()));
        //qDebug()<<query.value(0).toInt()<<query.value(1)<<query.value(2)<<query.value(3);
    }
}
void MainWindow::SQL_Timer()
{
    static long TimeAotuAdd = 0;
    TimeAotuAdd++;
    //qDebug()<<TimeAotuAdd;
    if (TimeAotuAdd == GXTime)
    {
        SQL_Air->SQL_AddData(QString::number(Display->PM25_TV,'f',2),
                             QString::number(Display->MQ135_TV),
                             QString::number(Display->MQ7_TV));
        QSqlDatabase db = QSqlDatabase::database("airp1");
        QSqlQuery query(db);
        query.exec("select * from table_1;");
        int i = 0;
        model_t->clear();
        model_t->setColumnCount(4);
        model_t->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("时间"));
        model_t->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("PM2.5浓度"));
        model_t->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("有害气体浓度"));
        model_t->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("一氧化碳浓度"));
        ui->tableView->setColumnWidth(0,140);
        ui->tableView->setColumnWidth(1,110);
        ui->tableView->setColumnWidth(2,110);
        ui->tableView->setColumnWidth(3,114);
        while (query.next())
        {
            model_t->setItem(i,0,new QStandardItem(query.value(1).toString()));
            model_t->setItem(i,1,new QStandardItem(query.value(2).toString()));
            model_t->setItem(i,2,new QStandardItem(query.value(3).toString()));
            model_t->setItem(i++,3,new QStandardItem(query.value(4).toString()));
            //qDebug()<<query.value(0).toInt()<<query.value(1)<<query.value(2)<<query.value(3);
        }
        TimeAotuAdd = 0;
    }
}
void MainWindow::on_comboBox_Speed_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        GXTime = 500;
        break;
    case 1:
        GXTime = 1000;
        break;
    case 2:
        GXTime = 5000;
        break;
    case 3:
        GXTime = 15000;
        break;
    case 4:
        GXTime = 30000;
        break;
    case 5:
        GXTime = 60000;
        break;
    default:
        break;
    }
    //qDebug()<<GXTime;
}
