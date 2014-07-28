#include "database.h"

database::database()
{
}

QSqlDatabase database::dbConnect() {
    QList<QString> list = database::readDbRegister();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",QString("dbConnection"));
    db.setHostName(list.at(0));
    db.setDatabaseName(list.at(1));
    db.setUserName(list.at(2));
    db.setPassword(list.at(3));
    return db;
}

void database::dbRemove() {
    QSqlDatabase::removeDatabase(QString("dbConnection"));
}

QList<QString> database::readDbRegister() {
    QVariantList reading;
    QList<QString> db;
    QSettings settings("METZGER","IHManchots");

    reading = settings.value("Database/dbCredentials").toList();
    foreach(QVariant v, reading) {
        db << v.toString();
    }

    return db;
}
