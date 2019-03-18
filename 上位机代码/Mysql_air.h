#ifndef MYSQL_AIR_H
#define MYSQL_AIR_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class MySQL_Air
{
public:
    int TableIndex;
    MySQL_Air();
    void SQL_AddData(QString pm25,QString mq135,QString mq7);
    void SQL_Delete();
};

#endif // MYSQL_AIR_H
