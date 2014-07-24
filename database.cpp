#include "database.h"

database::database()
{
}

void database::dbConnect() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("antavia_ddu");
    db.setUserName("root");
    db.setPassword("narwolpobo");
    qDebug() << db.open();
}
