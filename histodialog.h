#ifndef HISTODIALOG_H
#define HISTODIALOG_H

#include <QDialog>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_histogram.h>
#include "statistique.h"
#include "analyse.h"

namespace Ui {
class histoDialog;
}

class histoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit histoDialog(QWidget *parent = 0);
    ~histoDialog();

    QwtPlotMarker *curve_Q[5][4];
    QwtPlotCurve *curvenormal[4];
    double* loiNormal[4];
    double* xs[4];

    void initHisto();
    void setData(double**data, int nb_valeur);

private slots:

private:
    Ui::histoDialog *ui;
    QwtPlot *histoPlot[4];
    QwtPlotHistogram *histo[4];
    Statistique st;
    Analyse anal;

    /**
     * @brief Histogramme
     * @param histo
     * @param plot_H
     */
    void Histogramme(QwtPlotHistogram *histo, QwtPlot *plot_H);

    /**
     * @brief setDataInHisto
     * @param data
     * @param nb_valeur
     * @param c
     */
    double setDataInHisto(double**data,int nb_valeur,double max,int c);
    /**
     * @brief setQuartileInHisto
     * @param data
     * @param nb_valeur
     * @param c
     */
    void setQuartileInHisto(double**data, int nb_valeur, double max, double maxHisto, int c);
};

#endif // HISTODIALOG_H
