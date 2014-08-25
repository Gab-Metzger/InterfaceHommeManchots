#ifndef ANALYSE_H
#define ANALYSE_H

#include "analyse_global.h"
#include "flat.h"
#include "histogramme.h"
#include "statistique.h"

typedef struct statInfo{
    double moy;
    double var;
    double size;
    double *dataY;
    double *dataX;
    int nbVal;
}statInfo;

typedef struct histoMasseInfo{
    double errorTot;
    double min;
    double max;
    double error;
    QVector<double> Error;
    double k;
    statInfo statistique;
}histoMasseInfo;

class ANALYSESHARED_EXPORT Analyse
{

public:
    Analyse();
    int convertion(QString arg,int i);
    int compar(QString arg1,QString arg2,int nb);
    void tri_Bulle(QStringList *arg,double **masse);
    void lissageCourbe(double** dataCourbe, int nbVal);
    void miseAJourCaract(statInfo *statistique, double **masse, int numValidated);
    void setMasseDate(double** masse, QVector<double> caractCourbe, statInfo* statistique);
    QVector<double> analysePassage(double** dataCourbe,int nbVal,CaractFlat* indexArray,Stat* statArray);
    QVector<double> getWeightByFlat(double **data, int nb_valeur, CaractFlat *indexArray, Stat *statArray);
    bool isGoodFlat(QVector<double> caractCourbe, double **data, int size);
    void  initLoiNormal(double** loiNormal, double moy, double maxHisto, double max);
    void initInter(QVector<double> *element_interval,double* data,double max, int nb_valeur, double *histoMax);
    void calculMax(double **data, int nb_valeur,double max[4]);
    QVector<double> caractLoiNormal(double** data,double nbData,int c);

private:
    Flat fl;
    Histogramme ht;
    Statistique st;
};

#endif // ANALYSE_H
