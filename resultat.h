#ifndef RESULTAT_H
#define RESULTAT_H

#include <QDialog>
#include "lecture_fichiers.h"
#include "analyse.h"
#include "database.h"
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

namespace Ui {
class Resultat;
}

class Resultat : public QDialog
{
    Q_OBJECT

public:
    explicit Resultat(QString fichierManchot, QWidget *parent = 0);
    ~Resultat();
    void caractManchot(QString fichierManchot, bool shouldUpdateDatabase=false);

private slots:
    void on_theoValidateButton_clicked();

    void on_weightGraphButton_clicked();

    void on_histoGraphButton_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Resultat *ui;
    lecture_fichiers lect;
    Analyse anal;

    QStringList dateInfo;

    QwtPlot *histoPlot;
    QwtPlot *curvePlot;

    QwtPlotCurve *curve;
    QwtPlotMarker *curveT;

    QwtPlotHistogram *histo;
    QString fileName;

    double *tmpX,*tmpY,*masse[3];
    bool init;
    int nbPassage,numValidated;

    statInfo initStatistique(int n);
    histoMasseInfo initInfo(statInfo statistique);

    void traceHistoMasse(double poidsTheo, QStringList periode1);
    void traceCourbe(statInfo statistique);

    void initComboBox(QStringList TabDate);
    void initHistoMasse(double poidsTheo, histoMasseInfo *info, QVector<QStringList> periode);

    void miseAJourPeriode();
};

#endif // RESULTAT_H
