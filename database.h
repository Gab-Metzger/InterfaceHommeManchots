#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QList>
#include "readregister.h"

class database
{
public:
    database();
    static QSqlDatabase dbConnect();
    static void dbRemove();
    static QList<QString> readDbRegister();
    static void updateManchot(double weight, int transition);
    static QString displayWeightOnMainWindow(int numTransition, QString output);
private:

};

#endif // DATABASE_H
