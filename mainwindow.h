#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_layout.h>
#include <QFileDialog>
#include <qwt_plot_zoomer.h>
#include <histodialog.h>
#include <QMessageBox>
#include <QSettings>
#include "lecture_fichiers.h"
#include "analyse.h"
#include "opentdmsdialog.h"
#include "ouverturemanchot.h"
#include "resultat.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    lecture_fichiers FICHIERS;
    double** l_data;
    int nb_valeur;
    int selection[4];
    int nb_passage;
    int sizeFlat[4];
    char cas;
    QString filename;
    QwtPlot *plot;
    QwtPlotCurve *curve[4];
    QwtPlotCurve **curveFlat[4];
    QPen *color [4];
    histoDialog *histoWindow;
    OuvertureManchot *omWindow;
    Resultat *resultWindow;
    int errorOpenFile;
    bool existFlat;
    analyse anal;

    /**
     * @brief affiche
     * @param i
     * @param select
     */
    void affiche(int i,bool select);
    /**
     * @brief tracer
     * @param num_passage
     * @param minInt
     * @param maxInt
     */
    void tracer(int num_passage, int minInt, int maxInt, int shouldSmooth = 0);
    /**
     * @brief writeSettings
     */
    void writeSettings();
    /**
     * @brief readSettings
     */
    void readSettings();
    /**
     * @brief maxArray
     * @param index
     * @param meanWeight
     * @return
     */
    int maxArray(QVector<double> index, QVector<double> meanWeight);
    /**
     * @brief traceFlat
     * @param indexArray
     * @param statArray
     */
    void traceFlat(Flat *indexArray);

private slots:
    void on_actionOuvrir_triggered();
    void on_actionQuitter_triggered();
    void on_spinBox_valueChanged(int arg1);
    void on_balance1_clicked(bool checked);
    void on_balance2_clicked(bool checked);
    void on_balance3_clicked(bool checked);
    void on_sommebalance_clicked(bool checked);
    void on_actionAfficher_triggered();
    void on_pushButton_2_clicked();
    void on_actionAlgorithme_de_plats_triggered();
    void on_actionA_propos_triggered();
    void on_pushButton_clicked();
    void on_actionEnregistrer_triggered();
    void on_actionLisser_la_courbe_triggered();
    void on_actionOuvrir_manchot_triggered();
    void on_checkBox_2_clicked();
    void on_checkBox_clicked();
    void on_actionR_sultat_triggered();
};

#endif // MAINWINDOW_H
