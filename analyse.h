#ifndef ANALYSE_H
#define ANALYSE_H

#include <QVector>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include "math.h"
#include "statistique.h"
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_layout.h>

#define SEUIL_V 0.3
#define SEUIL_H 50


typedef struct indexTab {
    QVector<QVector<int> >indexStart;
    QVector<QVector<int> >indexStop;
} Flat;

typedef struct caractStat {
   QVector<double> sum;
   QVector<double> var;
   double length[4];
}Stat;

class analyse
{
public:
    analyse();
     bool isGoodFlat(QVector<double> caractCourbe, double **data, int size);
    /**
     * @brief detectAllFlat
     * @param data
     * @param nb_value
     * @param array
     */
    void detectAllFlat(double **data, int nb_value, Flat *array);
    /**
     * @brief meanValuesEachFlat
     * @param data
     * @param indexArray
     * @return
     */
    QVector<QVector<double> > meanValuesEachFlat(double **data, Flat *indexArray);
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
    void lengthOfEachFlat(int i, QVector<double>meanValues, Flat *indexArray, double minMean, double maxMean);
    /**
     * @brief lengthGoodFlats
     * @param meanValues
     * @param indexArray
     * @param meanWeight
     * @return
     */
    void lengthGoodFlats(QVector<QVector<double> >meanValues, Flat *indexArray);
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
    void meanVarianceValuesByFlat(double **data, Stat *res, Flat *indexArray);
    /**
     * @brief getWeightByFlat
     * @param data
     * @param nb_valeur
     * @param indexArray
     * @param statArray
     * @return
     */
    QVector<double> getWeightByFlat(double **data, int nb_valeur, Flat *indexArray, Stat *statArray);
private:
    QwtPlot *plot;
    QwtPlotCurve *curve[4];
    QwtPlotCurve *curveWeight[4];
    statistique stat;
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

#endif // ANALYSE_H
