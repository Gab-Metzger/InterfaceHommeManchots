#ifndef DETECTIONS_H
#define DETECTIONS_H

#include <QtSql>
#include <math.h>
#include <QDebug>
#include <QVector>

class Detections
{
public:
    Detections();
    int spaceBetweenHour(int ourHour[], int HourTheo[],int IdManchot,int line[]);
    QVector<QVector<int> > extractRaws(int plateau, QString date, int heure, int idManchot);
    QVector<QString> listTdms(int IdManchot);

};

#endif // DETECTIONS_H
