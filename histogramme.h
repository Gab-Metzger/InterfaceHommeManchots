#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

#include <QVector>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_histogram.h>
#include <statistique.h>

class Histogramme
{
public:
    Histogramme();

    QVector<double> setConfidence(int nbR, double res, double moy, double var, double quar);
};

#endif // HISTOGRAMME_H
