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

namespace Ui {
class Resultat;
}

class Resultat : public QDialog
{
    Q_OBJECT

public:
    explicit Resultat(QString fichierManchot, QWidget *parent = 0);
    ~Resultat();
    void caractManchot(double poidsTheo, QString fichierManchot);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Resultat *ui;
    lecture_fichiers lect;
    analyse anal;

    QwtPlot *histoPlot;
    QwtPlotHistogram *histo;
    QString filename;
};

#endif // RESULTAT_H