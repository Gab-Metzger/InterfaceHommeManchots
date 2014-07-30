#ifndef RESULTAT_H
#define RESULTAT_H

#include <QDialog>
#include "lecture_fichiers.h"
#include "analyse.h"
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_histogram.h>
#include <histodialog.h>
#include <QTimer>

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


namespace Ui {
class Resultat;
}

class Resultat : public QDialog
{
    Q_OBJECT

public:
    explicit Resultat(QString fichierManchot, QWidget *parent = 0);
    ~Resultat();
    void caractManchot(QString fichierManchot);

private slots:
    void on_theoValidateButton_clicked();

    void on_weightGraphButton_clicked();

    void on_histoGraphButton_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Resultat *ui;
    lecture_fichiers lect;
    analyse anal;
    double* masse[3];
    QVector< QList<double> > caractInfo;
    QStringList dateInfo;

    QwtPlot *histoPlot;
    QwtPlot *curvePlot;

    QwtPlotCurve *curve;
    QwtPlotMarker *curveT;

    QwtPlotHistogram *histo;
    QString fileName;
    double *tmpX,*tmpY;

    int nbPassage;

    int compar(QString arg1,QString arg2,int nb);
    void tri_Bulle(QStringList *arg, double **masse);

    void traceHistoMasse(double poidsTheo, QStringList periode1);
    void traceCourbe(statInfo statistique);

    void initComboBox(QStringList TabDate);
    void lissageCourbe(double **dataCourbe, int nbVal);
    void miseAJourCaract(statInfo *statistique, int numValidated);
    void initHistoMasse(double poidsTheo, histoMasseInfo *info, QVector<QStringList> periode);
    statInfo initStatistique(int n);
    histoMasseInfo initInfo(statInfo statistique);
    void miseAJourPeriode();
};

#endif // RESULTAT_H
