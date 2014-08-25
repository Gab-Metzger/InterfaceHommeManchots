#ifndef FLAT_H
#define FLAT_H

#include <QVector>
#include <QDebug>
#include <QString>
#include <QSettings>
#include "math.h"
#include "statistique.h"

typedef struct indexTab {
    QVector<QVector<int> >indexStart;
    QVector<QVector<int> >indexStop;
} CaractFlat;

typedef struct caractStat {
   QVector<double> sum;
   QVector<double> var;
   double length[4];
}Stat;

class Flat
{
public:
     Flat();
    /**
     * @brief detectAllFlat
     * @param data
     * @param nb_value
     * @param array
     */
    void detectAllFlat(double **data, int nb_value, CaractFlat *array);
    /**
     * @brief meanValuesEachFlat
     * @param data
     * @param indexArray
     * @return
     */
    QVector<QVector<double> > meanValuesEachFlat(double **data, CaractFlat *indexArray);
    /**
     * @brief lengthOfEachFlat
     * @param i
     * @param meanValues
     * @param indexArray
     * @param minMean
     * @param maxMean
     * @param meanWeight
     * @return
     */
    void lengthOfEachFlat(int i, QVector<double>meanValues, CaractFlat *indexArray, double minMean, double maxMean);
    /**
     * @brief lengthGoodFlats
     * @param meanValues
     * @param indexArray
     * @param meanWeight
     * @return
     */
    void lengthGoodFlats(QVector<QVector<double> >meanValues, CaractFlat *indexArray);
    /**
     * @brief getTruthfulIndex
     * @param lengthFlats
     * @param lengthTotal
     * @return
     */
    QVector<double> getTruthfulIndex(double lengthFlats[], int lengthTotal);
    /**
     * @brief meanVarianceValuesByFlat
     * @param data
     * @param res
     * @param indexArray
     */
    void meanVarianceValuesByFlat(double **data, Stat *res, CaractFlat *indexArray);
    QVector<double> getConfidence( Stat *statArray,int nbR,double var);

private:
    int horizontalThreshold;
    double verticalThreshold;
    double levelThreshold;
    Statistique st;
    /**
     * @brief detectOneFlat
     * @param indice
     * @param data
     * @param nb_value
     */
    void detectOneFlat(int *indice, double *data, int nb_value);
    /**
     * @brief lookIntoOneFlat
     * @param index
     * @param data
     * @param nb_value
     */
    void lookIntoOneFlat(int *index, int start, double *data, int nb_value);

};

#endif // FLAT_H
