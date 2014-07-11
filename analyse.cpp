#include "analyse.h"

analyse::analyse()
{
}

/////////////////////////////////////////
/// Algorithm to detect flat in curves
/////////////////////////////////////////

void analyse::detectOneFlat(int *index, double *data, int nb_value) {
    while ((fabs(data[*index+1]-data[*index]) > SEUIL_V) && (*index < nb_value) && (data[*index] > SEUIL_V)) {
            *index = *index + 1;
    }
}

void analyse::lookIntoOneFlat(int *index, double *data, int nb_value) {
    int start = *index;
    while ((fabs(data[start]-data[*index]) < SEUIL_V) && (*index < nb_value)) {
        *index = *index + 1;
    }
}

void analyse::detectAllFlat(double **data, int nb_value,Flat *array) {
    int j=0;
    int nb_flat = 0;
    for (int i=0; i<4; i++){
        array->indexStart[i].resize(nb_value/SEUIL_H);
        array->indexStop[i].resize(nb_value/SEUIL_H);
        while(j < nb_value) {
            detectOneFlat(&j,data[i],nb_value);
            if ((nb_flat > 0) && (array->indexStop[i][nb_flat-1] == j)) {
                nb_flat--;
            }
            else {
                array->indexStart[i][nb_flat] = j;
            }
            lookIntoOneFlat(&j,data[i],nb_value);
            if ((j-array->indexStart[i][nb_flat]) > SEUIL_H) {
                array->indexStop[i][nb_flat] = j;
                nb_flat++;
            }
        }        
        if (array->indexStart[i][nb_flat-1] > array->indexStop[i][nb_flat-1]) {
            array->indexStart[i].resize(nb_flat-1);
            array->indexStop[i].resize(nb_flat-1);
        }
        else {
            array->indexStart[i].resize(nb_flat);
            array->indexStop[i].resize(nb_flat);
        }
        j=0;
        nb_flat=0;
    }
}

QVector <QVector<double> >analyse::meanValuesEachFlat(double **data, Flat *indexArray) {
    int i,j,k;
    double size;
    QVector<QVector<double> >sum(4);
    for (i=0; i<4; i++) {
        sum[i].resize(indexArray->indexStart[i].count());
        sum[i].fill(0.0);
        for (j=0; j<indexArray->indexStart[i].count();j++) {
            for (k=indexArray->indexStart[i][j]; k<=indexArray->indexStop[i][j]; k++) {
              sum[i][j] = sum[i][j] + data[i][k];
            }
            size = (indexArray->indexStop[i][j] - indexArray->indexStart[i][j] + 1);
            sum[i][j] = sum[i][j] / size;
        }
    }
    return sum;
}


void analyse::meanVarianceValuesByFlat(double **data,Stat *res, Flat *indexArray) {
    double size=0;

    res->sum.resize(4);
    res->var.resize(4);

    res->sum.fill(0.0);
    res->var.fill(0.0);

    for(int i=0;i< 4;i++) {
        size=0;
        for (int j=0; j<indexArray->indexStart[i].count();j++) {
            for (int k=indexArray->indexStart[i][j]; k<=indexArray->indexStop[i][j]; k++) {
              res->sum[i] += data[i][k];
              res->var[i] += (data[i][k] * data[i][k]);

            }
            size += (indexArray->indexStop[i][j] - indexArray->indexStart[i][j] + 1);
        }

        res->sum[i] = res->sum[i] / size;
        res->var[i] = res->var[i] / size;

        res->var[i] -= (res->sum[i] * res->sum[i]);
        res->length[i] = size;
    }

}

void analyse::lengthOfEachFlat(int i, QVector<double>meanValues, Flat *indexArray, double minMean, double maxMean) {
    double mean = 0.0;
    int size = 0, incr = 0;
    for(int j=0; j<indexArray->indexStart[i].count();j++) {
      mean = mean + (indexArray->indexStop[i][j] - indexArray->indexStart[i][j] + 1);
    }
    mean = mean / indexArray->indexStart[i].count();

    for (int j=0; j<indexArray->indexStart[i].count(); j++) {
        size = (indexArray->indexStop[i][j] - indexArray->indexStart[i][j] + 1);
        if (size >= (90*mean/100)) {
            if ((meanValues[j] >= minMean) && (meanValues[j] <= maxMean)) {           
                indexArray->indexStart[i][incr] = indexArray->indexStart[i][j];
                indexArray->indexStop[i][incr] = indexArray->indexStop[i][j];
                incr++;
            }
        }
    }

    if ( incr > 0 ) {
        indexArray->indexStart[i].resize(incr);
        indexArray->indexStop[i].resize(incr);
    }
}

void analyse::lengthGoodFlats(QVector<QVector<double> >meanValues, Flat *indexArray) {
   statistique STAT;
   int i, nb_element_R;
   double moy=0.0,var=0.0,*quartile=(double*)calloc(4,sizeof(double)),d=0.0,f=0.0;

   for (i=0;i<4;i++) {
        STAT.moustach(meanValues[i].data(),meanValues[i].count(),&d,&f,&moy,&var,&nb_element_R,quartile);
        lengthOfEachFlat(i,meanValues[i],indexArray,d,f);
   }
   free(quartile);
}

QVector<double> analyse::getTruthfulIndex(double lengthFlats[4], int lengthTotal) {
    QVector<double> index(4,0);
    for (int i=0;i<4;i++) {
       index[i] = lengthFlats[i] / lengthTotal;
    }

    return index;
}

QVector<double> analyse::getWeightByFlat(double **data, int nb_valeur, Flat *indexArray, Stat *statArray) {
    QVector<QVector<double> >meanValues;
    QVector<double> confidence(4), res(2);

    indexArray->indexStart.resize(4);
    indexArray->indexStop.resize(4);

    detectAllFlat(data, nb_valeur, indexArray);
    meanValues = meanValuesEachFlat(data,indexArray);
    lengthGoodFlats(meanValues,indexArray);
    meanVarianceValuesByFlat(data,statArray,indexArray);
    confidence = getTruthfulIndex(statArray->length,nb_valeur);

    //Confidence of a Flat
    double indexFlat = confidence[3] / statArray->var[3];
    //Confidence of a Curve
    double indexCurve = double(nb_valeur)/1000;
    //Confidence of Weight
    res[1] = indexFlat*indexCurve;

    //Weight
    res[0] = statArray->sum[3];

    return res;
}
