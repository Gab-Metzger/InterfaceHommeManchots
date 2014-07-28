#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QList>

class database
{
public:
    database();
    static QSqlDatabase dbConnect();
    static void dbRemove();
    static QList<QString> readDbRegister();
private:

};

#endif // DATABASE_H
