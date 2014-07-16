#ifndef PARCOURSMANCHOTS_H
#define PARCOURSMANCHOTS_H

#include <QVector>
#include <QFile>
#include <QString>
#include <QDir>
#include <QDebug>

typedef struct Results {
    int idManchot;
    double weightFlatAlgo;
    double weightHistoAlgo;
    double indexFlatConfidence;
} exportResults;

class ParcoursManchots
{
public:
    ParcoursManchots();

private:
    QVector<QString> searchRightFile(int idManchot);
    QVector<QVector<double> > processLineAllocation(QString line);
    QVector<QVector<double> > processLineSaveData(QVector<QVector<double> >data, QString line);
    QVector<QVector<double> > openAndDigIntoFiles(QVector<QString> fileNames);
};

#endif // PARCOURSMANCHOTS_H
