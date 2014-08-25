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
#include "parametres.h"
#include "algoplatmanager.h"
#include "database.h"
#include "updatedatabase.h"
#include "readregister.h"

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
    OpenTdms ot;
    Analyse anal;
    histoDialog *histoWindow;
    Parametres *paramWindow;
    Resultat *resultWindow;
    OuvertureManchot *omWindow;
    AlgoPlatManager *managerPlatWindow;
    UpdateDatabase *updateDatabaseWindow;

    double** l_data;
    double* xs;
    double *dataInterval[4];
    int nb_valeur;
    int selection[4];
    int nb_passage;
    int numTransition;
    int sizeFlat[4];
    int max;
    char cas;
    QString filename;
    QwtPlot *plot;
    QwtPlotCurve *curve[4];
    QwtPlotCurve **curveFlat[4];
    QPen *color [4];

    int errorOpenFile;
    bool existFlat;

    double* tmp;
    QSqlDatabase db;
    double finalWeight;

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
     * @brief MainWindow::initGraphicComposents
     * @param maxInt
     */
    void initGraphicComposents(int minInt, int maxInt);
    /**
     * @brief MaxWindow::initPlot
     * @param dateTime
     */
    void initPlot(QString dateTime, int min);
    /**
     * @brief calculDatas
     * @param min
     * @param maxInt
     * @param shouldSmooth
     * @param xs
     * @param dataInterval
     * @return
     */
    int calculDatas(int min, int maxInt, int shouldSmooth);
    /**
     * @brief MainWindow::calculXDatas
     * @param min
     * @param max
     * @param xs
     */
    void calculXDatas(int min, int max);
    /**
     * @brief MainWindow::calculYDatas
     * @param minInt
     * @param max
     * @return
     */
    void calculYDatas(int minInt, int max, int shouldSmooth);
    /**
     * @brief MainWindow::displayResults
     * @param caractCourbe
     * @return
     */
    QString displayResults(QVector<double> caractCourbe);
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
    void traceFlat(CaractFlat *indexArray);
    /**
     * @brief removeTempDir
     */
    void removeTempDir();

private slots:
    void on_actionOuvrir_triggered();
    void on_actionQuitter_triggered();
    void on_passageSpinBox_valueChanged(int arg1);
    void on_balance1_clicked(bool checked);
    void on_balance2_clicked(bool checked);
    void on_balance3_clicked(bool checked);
    void on_sommeBalance_clicked(bool checked);
    void on_actionAfficher_triggered();
    void on_drawButton_clicked();
    void on_actionAlgorithme_de_plats_triggered();
    void on_actionA_propos_triggered();
    void on_resetButton_clicked();
    void on_actionEnregistrer_triggered();
    void on_actionLisser_la_courbe_triggered();
    void on_actionOuvrir_manchot_triggered();
    void on_simpleCase_clicked();
    void on_complexCase_clicked();
    void on_actionR_sultat_triggered();
    void on_actionOptions_triggered();
    void on_actionMettre_jour_dans_la_BDD_triggered();
    void on_actionSauvegarder_Poids_triggered();
};

#endif // MAINWINDOW_H
