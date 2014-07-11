#ifndef DETECTIONS_H
#define DETECTIONS_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QTableView>
#include <math.h>
#include <QDebug>


namespace Ui {
class Detections;
}

class Detections : public QDialog
{
    Q_OBJECT

public:
    explicit Detections(QWidget *parent = 0);
    ~Detections();
    int spaceBetweenHour(int ourHour, int endTheo, int ligne);

private:
    Ui::Detections *ui;
    QSqlQueryModel *plainModel;
    QTableView* createView(QSqlQueryModel *model);
    void initializeModel(QSqlQueryModel *model);
    QVector<QVector<int> > extractRaws(int plateau, QString date, QString heure);
};

#endif // DETECTIONS_H
