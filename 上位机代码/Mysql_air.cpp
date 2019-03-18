#include "Mysql_air.h"
#include <QDateTime>
#include <QSqlRecord>
MySQL_Air::MySQL_Air()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","airp1");
    db.setDatabaseName("AirDb.db");
    db.open();
    QSqlQuery query(db);
    query.exec("create table if not exists table_1("
                         "ID INT(4) PRIMARY KEY,"
                         "TIME  VERCHAR,"
                         "PM25  VERCHAR,"
                         "MQ135  VERCHAR,"
                         "MQ7 VERCHAR"
                         ");");
    TableIndex = 1;
}

void MySQL_Air::SQL_AddData(QString pm25,QString mq135,QString mq7)
{
    QSqlDatabase db = QSqlDatabase::database("airp1");
    QSqlQuery query(db);
    TableIndex = 0;
    query.exec("select * from table_1;");
    while (query.next())
    {TableIndex = query.value(0).toInt();}
    query.prepare("insert into table_1(ID,TIME,PM25,MQ135,MQ7)"
                  "VALUES (:id,:time,:pm25,:zzz,:xxx);");
    query.bindValue(":id",TableIndex+1);
    query.bindValue(":time",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
    query.bindValue(":pm25",pm25);
    query.bindValue(":zzz",mq135);
    query.bindValue(":xxx",mq7);
    query.exec();
}
void MySQL_Air::SQL_Delete()
{
    TableIndex = 1;
    QSqlDatabase db = QSqlDatabase::database("airp1");
    QSqlQuery query(db);
    query.prepare("delete from table_1;");
    query.exec();
}
