#include "database.h"

database::database()
{
}

QSqlDatabase database::dbConnect() {
    QVariantList reading;
    QList<QString> list;
    reading = ReadRegister::readInRegister("Database/dbCredentials").toList();
    foreach(QVariant v, reading) {
        list << v.toString();
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","ManchotDb");
    db.setHostName(list.at(0));
    db.setDatabaseName(list.at(1));
    db.setUserName(list.at(2));
    db.setPassword(list.at(3));
    return db;
}

void database::dbRemove() {
    QSqlDatabase::removeDatabase(QString("dbConnection"));
}

void database::updateManchot(double weight, int transition) {
    QSqlDatabase db = QSqlDatabase::database("ManchotDb");
    QSqlQuery query(db);
    query.prepare("UPDATE transitions SET analyse_poids=? WHERE id=?");
    query.addBindValue(weight);
    query.addBindValue(transition);
    if(query.exec()) {
        qDebug() << "Weight updated !";
    }
    else {
        qDebug() << "Error query" << db.lastError();
    }
}

QString database::displayWeightOnMainWindow(int numTransition, QString output) {
    QSqlDatabase db = QSqlDatabase::database("ManchotDb");
    QSqlQuery query(db);
    query.prepare("SELECT analyse_poids FROM transitions WHERE id=?");
    query.addBindValue(numTransition);
    query.exec();
    query.next();
    output = output + QString("\nPoids dans la BDD : " + query.value(0).toString() + " kg");

    return output;
}
